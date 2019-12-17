// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyEditorPrivatePCH.h"
#include "RyEditorLevelHelpers.h"
#include "Kismet2/ComponentEditorUtils.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
URyEditorLevelHelpers::URyEditorLevelHelpers(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UActorComponent* URyEditorLevelHelpers::CreateComponentForActor(AActor *owner, TSubclassOf<UActorComponent> newComponentClass, 
                                                                USceneComponent *attachComponent /*= nullptr*/)
{
    if(!owner || !newComponentClass)
        return nullptr;

    // No template, so create a wholly new component
    owner->Modify();

    // Create an appropriate name for the new component
    FString ComponentTypeName = FBlueprintEditorUtils::GetClassNameWithoutSuffix(newComponentClass);

    // Strip off 'Component' if the class ends with that.  It just looks better in the UI.
    FString SuffixToStrip(TEXT("Component"));
    if(ComponentTypeName.EndsWith(SuffixToStrip))
    {
        ComponentTypeName = ComponentTypeName.Left(ComponentTypeName.Len() - SuffixToStrip.Len());
    }

    // Try to create a name without any numerical suffix first
    int32 Counter = 1;
    FString ComponentInstanceName = ComponentTypeName;
    while(!FComponentEditorUtils::IsComponentNameAvailable(ComponentInstanceName, owner))
    {
        // Assign the lowest possible numerical suffix
        ComponentInstanceName = FString::Printf(TEXT("%s%d"), *ComponentTypeName, Counter++);
    }
    FName NewComponentName = *ComponentInstanceName;

    // Get the set of owned components that exists prior to instancing the new component.
    TInlineComponentArray<UActorComponent*> PreInstanceComponents;
    owner->GetComponents(PreInstanceComponents);

    // Construct the new component and attach as needed
    UActorComponent* NewInstanceComponent = NewObject<UActorComponent>(owner, newComponentClass, NewComponentName, RF_Transactional);
    if(USceneComponent* NewSceneComponent = Cast<USceneComponent>(NewInstanceComponent))
    {
        if(attachComponent)
        {
            NewSceneComponent->AttachToComponent(attachComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
        }
        else
        {
            USceneComponent* RootComponent = owner->GetRootComponent();
            if(RootComponent)
            {
                NewSceneComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
            }
            else
            {
                owner->SetRootComponent(NewSceneComponent);
            }
        }
    }

    // Add to SerializedComponents array so it gets saved
    owner->AddInstanceComponent(NewInstanceComponent);
    NewInstanceComponent->OnComponentCreated();
    NewInstanceComponent->RegisterComponent();

    // Register any new components that may have been created during construction of the instanced component, but were not explicitly registered.
    TInlineComponentArray<UActorComponent*> PostInstanceComponents;
    owner->GetComponents(PostInstanceComponents);
    for(UActorComponent* ActorComponent : PostInstanceComponents)
    {
        if(!ActorComponent->IsRegistered() && ActorComponent->bAutoRegister && !ActorComponent->IsPendingKill() && !PreInstanceComponents.Contains(ActorComponent))
        {
            ActorComponent->RegisterComponent();
        }
    }

    // Rerun construction scripts
    owner->RerunConstructionScripts();

    return NewInstanceComponent;
}
