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
 * This can be used to add debug lines (shapes, other stuff) which persist with the lifetime of an actor, not the world.
 * This can be useful for creating and clearing sets of debug lines.
*/
UCLASS(ClassGroup=(Rendering), meta=(BlueprintSpawnableComponent))
class RYRUNTIME_API URyLineBatchComponent : public ULineBatchComponent
{
	GENERATED_BODY()

public:

	/**
	 * Add a line
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddLine(const FVector& Start,
                           const FVector& End,
                           const FLinearColor Color = FLinearColor::White,
                           const float LifeTime = -1.0f,
                           const float Thickness = 1.0f,
                           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a point
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddPoint(const FVector& Position,
                            const FLinearColor Color = FLinearColor::White,
                            const float PointSize = 20.0f,
                            const float LifeTime = -1.0f,
                            const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a solid box
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddSolidBox(const FBox& Box,
					           const FTransform& BoxToWorld,
					           const FLinearColor Color = FLinearColor::White,
					           const float LifeTime = -1.0f,
					           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);
	
	/**
	 * Add a mesh
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddMesh(const TArray<FVector>& Verts,
				           const TArray<int32>& Indices,
				           const FLinearColor Color = FLinearColor::White,
				           const float LifeTime = -1.0f,
				           const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a coordinate system visualizer
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
    void AddCoordinateSystem(const FVector& AxisLoc,
    						 const FRotator& AxisRot,
    						 const float Scale=1.f,
    						 const float LifeTime=-1.0f,
    						 const float Thickness = 1.0f,
    						 const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	
	/**
	 * Add a sphere
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddSphere(const FVector& Center,
				   const float Radius=100.f,
				   int32 Segments=12,
				   const FLinearColor LineColor = FLinearColor::White,
				   const float LifeTime=-1.0f,
				   const float Thickness = 1.0f,
				   const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a cylinder
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddCylinder(const FVector& Start,
					 const FVector& End,
					 const float Radius=100.f,
					 int32 Segments=12,
					 const FLinearColor LineColor = FLinearColor::White,
					 const float LifeTime=-1.0f,
					 const float Thickness = 1.f,
                     const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/** 
	 * Add a cone 
	 * Angles are specified in degrees
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddCone(const FVector Origin,
					      const FVector Direction,
					      const float Length=100.f,
					      const float AngleWidth=45.f,
					      const float AngleHeight=45.f,
					      int32 NumSides = 12,
					      const FLinearColor LineColor = FLinearColor::White,
					      const float LifeTime=-1.0f,
					      const float Thickness = 1.0f,
                          const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a capsule
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddCapsule(const FVector& Center,
					const float HalfHeight,
					const float Radius,
					const FRotator& Rotation,
					const FLinearColor LineColor = FLinearColor::White,
					const float LifeTime=-1.0f,
					const float Thickness = 1.0f,
                    const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a plane.
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddPlane(const FPlane& PlaneCoordinates,
			      const FVector Location,
			      const float Size,
			      const FLinearColor PlaneColor = FLinearColor::White,
			      const float LifeTime=-1.0f,
                  const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a frustum.
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddFrustum(const FTransform& FrustumTransform,
				    const FLinearColor FrustumColor = FLinearColor::White,
				    const float LifeTime=-1.0f,
				    const float Thickness = 1.0f,
                    const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a camera shape.
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void AddCamera(const ACameraActor* CameraActor,
				   const FLinearColor CameraColor = FLinearColor::White,
				   const float LifeTime=-1.0f,
                   const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Add a box
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
    void AddBox(const FVector& Center,
    			const FVector& Extent,
    			const FLinearColor LineColor = FLinearColor::White,
    			const FRotator Rotation = FRotator::ZeroRotator,
    			const float LifeTime=-1.0f,
    			const float Thickness = 1.0f,
    			const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);

	/**
	 * Draw directional arrow, pointing from LineStart to LineEnd.
	 * @param LifeTime - The lifetime of the line. -1 means infitite. 
	 */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
    void AddArrow(const FVector& LineStart,
    	          const FVector& LineEnd,
    	          const float ArrowSize,
    	          const FLinearColor LineColor = FLinearColor::White,
    	          const float LifeTime=-1.0f,
    	          const float Thickness = 1.0f,
    	          const ERyLineBatchDepthPriority DepthPriority = ERyLineBatchDepthPriority::World);
	
	/** Clear all batched lines, points and meshes */
	UFUNCTION(BlueprintCallable, Category="RyLineBatch")
	void ClearLineBatches();

private:
	static float SanitizeLifetime(const float lifeTime);
	void RyDrawCircle(const FVector& Base, const FVector& X, const FVector& Y, const FColor& Color, float Radius, int32 NumSides, float LifeTime, uint8 DepthPriority, float Thickness);
	void RyDrawHalfCircle(const FVector& Base, const FVector& X, const FVector& Y, const FColor& Color, float Radius, int32 NumSides, float LifeTime, uint8 DepthPriority, float Thickness);
	void RyDrawDebugDirectionalArrow(FVector const& LineStart, FVector const& LineEnd,
      float ArrowSize, FColor const& Color, float LifeTime, uint8 DepthPriority, float Thickness);
	void RyDrawDebugBox(FVector const& Center, FVector const& Box, const FQuat& Rotation, FColor const& Color, float LifeTime, uint8 DepthPriority, float Thickness);
	void RyDrawDebugBox(FVector const& Center, FVector const& Box, FColor const& Color, float LifeTime, uint8 DepthPriority, float Thickness);
};
