// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to editor levels
*/
UCLASS()
class RYEDITOR_API URyEditorHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    URyEditorHelpers(const FObjectInitializer& ObjectInitializer);

	/**
	 * Find all loaded Actors that are selected in the world editor. Only return editor actors if editorActor is true.
	 * This is a replacement for GetSelectedLevelActors which could only return editor actors IF PIE was not active.
	 * This version can return selected actors in PIE as well.
	 * @param editorActors If true will only return editor actors, otherwise will only return game actors
	 * @return	List of found Actors
	 */
	UFUNCTION(BlueprintCallable, Category = "RyEditor|Editor Helpers")
	static TArray<class AActor*> GetSelectedLevelActorsExt(bool editorActors);
};
