﻿// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineComponent.h"

#include "RyRuntimeComponentHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
* Static Helper functions related to components in general. Adding extra functionality to components that exist with the
* Unreal Engine.
*/
UCLASS()
class RYRUNTIME_API URyRuntimeComponentHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// Takes an array of points and applies them to the spline by distributing them evenly across the spline.
	// The spline component should be pre-populated.
	// If there are less points than splineComponent points, the points will be interpolated to fill the gap.
	// If there are more points than splineComponent points, you will loose resolution with the spline because decimation will occur.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers")
    static void DistributePointsToSpline(class USplineComponent* splineComponent,
    								     const TArray<FVector>& points,
    								     ESplineCoordinateSpace::Type coordinateSpace,
    								     bool updateSpline = true);

	// Copy the collision properties from one primitive component to another.
	// This does not copy custom profile overrides!
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Primitives")
	static void CopyCollisionProperties(class UPrimitiveComponent* sourceMesh, class UPrimitiveComponent* destMesh);

	// Copy the materials for source mesh to dest mesh.
	// This iterates source meshes materials and assigns them to dest mesh.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Primitives")
	static void CopyMaterials(class UPrimitiveComponent* sourceMesh, class UPrimitiveComponent* destMesh);

	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Primitives")
	static void InvalidateLightingCacheDetailed(UActorComponent* component, bool invalidateBuildEnqueuedLighting, bool translationOnly);

	/**
	 * Query the world and get all components of a specific class
	 * WARNING: This is slow! Don't use this in a tick unless you account for the expense overall.
	 * @param world The world to query
	 * @param componentClass The specific class to look for, use None for "all"
	 * @param componentsOut The found components
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Query")
	static void GetWorldComponentsByClass(UWorld* world, TSubclassOf<UActorComponent> componentClass, TArray<UActorComponent*>& componentsOut);

	/**
	*	Add a force to all rigid bodies below at location.
	*   This is like a 'thruster'. Good for adding a burst over some (non zero) time. Should be called every frame for the duration of the force.
	*
	*	@param	Force		 Force vector to apply. Magnitude indicates strength of force.
	*	@param Location		 Location to apply force, in world space.
	*	@param	BoneName	 If a SkeletalMeshComponent, name of body to apply force to. 'None' indicates root body.
	*	@param bIncludeSelf  If false, Force is only applied to bodies below but not given bone name.
	*/
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|SkeletalMesh")
	static void AddForceAtLocationToAllBodiesBelow(class USkeletalMeshComponent* skelMesh, FVector Force, FVector Location, FName BoneName = NAME_None, bool bIncludeSelf = true);

	/**
	 * @brief Gets the current array of bone transforms from the poseable mesh
	 * @param poseableMesh The Mesh to get transforms of
	 * @param transformsOut The transforms to get
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|PoseableMesh")
	static void GetPoseableMeshTransforms(class UPoseableMeshComponent* poseableMesh, TArray<FTransform>& transformsOut);

	/**
	 * @brief Sets the current array of bone transforms from the poseable mesh
	 * If this function fails with a return of False, check that the number of transforms you pass in is the same as the
	 * number of transforms returned from GetPoseableMeshTransforms / The number of bones of the reference skeleton.
	 * @param poseableMesh The Mesh to get transforms of
	 * @param transformsIn The transforms to set
	 * @param markDirty If true the transforms will be marked as dirty to refresh the mesh rendering
	 * @return True if setting was possible. False if the passed in transforms are greater than the number of expected.
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|PoseableMesh")
	static bool SetPoseableMeshTransforms(class UPoseableMeshComponent* poseableMesh, const TArray<FTransform>& transformsIn, bool markDirty = true);

	/**
	 * @brief 
	 * @param poseableMesh The mesh to mark as dirty
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|PoseableMesh")
	static void MarkPoseableMeshTransformsDirty(class UPoseableMeshComponent* poseableMesh);

	/**
	 * @brief Calculates a components local bounds. This is a calculation unlike GetComponentBounds which uses a cached value. Note this for performance reasons.
	 * @param Component The component
	 * @param Origin The local origin
	 * @param BoxExtent The local extent
	 * @param SphereRadius The sphere radius of the component
	 */
	UFUNCTION(BlueprintPure, Category="RyRuntime|ComponentHelpers|Collision")
	static void CalculateComponentLocalBounds(const USceneComponent* Component, FVector& Origin, FVector& BoxExtent, float& SphereRadius);
};
