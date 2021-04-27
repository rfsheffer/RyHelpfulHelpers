// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "Components/RyLineBatchComponent.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddBox(const FBox& Box,
											 const FTransform& BoxToWorld,
											 const FLinearColor& Color,
											 const float LifeTime,
											 const ERyLineBatchDepthPriority DepthPriority)
{
	const float tmp = DefaultLifeTime;
	DefaultLifeTime = LifeTime;
	DrawBox(Box, BoxToWorld.ToMatrixWithScale(), Color.ToFColor(true), static_cast<uint8>(DepthPriority));
	DefaultLifeTime = tmp;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddDirectionalArrow(const FTransform& ArrowToWorld,
														  const FLinearColor& Color,
														  const float Length,
														  const float ArrowSize,
														  const float LifeTime,
														  const ERyLineBatchDepthPriority DepthPriority)
{
	const float tmp = DefaultLifeTime;
	DefaultLifeTime = LifeTime;
	DrawDirectionalArrow(ArrowToWorld.ToMatrixWithScale(), Color.ToFColor(true), 
	Length, ArrowSize, static_cast<uint8>(DepthPriority));
	DefaultLifeTime = tmp;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddCircle(const FVector& Base,
												    const FVector& X,
												    const FVector& Y,
												    const FLinearColor& Color,
												    const float Radius,
												    const int32 NumSides,
												    const float LifeTime,
													const ERyLineBatchDepthPriority DepthPriority)
{
	const float tmp = DefaultLifeTime;
	DefaultLifeTime = LifeTime;
	DrawCircle(Base, X, Y, Color.ToFColor(true), Radius, NumSides, static_cast<uint8>(DepthPriority));
	DefaultLifeTime = tmp;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddLine(const FVector& Start,
											  const FVector& End,
											  const FLinearColor& Color,
											  const float Thickness,
											  const float LifeTime,
											  const ERyLineBatchDepthPriority DepthPriority)
{
	DrawLine(Start, End, Color.ToFColor(true), static_cast<uint8>(DepthPriority), Thickness, LifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddPoint(const FVector& Position,
										       const FLinearColor& Color,
										       const float PointSize,
										       const float LifeTime,
										       const ERyLineBatchDepthPriority DepthPriority)
{
	DrawPoint(Position, Color.ToFColor(true), PointSize, static_cast<uint8>(DepthPriority), LifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddSolidBox(const FBox& Box,
												  const FTransform& BoxToWorld,
												  const FLinearColor& Color,
												  const float LifeTime,
												  const ERyLineBatchDepthPriority DepthPriority)
{
	DrawSolidBox(Box, BoxToWorld, Color.ToFColor(true), static_cast<uint8>(DepthPriority), LifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddMesh(const TArray<FVector>& Verts,
											  const TArray<int32>& Indices,
											  const FLinearColor& Color,
											  const float LifeTime,
											  const ERyLineBatchDepthPriority DepthPriority)
{
	DrawMesh(Verts, Indices, Color.ToFColor(true), static_cast<uint8>(DepthPriority), LifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::ClearLineBatches()
{
	Flush();
}
