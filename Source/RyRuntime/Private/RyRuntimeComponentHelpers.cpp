// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeComponentHelpers.h"
#include "RyRuntimeModule.h"

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

	TArray<FVector> distributePoints;
	distributePoints.SetNum(numSplinePoints);
	if(points.Num() != numSplinePoints)
	{
		// Always set the first and last point exactly
		distributePoints[0] = points[0];
		distributePoints[numSplinePoints - 1] = points[points.Num() - 1];

		// Get mid values between points, works for both expansion and contraction
		const float frac = static_cast<float>(points.Num() - 2) / (numSplinePoints - 2);
		for(int32 pointIndex = 1; pointIndex < distributePoints.Num() - 1; ++pointIndex)
		{
			const int32 lowIndex = FMath::FloorToInt(pointIndex * frac);
			const int32 highIndex = FMath::CeilToInt(pointIndex * frac);
			distributePoints[pointIndex] = points[lowIndex] + (points[highIndex] - points[lowIndex]) / 2.0f;
		}
	}

	for(int32 pointIndex = 0; pointIndex < distributePoints.Num(); ++pointIndex)
	{
		splineComponent->SetLocationAtSplinePoint(pointIndex, distributePoints[pointIndex], coordinateSpace, false);
	}

	if(updateSpline)
	{
		splineComponent->UpdateSpline();
	}
}
