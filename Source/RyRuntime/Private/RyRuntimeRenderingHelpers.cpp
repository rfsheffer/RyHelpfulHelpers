// Copyright 2020-2022 Solar Storm Interactive

#include "RyRuntimeRenderingHelpers.h"
#include "RyRuntimeModule.h"
#include "Core/Public/Misc/FileHelper.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HighResScreenshot.h"
#include "RenderCore/Public/ShaderPipelineCache.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Runtime/Engine/Public/LatentActions.h"

#if WITH_EDITOR
	#include "Editor/EditorEngine.h"
	#include "IAssetViewport.h"
	#include "LevelEditor.h"
	#include "LevelEditorViewport.h"
	#include "ImageUtils.h"
	#include "Slate/SceneViewport.h"
	extern UNREALED_API UEditorEngine* GEditor;
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyShaderPlatform URyRuntimeRenderingHelpers::GetShaderPlatform(UObject* WorldContextObject)
{
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return ERyShaderPlatform::SP_INVALID;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !World->Scene)
	{
		return ERyShaderPlatform::SP_INVALID;
	}

	switch(World->Scene->GetShaderPlatform())
	{
	case SP_PCD3D_SM5:
		return ERyShaderPlatform::SP_PCD3D_SM5;
	case SP_METAL:
		return ERyShaderPlatform::SP_METAL;
	case SP_METAL_MRT:
		return ERyShaderPlatform::SP_METAL_MRT;
	case SP_PCD3D_ES3_1:
		return ERyShaderPlatform::SP_PCD3D_ES3_1;
	case SP_OPENGL_PCES3_1:
		return ERyShaderPlatform::SP_OPENGL_PCES3_1;
	case SP_METAL_SM5:
		return ERyShaderPlatform::SP_METAL_SM5;
	case SP_VULKAN_PCES3_1:
		return ERyShaderPlatform::SP_VULKAN_PCES3_1;
#if ENGINE_MAJOR_VERSION < 5
	case SP_METAL_SM5_NOTESS:
		return ERyShaderPlatform::SP_METAL_SM5_NOTESS;
#endif
	case SP_VULKAN_SM5:
		return ERyShaderPlatform::SP_VULKAN_SM5;
	case SP_VULKAN_ES3_1_ANDROID:
		return ERyShaderPlatform::SP_VULKAN_ES3_1_ANDROID;
	case SP_METAL_MACES3_1:
		return ERyShaderPlatform::SP_METAL_MACES3_1;
	case SP_OPENGL_ES3_1_ANDROID:
		return ERyShaderPlatform::SP_OPENGL_ES3_1_ANDROID;
	case SP_METAL_MRT_MAC:
		return ERyShaderPlatform::SP_METAL_MRT_MAC;
#if ENGINE_MAJOR_VERSION < 5
	case SP_VULKAN_SM5_LUMIN:
		return ERyShaderPlatform::SP_VULKAN_SM5_LUMIN;
	case SP_VULKAN_ES3_1_LUMIN:
		return ERyShaderPlatform::SP_VULKAN_ES3_1_LUMIN;
#endif
	case SP_METAL_TVOS:
		return ERyShaderPlatform::SP_METAL_TVOS;
	case SP_METAL_MRT_TVOS:
		return ERyShaderPlatform::SP_METAL_MRT_TVOS;
	case SP_VULKAN_SM5_ANDROID:
		return ERyShaderPlatform::SP_VULKAN_SM5_ANDROID;
#if ENGINE_MAJOR_VERSION >= 5
	case SP_PCD3D_SM6:
		return ERyShaderPlatform::SP_PCD3D_SM6;
	case SP_D3D_ES3_1_HOLOLENS:
		return ERyShaderPlatform::SP_D3D_ES3_1_HOLOLENS;
#endif
	default: ;
	}

	return ERyShaderPlatform::SP_INVALID;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyRenderingFeatureLevel URyRuntimeRenderingHelpers::GetRenderingFeatureLevel(UObject* WorldContextObject)
{
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return ERyRenderingFeatureLevel::INVALID;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !World->Scene)
	{
		return ERyRenderingFeatureLevel::INVALID;
	}

	switch(World->Scene->GetFeatureLevel())
	{
	case ERHIFeatureLevel::ES3_1:
		return ERyRenderingFeatureLevel::ES3_1;
	case ERHIFeatureLevel::SM5:
		return ERyRenderingFeatureLevel::SM5;
#if ENGINE_MAJOR_VERSION >= 5
	case ERHIFeatureLevel::SM6:
		return ERyRenderingFeatureLevel::SM6;
#endif
	default: ;
	}

	return ERyRenderingFeatureLevel::INVALID;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyShadingPath URyRuntimeRenderingHelpers::GetShadingPath(UObject* WorldContextObject)
{
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return ERyShadingPath::Invalid;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !World->Scene)
	{
		return ERyShadingPath::Invalid;
	}

	switch(World->Scene->GetShadingPath())
	{
	case EShadingPath::Mobile:
		return ERyShadingPath::Mobile;
	case EShadingPath::Deferred:
		return ERyShadingPath::Deferred;
	default: ;
	}

	return ERyShadingPath::Invalid;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
struct FTakeScreenshotAction : FPendingLatentAction
{
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool InProgress;
	bool &OutSuccess;
	FString& PathOut;
	float StartTime;
	UWorld* World;
	uint32 PreviousScreenshotX;
	uint32 PreviousScreenshotY;

	FTakeScreenshotAction(const FString& requestedPathOut, const ERyScreenShotMode screenshotMode, FString& pathOut, bool &outSuccess, UWorld* world, const FLatentActionInfo& InLatentInfo)
		: ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget)
		, OutSuccess(outSuccess)
		, PathOut(pathOut)
		, World(world)
	{
		InProgress = false;
		OutSuccess = false;
		check(World);

		PreviousScreenshotX = GScreenshotResolutionX;
		PreviousScreenshotY = GScreenshotResolutionX;

		StartTime = World->GetTimeSeconds();
		
		FString path;
		FString fileName;
		FString ext;
		FPaths::Split(requestedPathOut, path, fileName, ext);
		if(!IFileManager::Get().MakeDirectory(*path, true))
		{
			return;
		}

		const FString filePathToTest = FPaths::Combine(path, fileName);
		if(FPaths::FileExists(filePathToTest + TEXT(".png")))
		{
			FFileHelper::GenerateNextBitmapFilename(filePathToTest, TEXT("png"), PathOut);
		}
		else
		{
			PathOut = filePathToTest + TEXT(".png");
		}
		
		switch (screenshotMode)
		{
		case ERyScreenShotMode::Game:
			{
				if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() && UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected())
				{
					// In VR RequestScreenshot doesn't produce a proper image (either cropped or desaturated).
					// We use the highresscreenshot version to get around this.
					UGameViewportClient* GameViewportClient = GEngine->GameViewport;
					if(GameViewportClient && GameViewportClient->Viewport)
					{
						FHighResScreenshotConfig& screenShot = GetHighResScreenshotConfig();
						screenShot.SetFilename(PathOut);

						// Set to 0 which internally means "choose the best resolution"
						GScreenshotResolutionX = 0;
						GScreenshotResolutionY = 0;
						
						GameViewportClient->Viewport->TakeHighResScreenShot();
					}
					else
					{
						return;
					}
				}
				else
				{
					FScreenshotRequest::RequestScreenshot(PathOut, false, false);
					// Set to 0 which internally means "choose the best resolution"
					GScreenshotResolutionX = 0;
					GScreenshotResolutionY = 0;
#if WITH_EDITOR
					FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
					const TSharedPtr<IAssetViewport> ActiveAssetViewport = LevelEditorModule.GetFirstActiveViewport();
					if (ActiveAssetViewport.IsValid() && FSlateApplication::Get().FindWidgetWindow(ActiveAssetViewport->AsWidget()).IsValid())
					{
						GScreenshotResolutionX = ActiveAssetViewport->GetSharedActiveViewport()->GetSizeXY().X;
						GScreenshotResolutionY = ActiveAssetViewport->GetSharedActiveViewport()->GetSizeXY().Y;
					}
#endif
				}
			}
			InProgress = true;
			return;
		case ERyScreenShotMode::EditorActiveWindow:
#if WITH_EDITOR
			{
				const TSharedRef<SWidget> WindowRef = FSlateApplication::Get().GetActiveTopLevelWindow().ToSharedRef();

				TArray<FColor> ImageData;
				FIntVector ImageSize;

				if (FSlateApplication::Get().TakeScreenshot(WindowRef, ImageData, ImageSize))
				{
					if (IFileManager::Get().MakeDirectory(*FPaths::ScreenShotDir(), true))
					{
						TArray<uint8> CompressedBitmap;
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
						FImageUtils::ThumbnailCompressImageArray(ImageSize.X, ImageSize.Y, ImageData, CompressedBitmap);
#else
						FImageUtils::CompressImageArray(ImageSize.X, ImageSize.Y, ImageData, CompressedBitmap);
#endif
						if (FFileHelper::SaveArrayToFile(CompressedBitmap, *PathOut))
						{
							InProgress = true;
							return;
						}
					}
				}
			}
			InProgress = true;
			return;
#else
			break;
#endif
		case ERyScreenShotMode::EditorLevelViewport:
#if WITH_EDITOR
			{
				FScreenshotRequest::RequestScreenshot(PathOut, true, false);
				// Set to 0 which internally means "choose the best resolution"
				GScreenshotResolutionX = 0;
				GScreenshotResolutionY = 0;

				if (GEditor != nullptr)
				{
					GEditor->RedrawLevelEditingViewports();
				}
			}
			InProgress = true;
			return;
#else
			break;
#endif
		}
	}

	virtual ~FTakeScreenshotAction() override
	{
		
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		bool bFinished = !InProgress;
		if(InProgress)
		{
			bFinished = FPaths::FileExists(PathOut);
			if(!bFinished)
			{
				// Timeout
				check(World);
				if(StartTime + 2.0f <= World->GetTimeSeconds())
				{
					bFinished = true;
				}
			}
			else
			{
				OutSuccess = true;
			}
		}
		if(bFinished)
		{
			GScreenshotResolutionX = PreviousScreenshotX;
			GScreenshotResolutionX = PreviousScreenshotY;
		}
		Response.FinishAndTriggerIf(bFinished, ExecutionFunction, OutputLink, CallbackTarget);
	}

#if WITH_EDITOR
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("Pending screenshot to: %s"), *PathOut);
	}
#endif
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeRenderingHelpers::TakeScreenshot(UObject* WorldContextObject, FLatentActionInfo LatentInfo, const FString& requestedPathOut, const ERyScreenShotMode screenshotMode, FString& pathOut, bool &outSuccess)
{

	if (UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& latentManager = world->GetLatentActionManager();
		if (latentManager.FindExistingAction<FTakeScreenshotAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			// We always spawn a new load even if this node already queued one, the outside node handles this case
			FTakeScreenshotAction* newAction = new FTakeScreenshotAction(requestedPathOut, screenshotMode, pathOut, outSuccess, world, LatentInfo);
			latentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, newAction);
		}
		else
		{
			UE_LOG(LogRyRuntime, Warning, TEXT("TakeScreenshot not executed. The previous action hasn't finished yet."));
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
int32 URyRuntimeRenderingHelpers::ShaderPipelineCache_PrecompilesRemaining()
{
	return FShaderPipelineCache::NumPrecompilesRemaining();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeRenderingHelpers::ShaderPipelineCache_PauseBatching()
{
	FShaderPipelineCache::PauseBatching();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeRenderingHelpers::ShaderPipelineCache_ResumeBatching()
{
	FShaderPipelineCache::ResumeBatching();
}

static_assert(ERyBatchMode::Precompile == static_cast<ERyBatchMode>(FShaderPipelineCache::BatchMode::Precompile), "Batch mode enum has changed!");
//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeRenderingHelpers::ShaderPipelineCache_SetBatchMode(ERyBatchMode batchMode)
{
	FShaderPipelineCache::SetBatchMode(static_cast<FShaderPipelineCache::BatchMode>(batchMode));
}
