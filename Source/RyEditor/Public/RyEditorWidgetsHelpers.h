// Copyright 2020 Sheffer Online Services. All Rights Reserved.

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
