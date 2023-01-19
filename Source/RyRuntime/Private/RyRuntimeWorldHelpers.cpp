// Copyright 2020-2023 Solar Storm Interactive

#include "RyRuntimeWorldHelpers.h"
#include "LatentActions.h"

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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UWorld* URyRuntimeWorldHelpers::GetEngineWorld(UObject* WorldContextObject, ERyWorldType specificType)
{
	check(GEngine);
	bool shouldSearch = true;
	if(WorldContextObject)
	{
		shouldSearch = false;
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if(World)
		{
			return World;
		}
	}

	if(specificType != ERyWorldType::None)
	{
		shouldSearch = false;

		for(const FWorldContext& context : GEngine->GetWorldContexts())
		{
			if(static_cast<EWorldType::Type>(specificType) == context.WorldType)
			{
				return context.World();
			}
		}
	}
	
	if(!shouldSearch)
	{
		return nullptr;
	}

	UWorld* bestWorldOut = nullptr;
	for(const FWorldContext& context : GEngine->GetWorldContexts())
	{
		if(context.WorldType == EWorldType::Game || context.WorldType == EWorldType::PIE)
		{
			return context.World();
		}

		if(context.WorldType == EWorldType::Editor)
		{
			bestWorldOut = context.World();
		}
	}

	return bestWorldOut;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeWorldHelpers::GetEngineWorldContexts(TArray<FRyWorldContext>& worldsOut)
{
	worldsOut.Empty(GEngine->GetWorldContexts().Num());
	check(GEngine);
	for(const FWorldContext& context : GEngine->GetWorldContexts())
	{
		FRyWorldContext worldContext;
		worldContext.World = context.World();
		worldContext.WorldType = static_cast<ERyWorldType>(context.WorldType.GetValue());
		worldContext.ContextHandle = context.ContextHandle;
		worldContext.TravelURL = context.TravelURL;
		worldContext.TravelType = context.TravelType;
		worldContext.OwningGameInstance = context.OwningGameInstance;
		worldContext.PIEPrefix = context.PIEPrefix;
		worldContext.RunAsDedicated = context.RunAsDedicated;
		worldContext.bWaitingOnOnlineSubsystem = context.bWaitingOnOnlineSubsystem;
		worldContext.CustomDescription = context.CustomDescription;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FTimerHandle URyRuntimeWorldHelpers::SetTimerByEventForWorld(UObject* WorldContextObject,
	FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay, float InitialStartDelayVariance)
{
	FTimerHandle Handle;
	if (Delegate.IsBound())
	{
		const UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if(World)
		{
			InitialStartDelay += FMath::RandRange(-InitialStartDelayVariance, InitialStartDelayVariance);
			if (Time <= 0.f || (Time + InitialStartDelay) < 0.f)
			{
				FString ObjectName = GetNameSafe(Delegate.GetUObject());
				FString FunctionName = Delegate.GetFunctionName().ToString(); 
				FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s %s SetTimer passed a negative or zero time. The associated timer may fail to be created/fire! If using InitialStartDelayVariance, be sure it is smaller than (Time + InitialStartDelay)."), *ObjectName, *FunctionName), ELogVerbosity::Warning);
			}

			FTimerManager& TimerManager = World->GetTimerManager();
			Handle = TimerManager.K2_FindDynamicTimerHandle(Delegate);
			TimerManager.SetTimer(Handle, Delegate, Time, bLooping, (Time + InitialStartDelay));
		}
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Warning, 
			TEXT("SetTimer passed a bad function (%s) or object (%s)"),
			*Delegate.GetFunctionName().ToString(), *GetNameSafe(Delegate.GetUObject()));
	}

	return Handle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FTimerHandle URyRuntimeWorldHelpers::SetTimerByFunctionForWorld(UObject* WorldContextObject, UObject* Object,
	FString FunctionName, float Time, bool bLooping, float InitialStartDelay, float InitialStartDelayVariance)
{
	FName const FunctionFName(*FunctionName);

	if (Object)
	{
		UFunction* const Func = Object->FindFunction(FunctionFName);
		if ( Func && (Func->ParmsSize > 0) )
		{
			// User passed in a valid function, but one that takes parameters
			// FTimerDynamicDelegate expects zero parameters and will choke on execution if it tries
			// to execute a mismatched function
			UE_LOG(LogBlueprintUserMessages, Warning, TEXT("SetTimer passed a function (%s) that expects parameters."), *FunctionName);
			return FTimerHandle();
		}
	}

	FTimerDynamicDelegate Delegate;
	Delegate.BindUFunction(Object, FunctionFName);
	return SetTimerByEventForWorld(WorldContextObject, Delegate, Time, bLooping, InitialStartDelay, InitialStartDelayVariance);
}
