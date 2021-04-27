// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Components/LineBatchComponent.h"
#include "RyLineBatchComponent.generated.h"

UENUM(BlueprintType)
enum class ERyLineBatchDepthPriority : uint8
{
	/** Occlude vs the World scene */
	World,
    /** Occlude vs the Foreground scene */
    Foreground,
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * A component which wraps the internal LineBatchComponent
*/
UCLASS(ClassGroup=(Rendering), meta=(BlueprintSpawnableComponent))
class RYRUNTIME_API URyLineBatchComponent : public ULineBatchComponent
{
	GENERATED_BODY()

public:

	/**
	 * Add a box
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddBox(const FBox& Box,
				          const FTransform& BoxToWorld,
				          const FLinearColor& Color,
				          const float LifeTime = 0.0f,
				          const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add an arrow
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddDirectionalArrow(const FTransform& ArrowToWorld,
							           const FLinearColor& Color,
							           const float Length = 20.0f,
							           const float ArrowSize = 20.0f,
							           const float LifeTime = 0.0f,
							           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a circle
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddCircle(const FVector& Base,
					             const FVector& X,
					             const FVector& Y,
					             const FLinearColor& Color,
					             const float Radius = 10.0f,
					             const int32 NumSides = 20,
					             const float LifeTime = 0.0f,
					             const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a line
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddLine(const FVector& Start,
                           const FVector& End,
                           const FLinearColor& Color,
                           const float Thickness = 0.0f,
                           const float LifeTime = 0.0f,
                           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a point
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddPoint(const FVector& Position,
                            const FLinearColor& Color,
                            const float PointSize = 20.0f,
                            const float LifeTime = 0.0f,
                            const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a solid box
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddSolidBox(const FBox& Box,
					           const FTransform& BoxToWorld,
					           const FLinearColor& Color,
					           const float LifeTime = 0.0f,
					           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);
	
	/**
	 * Add a mesh
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddMesh(const TArray<FVector>& Verts,
				           const TArray<int32>& Indices,
				           const FLinearColor& Color,
				           const float LifeTime = 0.0f,
				           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/** Clear all batched lines, points and meshes */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void ClearLineBatches();
};
