// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

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
};
