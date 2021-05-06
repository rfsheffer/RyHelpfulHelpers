// Copyright 2020-2021 Sheffer Online Services.
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
bool URyRuntimeNavigationHelpers::GetCanEverAffectNavigation(UActorComponent* actorComponent)
{
    if(actorComponent)
    {
        return actorComponent->CanEverAffectNavigation();
    }
    return false;
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

static_assert(ERyNavLinkDirection::RightToLeft == static_cast<ERyNavLinkDirection>(ENavLinkDirection::RightToLeft), "Fix NavLinkDirection enum misalignment!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeNavigationHelpers::SetSmartLinkData(ANavLinkProxy* navLinkProxy, const FVector& relativeStart,
    const FVector& relativeEnd, ERyNavLinkDirection direction)
{
    if(!navLinkProxy || !navLinkProxy->GetSmartLinkComp())
    {
        return;
    }

    navLinkProxy->GetSmartLinkComp()->SetLinkData(relativeStart, relativeEnd, static_cast<ENavLinkDirection::Type>(direction));

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(navLinkProxy->GetWorld());
    if (NavSys)
    {
        NavSys->UpdateActorInNavOctree(*navLinkProxy);
    }
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
