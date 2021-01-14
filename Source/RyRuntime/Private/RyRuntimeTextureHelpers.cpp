// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeTextureHelpers.h"
#include "Runtime/Engine/Public/LatentActions.h"

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
		if(Texture && Texture->Resource)
		{
			Texture->UpdateResource();
		}
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if(Response.ElapsedTime() > 20.0f || !TextureIn.IsValid(false) || !TextureIn->Resource)
		{
			*Result = ERyTextureResourceResult::Failure;
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
			return;
		}
		if (TextureIn->Resource && TextureIn->Resource->TextureRHI)
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
