// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeNavigationHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to the Unreal navigation system
*/
UCLASS()
class RYRUNTIME_API URyRuntimeNavigationHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /** box extent used ONLY when owning actor doesn't have collision component */
    UFUNCTION(BlueprintPure, Category = "RyRuntime|NavigationHelpers")
	static FVector GetFailsafeExtent(class UNavModifierComponent* NavModifierComponent);

    /** 
     * Set 'SetCanEverAffectNavigation' on an actor component.
     * NOTE: This should only be used in construction scripts. It will probably fail to work as expected at runtime.
    */
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|NavigationHelpers")
    static void SetCanEverAffectNavigation(class UActorComponent* actorComponent, const bool canEverAffectNavigation);

    // Get the start point in world space of a smart link
    UFUNCTION(BlueprintPure, Category = "RyRuntime|NavLinkHelpers")
    static FVector GetStartPoint(class UNavLinkCustomComponent* smartLinkComponent);

    // Get the end point in world space of a smart link
    UFUNCTION(BlueprintPure, Category = "RyRuntime|NavLinkHelpers")
    static FVector GetEndPoint(class UNavLinkCustomComponent* smartLinkComponent);

    // Set whether the smart link in a nav link proxy is relevant
    UFUNCTION(BlueprintCallable, Category = "RyRuntime|NavigationHelpers")
    static void SetSmartLinkIsRelevant(class ANavLinkProxy* navLinkProxy, const bool isRelevant);

    // Get the smart link component of the nav link proxy
    UFUNCTION(BlueprintPure, Category = "RyRuntime|NavLinkHelpers")
    static UNavLinkCustomComponent* GetSmartLinkComponent(class ANavLinkProxy* navLinkProxy);
};
