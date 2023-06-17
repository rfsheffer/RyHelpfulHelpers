// Copyright 2020-2023 Solar Storm Interactive


#include "RyRuntimeComponentHelpers.h"
#include "RyRuntimeModule.h"
#include "Runtime/Launch/Resources/Version.h"
//#include "Runtime/Core/Public/Logging/MessageLog.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsAsset.h"
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "Runtime/Engine/Classes/Components/PoseableMeshComponent.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::DistributePointsToSpline(USplineComponent* splineComponent,
														  const TArray<FVector>& points,
														  ESplineCoordinateSpace::Type coordinateSpace,
														  bool updateSpline)
{
	if(!splineComponent)
	{
		UE_LOG(LogRyRuntime, Warning, TEXT("DistributePointsToSpline called with null splineComponent!"));
		return;
	}

	const int32 numSplinePoints = splineComponent->GetNumberOfSplinePoints();
	if(numSplinePoints < 2)
	{
		UE_LOG(LogRyRuntime, Warning, TEXT("DistributePointsToSpline called with splineComponent with invalid number of points. An initialized with points (>=2) spline is required!"));
		return;
	}

	if(points.Num() < 2)
	{
		UE_LOG(LogRyRuntime, Warning, TEXT("DistributePointsToSpline called with not enough points... Requires (>=2)"));
		return;
	}

	if(points.Num() != numSplinePoints)
	{
		TArray<FVector> distributePoints;
		distributePoints.SetNum(numSplinePoints);
		
		// Always set the first and last point exactly
		distributePoints[0] = points[0];
		distributePoints[numSplinePoints - 1] = points[points.Num() - 1];

		// Get mid values between points, works for both expansion and contraction
		if(distributePoints.Num() > 2)
		{
			if(points.Num() < numSplinePoints)
			{
				for(int32 pointIndex = 1; pointIndex < distributePoints.Num() - 1; ++pointIndex)
				{
					const float pointFrac = (static_cast<float>(pointIndex) / distributePoints.Num()) * (points.Num() - 1);
					const int32 lowIndex = FMath::FloorToInt(pointFrac);
					const int32 highIndex = FMath::CeilToInt(pointFrac);

					distributePoints[pointIndex] = points[lowIndex] + (points[highIndex] - points[lowIndex]) * (pointFrac - lowIndex);
				}
			}
			else
			{
				const float frac = static_cast<float>(points.Num()) / numSplinePoints;
				for(int32 pointIndex = 1; pointIndex < distributePoints.Num() - 1; ++pointIndex)
				{
					const int32 lowIndex = FMath::FloorToInt(pointIndex * frac);
					const int32 highIndex = FMath::CeilToInt(pointIndex * frac);
					distributePoints[pointIndex] = points[lowIndex] + (points[highIndex] - points[lowIndex]) / 2.0f;
				}
			}
		}

		for(int32 pointIndex = 0; pointIndex < distributePoints.Num(); ++pointIndex)
		{
			splineComponent->SetLocationAtSplinePoint(pointIndex, distributePoints[pointIndex], coordinateSpace, false);
		}
	}
	else
	{
		// Number Aligned arrays, just assign the points directly
		for(int32 pointIndex = 0; pointIndex < points.Num(); ++pointIndex)
		{
			splineComponent->SetLocationAtSplinePoint(pointIndex, points[pointIndex], coordinateSpace, false);
		}
	}

	if(updateSpline)
	{
		splineComponent->UpdateSpline();
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::CopyCollisionProperties(UPrimitiveComponent* sourceMesh, UPrimitiveComponent* destMesh)
{
	if(!sourceMesh || !destMesh)
	{
		return;
	}

	destMesh->SetGenerateOverlapEvents(sourceMesh->GetGenerateOverlapEvents());
	destMesh->SetCollisionProfileName(sourceMesh->GetCollisionProfileName());
	destMesh->SetCollisionEnabled(sourceMesh->GetCollisionEnabled());
	destMesh->SetCanEverAffectNavigation(sourceMesh->CanEverAffectNavigation());
	// TODO: This would be desirable, but crashes in my tests...
	// if(destMesh->BodyInstance.IsValidBodyInstance() && sourceMesh->BodyInstance.IsValidBodyInstance())
	// {
	// 	destMesh->BodyInstance.CopyBodyInstancePropertiesFrom(&sourceMesh->BodyInstance);
	// }
#if WITH_EDITOR
	if(destMesh->CanModify())
	{
		destMesh->Modify();
	}
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::CopyMaterials(UPrimitiveComponent* sourceMesh, UPrimitiveComponent* destMesh)
{
	if(!sourceMesh || !destMesh)
	{
		return;
	}

	for(int32 matIndex = 0; matIndex < sourceMesh->GetNumMaterials() && matIndex < destMesh->GetNumMaterials(); ++matIndex)
	{
		destMesh->SetMaterial(matIndex, sourceMesh->GetMaterial(matIndex));
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::InvalidateLightingCacheDetailed(UActorComponent* component, bool invalidateBuildEnqueuedLighting, bool translationOnly)
{
	if(component)
	{
		component->InvalidateLightingCacheDetailed(invalidateBuildEnqueuedLighting, translationOnly);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::GetWorldComponentsByClass(UWorld* world, TSubclassOf<UActorComponent> componentClass, TArray<UActorComponent*>& componentsOut)
{
	componentsOut.Reset();
	
	if(!world)
	{
		return;
	}

	if(!componentClass)
	{
		componentClass = UActorComponent::StaticClass();
	}
	
#if ENGINE_MAJOR_VERSION >= 5
	EInternalObjectFlags flags = EInternalObjectFlags::Garbage;
#else
	EInternalObjectFlags flags = EInternalObjectFlags::PendingKill;
#endif
	
	for (TObjectIterator<UActorComponent> objIt(RF_ClassDefaultObject, true, flags); objIt; ++objIt)
	{
		UActorComponent* obj = *objIt;
		if (obj && obj->GetWorld() == world && obj->IsA(componentClass))
		{
			componentsOut.Add(obj);
		}
	}
}

namespace
{
	int32 ForEachBodyBelow(USkeletalMeshComponent* skelMesh, FName BoneName, bool bIncludeSelf, bool bSkipCustomType, TFunctionRef<void(FBodyInstance*)> Func)
	{
		if(!skelMesh)
		{
			return 0;
		}
	
		if (BoneName == NAME_None && bIncludeSelf && !bSkipCustomType)
		{
			for (FBodyInstance* BI : skelMesh->Bodies)	//we want all bodies so just iterate the regular array
			{
				Func(BI);
			}

			return skelMesh->Bodies.Num();
		}

		UPhysicsAsset* const PhysicsAsset = skelMesh->GetPhysicsAsset();
		if (!PhysicsAsset)
		{
			return 0;
		}

		USkeletalMesh* skeletalMeshAsset = nullptr;
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
		skeletalMeshAsset = skelMesh->GetSkeletalMeshAsset();
#else
		skeletalMeshAsset = skelMesh->SkeletalMesh;
#endif

		if(!skeletalMeshAsset)
		{
			return 0;
		}

		// if physics state is invalid - i.e. collision is disabled - or it does not have a valid bodies, this will crash right away
		if (!skelMesh->IsPhysicsStateCreated() || !skelMesh->bHasValidBodies)
		{
			//FMessageLog("PIE").Warning(LOCTEXT("InvalidBodies", "Invalid Bodies : Make sure collision is enabled or root bone has body in PhysicsAsset."));
			return 0;
		}

		TArray<int32> BodyIndices;
		BodyIndices.Reserve(skelMesh->Bodies.Num());
		PhysicsAsset->GetBodyIndicesBelow(BodyIndices, BoneName, skeletalMeshAsset, bIncludeSelf);

		int32 NumBodiesFound = 0;
		for (int32 BodyIdx : BodyIndices)
		{
			FBodyInstance* BI = skelMesh->Bodies[BodyIdx];
			if (bSkipCustomType)
			{
				if (UBodySetup* PhysAssetBodySetup = PhysicsAsset->SkeletalBodySetups[BodyIdx])
				{
					if (PhysAssetBodySetup->PhysicsType != EPhysicsType::PhysType_Default)
					{
						continue;
					}
				}
			}

			++NumBodiesFound;
			Func(BI);
		}

		return NumBodiesFound;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::AddForceAtLocationToAllBodiesBelow(USkeletalMeshComponent* skelMesh, FVector Force,
	FVector Location, FName BoneName, bool bIncludeSelf)
{
	ForEachBodyBelow(skelMesh, BoneName, bIncludeSelf, /*bSkipCustomPhysics=*/false, [Force, Location](FBodyInstance* BI)
		{
			BI->AddForceAtPosition(Force, Location);
		});
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::GetPoseableMeshTransforms(UPoseableMeshComponent* poseableMesh, TArray<FTransform>& transformsOut)
{
	if(poseableMesh)
	{
		transformsOut.Reset(poseableMesh->BoneSpaceTransforms.Num());
		transformsOut = poseableMesh->BoneSpaceTransforms;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeComponentHelpers::SetPoseableMeshTransforms(UPoseableMeshComponent* poseableMesh, const TArray<FTransform>& transformsIn, bool markDirty)
{
	if(poseableMesh)
	{
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
		USkeletalMesh* skelMesh = Cast<class USkeletalMesh>(poseableMesh->GetSkinnedAsset());
#else
		USkeletalMesh* skelMesh = poseableMesh->SkeletalMesh;
#endif
		if(skelMesh)
		{
			// Important check because otherwise it will crash
			if(skelMesh->GetRefSkeleton().GetNum() != transformsIn.Num())
			{
				UE_LOG(LogRyRuntime, Warning, TEXT("SetPoseableMeshTransforms failed - Reference skeleton doesn't have the same number of bones as transformsIn!"));
				return false;
			}
		
			poseableMesh->BoneSpaceTransforms = transformsIn;
			if(markDirty)
			{
				poseableMesh->MarkRefreshTransformDirty();
			}

			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeComponentHelpers::MarkPoseableMeshTransformsDirty(UPoseableMeshComponent* poseableMesh)
{
	if(poseableMesh)
	{
		poseableMesh->MarkRefreshTransformDirty();
	}
}
