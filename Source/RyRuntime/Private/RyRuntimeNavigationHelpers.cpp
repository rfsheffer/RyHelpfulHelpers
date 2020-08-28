// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeNavigationHelpers.h"
#include "RyRuntimeModule.h"
#include "Kismet/GameplayStatics.h"
#include "NavModifierComponent.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeNavigationHelpers::GetFailsafeExtent(UNavModifierComponent* NavModifierComponent)
{
    if(!NavModifierComponent)
        return FVector::ZeroVector;

    return NavModifierComponent->FailsafeExtent;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeNavigationHelpers::SetCanEverAffectNavigation(class UActorComponent* actorComponent, const bool canEverAffectNavigation)
{
    if(actorComponent)
    {
        actorComponent->SetCanEverAffectNavigation(canEverAffectNavigation);
    }
}
