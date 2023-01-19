// Copyright 2020-2023 Solar Storm Interactive

#include "RyEditorWidgetsHelpers.h"

#include "Editor.h"
#include "Components/DetailsView.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorWidgetsHelpers::SetCategoriesToShow(class UDetailsView* DetailsView, const TArray<FName>& CategoriesToShow)
{
    if(!DetailsView)
        return;

    DetailsView->CategoriesToShow = CategoriesToShow;
}
