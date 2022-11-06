// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorActorHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to Editor Actors
*/
UCLASS()
class RYEDITOR_API URyEditorActorHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// Sets the actor to be in a garbage collection cluster for its level.
	/// This can only be set before runtime so is an editor only function call.
	UFUNCTION(BlueprintCallable, Category="RyEditor|ActorHelpers")
	static void SetActorCanBeInCluster(class AActor* theActor, bool can);
};
