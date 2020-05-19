// Copyright 2020 Sheffer Online Services. All Rights Reserved.

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
