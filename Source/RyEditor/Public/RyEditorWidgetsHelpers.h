// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyEditorWidgetsHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to widgets and UMG
*/
UCLASS()
class RYEDITOR_API URyEditorWidgetsHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // Set the categories to show on a Details view
    // NOTE: RebuildDetailsView can be false if you plan on updating the view manually
    UFUNCTION(BlueprintCallable, CallInEditor, Category = "RyEditor|WidgetsHelpers")
    static void SetCategoriesToShow(class UDetailsView* DetailsView, const TArray<FName>& CategoriesToShow);
};
