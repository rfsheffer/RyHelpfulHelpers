// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

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
