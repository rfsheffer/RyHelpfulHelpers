// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeRenderingHelpers.h"
#include "Core/Public/Misc/FileHelper.h"

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
	case SP_METAL_SM5_NOTESS:
		return ERyShaderPlatform::SP_METAL_SM5_NOTESS;
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
	case SP_VULKAN_SM5_LUMIN:
		return ERyShaderPlatform::SP_VULKAN_SM5_LUMIN;
	case SP_VULKAN_ES3_1_LUMIN:
		return ERyShaderPlatform::SP_VULKAN_ES3_1_LUMIN;
	case SP_METAL_TVOS:
		return ERyShaderPlatform::SP_METAL_TVOS;
	case SP_METAL_MRT_TVOS:
		return ERyShaderPlatform::SP_METAL_MRT_TVOS;
	case SP_VULKAN_SM5_ANDROID:
		return ERyShaderPlatform::SP_VULKAN_SM5_ANDROID;
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
bool URyRuntimeRenderingHelpers::TakeScreenshot(const FString& requestedPathOut, const ERyScreenShotMode screenshotMode, FString& pathOut)
{
	FString path;
	FString fileName;
	FString ext;
	FPaths::Split(requestedPathOut, path, fileName, ext);
	if(!IFileManager::Get().MakeDirectory(*path, true))
	{
		return false;
	}

	const FString filePathToTest = FPaths::Combine(path, fileName);
	if(FPaths::FileExists(filePathToTest + TEXT(".png")))
	{
		FFileHelper::GenerateNextBitmapFilename(filePathToTest, TEXT("png"), pathOut);
	}
	else
	{
		pathOut = filePathToTest + TEXT(".png");
	}
	
	switch (screenshotMode)
	{
	case ERyScreenShotMode::Game:
		{
			FScreenshotRequest::RequestScreenshot(pathOut, false, false);
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
		return true;
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
					FImageUtils::CompressImageArray(ImageSize.X, ImageSize.Y, ImageData, CompressedBitmap);
					if (FFileHelper::SaveArrayToFile(CompressedBitmap, *pathOut))
					{
						return true;
					}
				}
			}
		}
		return true;
#else
		break;
#endif
	case ERyScreenShotMode::EditorLevelViewport:
#if WITH_EDITOR
		{
			FScreenshotRequest::RequestScreenshot(pathOut, true, false);
			GScreenshotResolutionX = 0;
			GScreenshotResolutionY = 0;

			if (GEditor != nullptr)
			{
				GEditor->RedrawLevelEditingViewports();
			}
		}
		return true;
#else
		break;
#endif
	}

	return false;
}
