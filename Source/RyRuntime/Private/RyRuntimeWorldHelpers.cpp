// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeWorldHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class FRyTickUntilStopAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

	const ERyTickUntilStopInExecs* InExecs;
	ERyTickUntilStopOutExecs* OutExecs;

	FRyTickUntilStopAction(FLatentActionInfo const& latentInfo, const ERyTickUntilStopInExecs* inExecs, ERyTickUntilStopOutExecs* outExecs)
		: ExecutionFunction(latentInfo.ExecutionFunction)
		, OutputLink(latentInfo.Linkage)
		, CallbackTarget(latentInfo.CallbackTarget)
		, InExecs(inExecs)
		, OutExecs(outExecs)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if(*InExecs == ERyTickUntilStopInExecs::Start)
		{
			*OutExecs = ERyTickUntilStopOutExecs::Tick;
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		}
		else
		{
			*OutExecs = ERyTickUntilStopOutExecs::Stopped;
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		}
	}
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWorldHelpers::TickUntilStop(const UObject* WorldContextObject, const ERyTickUntilStopInExecs& InExecs, ERyTickUntilStopOutExecs& OutExecs, FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FRyTickUntilStopAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr && InExecs == ERyTickUntilStopInExecs::Start)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FRyTickUntilStopAction(LatentInfo, &InExecs, &OutExecs));
		}
	}
}
