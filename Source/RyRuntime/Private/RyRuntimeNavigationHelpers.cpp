// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeNavigationHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "NavigationSystem.h"
#include "NavModifierComponent.h"
#include "NavLinkCustomComponent.h"
#include "Navigation/NavLinkProxy.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeNavigationHelpers::GetFailsafeExtent(UNavModifierComponent* NavModifierComponent)
{
    if(NavModifierComponent)
    {
        return NavModifierComponent->FailsafeExtent;
    }
    return FVector::ZeroVector;
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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeNavigationHelpers::GetStartPoint(UNavLinkCustomComponent* smartLinkComponent)
{
    if(smartLinkComponent)
    {
        return smartLinkComponent->GetStartPoint();
    }
    return FVector::ZeroVector;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeNavigationHelpers::GetEndPoint(UNavLinkCustomComponent* smartLinkComponent)
{
    if(smartLinkComponent)
    {
        return smartLinkComponent->GetEndPoint();
    }
    return FVector::ZeroVector;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeNavigationHelpers::SetSmartLinkIsRelevant(ANavLinkProxy* navLinkProxy, const bool isRelevant)
{
    if(navLinkProxy)
    {
        navLinkProxy->bSmartLinkIsRelevant = isRelevant;
        navLinkProxy->GetSmartLinkComp()->SetNavigationRelevancy(isRelevant);

        UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(navLinkProxy->GetWorld());
        if (NavSys)
        {
            NavSys->UpdateActorInNavOctree(*navLinkProxy);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UNavLinkCustomComponent* URyRuntimeNavigationHelpers::GetSmartLinkComponent(ANavLinkProxy* navLinkProxy)
{
    if(navLinkProxy)
    {
        return navLinkProxy->GetSmartLinkComp();
    }
    return nullptr;
}
