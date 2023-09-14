// Copyright 2020-2023 Solar Storm Interactive

#include "RyEditorActorHelpers.h"
#include "GameFramework/Actor.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorActorHelpers::SetActorCanBeInCluster(AActor* theActor, bool can)
{
	if(!theActor)
	{
		return;
	}

	check(theActor->GetClass());
	
	const FBoolProperty* bCanBeInClusterProp = CastField<FBoolProperty>(theActor->GetClass()->FindPropertyByName(FName(TEXT("bCanBeInCluster"))));
	if(bCanBeInClusterProp)
	{
		void *pPropertyPtr = bCanBeInClusterProp->ContainerPtrToValuePtr<void>(theActor);
		check(pPropertyPtr);
		bCanBeInClusterProp->SetPropertyValue(pPropertyPtr, can);
		theActor->Modify();
	}
}
