// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeTextureHelpers.h"
#include "LatentActions.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Runtime/Engine/Classes/Engine/Texture.h"
#include "Runtime/Engine/Public/TextureResource.h"
#include "Engine/Engine.h"

class FRyEnsureTextureResourceLatentAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	ERyTextureResourceResult* Result = nullptr;
	TWeakObjectPtr<UTexture> TextureIn;

	FRyEnsureTextureResourceLatentAction(const FLatentActionInfo& LatentInfo, UTexture* Texture, ERyTextureResourceResult& Mode)
        : ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
        , Result(&Mode)
		, TextureIn(Texture)
	{
#if ENGINE_MAJOR_VERSION >= 5
		if(Texture && Texture->GetResource())
#else
		if(Texture && Texture->Resource)
#endif
		{
			Texture->UpdateResource();
		}
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
#if ENGINE_MAJOR_VERSION >= 5
		if(Response.ElapsedTime() > 20.0f || !TextureIn.IsValid(false) || !TextureIn->GetResource())
#else
		if(Response.ElapsedTime() > 20.0f || !TextureIn.IsValid(false) || !TextureIn->Resource)
#endif
		{
			*Result = ERyTextureResourceResult::Failure;
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
			return;
		}
#if ENGINE_MAJOR_VERSION >= 5
		if (TextureIn->GetResource() && TextureIn->GetResource()->TextureRHI)
#else
		if (TextureIn->Resource && TextureIn->Resource->TextureRHI)
#endif
		{
			*Result = ERyTextureResourceResult::Success;
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		}
	}
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeTextureHelpers::UpdateTextureResource(UObject* WorldContextObject, UTexture* Texture, FLatentActionInfo LatentInfo, ERyTextureResourceResult& Result)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FRyEnsureTextureResourceLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			Result = ERyTextureResourceResult::Failure;
			FRyEnsureTextureResourceLatentAction* CompletionAction = new FRyEnsureTextureResourceLatentAction(LatentInfo, Texture, Result);

			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, CompletionAction);
		}
	}
}
