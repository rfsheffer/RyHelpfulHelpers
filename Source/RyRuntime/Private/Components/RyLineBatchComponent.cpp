// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "Components/RyLineBatchComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddLine(const FVector& Start,
											  const FVector& End,
											  const FLinearColor Color,
											  const float LifeTime,
											  const float Thickness,
											  const ERyLineBatchDepthPriority DepthPriority)
{
	DrawLine(Start, End, Color.ToFColor(true), static_cast<uint8>(DepthPriority), Thickness, SanitizeLifetime(LifeTime));
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddPoint(const FVector& Position,
										       const FLinearColor Color,
										       const float PointSize,
										       const float LifeTime,
										       const ERyLineBatchDepthPriority DepthPriority)
{
	DrawPoint(Position, Color.ToFColor(true), PointSize, static_cast<uint8>(DepthPriority), SanitizeLifetime(LifeTime));
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddSolidBox(const FBox& Box,
												  const FTransform& BoxToWorld,
												  const FLinearColor Color,
												  const float LifeTime,
												  const ERyLineBatchDepthPriority DepthPriority)
{
	DrawSolidBox(Box, BoxToWorld, Color.ToFColor(true), static_cast<uint8>(DepthPriority), SanitizeLifetime(LifeTime));
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddMesh(const TArray<FVector>& Verts,
											  const TArray<int32>& Indices,
											  const FLinearColor Color,
											  const float LifeTime,
											  const ERyLineBatchDepthPriority DepthPriority)
{
	DrawMesh(Verts, Indices, Color.ToFColor(true), static_cast<uint8>(DepthPriority), SanitizeLifetime(LifeTime));
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddCoordinateSystem(const FVector& AxisLoc,
										        const FRotator& AxisRot,
										        const float Scale,
										        const float LifeTime,
										        const float Thickness,
										        const ERyLineBatchDepthPriority DepthPriority)
{
	FRotationMatrix R(AxisRot);
	FVector const X = R.GetScaledAxis( EAxis::X );
	FVector const Y = R.GetScaledAxis( EAxis::Y );
	FVector const Z = R.GetScaledAxis( EAxis::Z );

	DrawLine(AxisLoc, AxisLoc + X*Scale, FColor::Red, static_cast<uint8>(DepthPriority), Thickness, SanitizeLifetime(LifeTime));
	DrawLine(AxisLoc, AxisLoc + Y*Scale, FColor::Green, static_cast<uint8>(DepthPriority), Thickness, SanitizeLifetime(LifeTime));
	DrawLine(AxisLoc, AxisLoc + Z*Scale, FColor::Blue, static_cast<uint8>(DepthPriority), Thickness, SanitizeLifetime(LifeTime));
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddSphere(const FVector& Center, const float Radius, int32 Segments,
	const FLinearColor LineColor, const float LifeTime, const float Thickness, const ERyLineBatchDepthPriority DepthPriority)
{
	// Need at least 4 segments
	Segments = FMath::Max(Segments, 4);
	float LineLifeTime = SanitizeLifetime(LifeTime);

	FVector Vertex1, Vertex2, Vertex3, Vertex4;
	const float AngleInc = 2.f * PI / float(Segments);
	int32 NumSegmentsY = Segments;
	float Latitude = AngleInc;
	int32 NumSegmentsX;
	float Longitude;
	float SinY1 = 0.0f, CosY1 = 1.0f, SinY2, CosY2;
	float SinX, CosX;

	TArray<FBatchedLine> Lines;
	Lines.Empty(NumSegmentsY * Segments * 2);
	while (NumSegmentsY--)
	{
		SinY2 = FMath::Sin(Latitude);
		CosY2 = FMath::Cos(Latitude);

		Vertex1 = FVector(SinY1, 0.0f, CosY1) * Radius + Center;
		Vertex3 = FVector(SinY2, 0.0f, CosY2) * Radius + Center;
		Longitude = AngleInc;

		NumSegmentsX = Segments;
		while (NumSegmentsX--)
		{
			SinX = FMath::Sin(Longitude);
			CosX = FMath::Cos(Longitude);

			Vertex2 = FVector((CosX * SinY1), (SinX * SinY1), CosY1) * Radius + Center;
			Vertex4 = FVector((CosX * SinY2), (SinX * SinY2), CosY2) * Radius + Center;

			Lines.Add(FBatchedLine(Vertex1, Vertex2, LineColor.ToFColor(true), LineLifeTime, Thickness, static_cast<uint8>(DepthPriority)));
			Lines.Add(FBatchedLine(Vertex1, Vertex3, LineColor.ToFColor(true), LineLifeTime, Thickness, static_cast<uint8>(DepthPriority)));

			Vertex1 = Vertex2;
			Vertex3 = Vertex4;
			Longitude += AngleInc;
		}
		SinY1 = SinY2;
		CosY1 = CosY2;
		Latitude += AngleInc;
	}
	DrawLines(Lines);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddCylinder(const FVector& Start, const FVector& End, const float Radius,
	int32 Segments, const FLinearColor LineColor, const float LifeTime, const float Thickness,
	const ERyLineBatchDepthPriority DepthPriority)
{
	// Need at least 4 segments
	Segments = FMath::Max(Segments, 4);

	// Rotate a point around axis to form cylinder segments
	FVector Segment;
	FVector P1, P2, P3, P4;
	const float AngleInc = 360.f / Segments;
	float Angle = AngleInc;

	// Default for Axis is up
	FVector Axis = (End - Start).GetSafeNormal();
	if( Axis.IsZero() )
	{
		Axis = FVector(0.f, 0.f, 1.f);
	}

	FVector Perpendicular;
	FVector Dummy;

	Axis.FindBestAxisVectors(Perpendicular, Dummy);
		
	Segment = Perpendicular.RotateAngleAxis(0, Axis) * Radius;
	P1 = Segment + Start;
	P3 = Segment + End;

	// this means foreground lines can't be persistent 
	const float LineLifeTime = SanitizeLifetime(LifeTime);
	while( Segments-- )
	{
		Segment = Perpendicular.RotateAngleAxis(Angle, Axis) * Radius;
		P2 = Segment + Start;
		P4 = Segment + End;

		DrawLine(P2, P4, LineColor.ToFColor(true), static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);
		DrawLine(P1, P2, LineColor.ToFColor(true), static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);
		DrawLine(P3, P4, LineColor.ToFColor(true), static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);

		P1 = P2;
		P3 = P4;
		Angle += AngleInc;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddCone(const FVector Origin, const FVector Direction, const float Length,
	const float AngleWidth, const float AngleHeight, int32 NumSides, const FLinearColor LineColor,
	const float LifeTime, const float Thickness, const ERyLineBatchDepthPriority DepthPriority)
{
	// Need at least 4 sides
	NumSides = FMath::Max(NumSides, 4);

	const float Angle1 = FMath::Clamp<float>(AngleHeight, (float)KINDA_SMALL_NUMBER, (float)(PI - KINDA_SMALL_NUMBER));
	const float Angle2 = FMath::Clamp<float>(AngleWidth, (float)KINDA_SMALL_NUMBER, (float)(PI - KINDA_SMALL_NUMBER));

	const float SinX_2 = FMath::Sin(0.5f * Angle1);
	const float SinY_2 = FMath::Sin(0.5f * Angle2);

	const float SinSqX_2 = SinX_2 * SinX_2;
	const float SinSqY_2 = SinY_2 * SinY_2;

	//const float TanX_2 = FMath::Tan(0.5f * Angle1);
	//const float TanY_2 = FMath::Tan(0.5f * Angle2);

	TArray<FVector> ConeVerts;
	ConeVerts.AddUninitialized(NumSides);

	for(int32 i = 0; i < NumSides; i++)
	{
		const float Fraction	= (float)i/(float)(NumSides);
		const float Thi			= 2.f * PI * Fraction;
		const float Phi			= FMath::Atan2(FMath::Sin(Thi)*SinY_2, FMath::Cos(Thi)*SinX_2);
		const float SinPhi		= FMath::Sin(Phi);
		const float CosPhi		= FMath::Cos(Phi);
		const float SinSqPhi	= SinPhi*SinPhi;
		const float CosSqPhi	= CosPhi*CosPhi;

		const float RSq			= SinSqX_2*SinSqY_2 / (SinSqX_2*SinSqPhi + SinSqY_2*CosSqPhi);
		const float R			= FMath::Sqrt(RSq);
		const float Sqr			= FMath::Sqrt(1-RSq);
		const float Alpha		= R*CosPhi;
		const float Beta		= R*SinPhi;

		ConeVerts[i].X = (1 - 2*RSq);
		ConeVerts[i].Y = 2 * Sqr * Alpha;
		ConeVerts[i].Z = 2 * Sqr * Beta;
	}

	// Calculate transform for cone.
	FVector YAxis, ZAxis;
	FVector DirectionNorm = Direction.GetSafeNormal();
	DirectionNorm.FindBestAxisVectors(YAxis, ZAxis);
	const FMatrix ConeToWorld = FScaleMatrix(FVector(Length)) * FMatrix(DirectionNorm, YAxis, ZAxis, Origin);

	// this means foreground lines can't be persistent 
	float const LineLifeTime = SanitizeLifetime(LifeTime);

	TArray<FBatchedLine> Lines;
	Lines.Empty(NumSides);

	FVector CurrentPoint, PrevPoint, FirstPoint;
	for(int32 i = 0; i < NumSides; i++)
	{
		CurrentPoint = ConeToWorld.TransformPosition(ConeVerts[i]);
		Lines.Add(FBatchedLine(ConeToWorld.GetOrigin(), CurrentPoint, LineColor.ToFColor(true), LineLifeTime, Thickness, static_cast<uint8>(DepthPriority)));

		// PrevPoint must be defined to draw junctions
		if( i > 0 )
		{
			Lines.Add(FBatchedLine(PrevPoint, CurrentPoint, LineColor.ToFColor(true), LineLifeTime, Thickness, static_cast<uint8>(DepthPriority)));
		}
		else
		{
			FirstPoint = CurrentPoint;
		}

		PrevPoint = CurrentPoint;
	}
	// Connect last junction to first
	Lines.Add(FBatchedLine(CurrentPoint, FirstPoint, LineColor.ToFColor(true), LineLifeTime, Thickness, static_cast<uint8>(DepthPriority)));

	DrawLines(Lines);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::RyDrawCircle(const FVector& Base, const FVector& X, const FVector& Y, const FColor& Color, float Radius, int32 NumSides, float LifeTime, uint8 DepthPriority, float Thickness)
{
	const float	AngleDelta = 2.0f * PI / NumSides;
	FVector	LastVertex = Base + X * Radius;

	for(int32 SideIndex = 0;SideIndex < NumSides;SideIndex++)
	{
		const FVector Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		DrawLine(LastVertex, Vertex, Color, DepthPriority, Thickness, LifeTime);
		LastVertex = Vertex;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::RyDrawHalfCircle(const FVector& Base, const FVector& X, const FVector& Y, const FColor& Color, float Radius, int32 NumSides, float LifeTime, uint8 DepthPriority, float Thickness)
{
	float	AngleDelta = 2.0f * (float)PI / ((float)NumSides);
	FVector	LastVertex = Base + X * Radius;

	for(int32 SideIndex = 0; SideIndex < (NumSides/2); SideIndex++)
	{
		FVector	Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		DrawLine(LastVertex, Vertex, Color, DepthPriority, Thickness, LifeTime);
		LastVertex = Vertex;
	}	
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddCapsule(const FVector& Center, const float HalfHeight, const float Radius,
	const FRotator& Rotation, const FLinearColor LineColor, const float LifeTime, const float Thickness,
	const ERyLineBatchDepthPriority DepthPriority)
{
	const float LineLifeTime = SanitizeLifetime(LifeTime);
	const int32 DrawCollisionSides = 16;

	FVector Origin = Center;
	FMatrix Axes = FQuatRotationTranslationMatrix(Rotation.Quaternion(), FVector::ZeroVector);
	FVector XAxis = Axes.GetScaledAxis( EAxis::X );
	FVector YAxis = Axes.GetScaledAxis( EAxis::Y );
	FVector ZAxis = Axes.GetScaledAxis( EAxis::Z ); 

	// Draw top and bottom circles
	float HalfAxis = FMath::Max<float>(HalfHeight - Radius, 1.f);
	FVector TopEnd = Origin + HalfAxis*ZAxis;
	FVector BottomEnd = Origin - HalfAxis*ZAxis;

	FColor lineColorIn = LineColor.ToFColor(true);

	RyDrawCircle(TopEnd, XAxis, YAxis, lineColorIn, Radius, DrawCollisionSides, LineLifeTime, static_cast<uint8>(DepthPriority), Thickness);
	RyDrawCircle(BottomEnd, XAxis, YAxis, lineColorIn, Radius, DrawCollisionSides, LineLifeTime, static_cast<uint8>(DepthPriority), Thickness);

	// Draw domed caps
	RyDrawHalfCircle(TopEnd, YAxis, ZAxis, lineColorIn, Radius, DrawCollisionSides, LineLifeTime, static_cast<uint8>(DepthPriority), Thickness);
	RyDrawHalfCircle(TopEnd, XAxis, ZAxis, lineColorIn, Radius, DrawCollisionSides, LineLifeTime, static_cast<uint8>(DepthPriority), Thickness);

	FVector NegZAxis = -ZAxis;

	RyDrawHalfCircle(BottomEnd, YAxis, NegZAxis, lineColorIn, Radius, DrawCollisionSides, LineLifeTime, static_cast<uint8>(DepthPriority), Thickness);
	RyDrawHalfCircle(BottomEnd, XAxis, NegZAxis, lineColorIn, Radius, DrawCollisionSides, LineLifeTime, static_cast<uint8>(DepthPriority), Thickness);

	// Draw connected lines
	DrawLine(TopEnd + Radius*XAxis, BottomEnd + Radius*XAxis, lineColorIn, static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);
	DrawLine(TopEnd - Radius*XAxis, BottomEnd - Radius*XAxis, lineColorIn, static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);
	DrawLine(TopEnd + Radius*YAxis, BottomEnd + Radius*YAxis, lineColorIn, static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);
	DrawLine(TopEnd - Radius*YAxis, BottomEnd - Radius*YAxis, lineColorIn, static_cast<uint8>(DepthPriority), Thickness, LineLifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddPlane(const FPlane& PlaneCoordinates, const FVector Location, const float Size,
	const FLinearColor PlaneColor, const float LifeTime, const ERyLineBatchDepthPriority DepthPriority)
{
	FVector const ClosestPtOnPlane = Location - PlaneCoordinates.PlaneDot(Location) * PlaneCoordinates;

	FVector U, V;
	PlaneCoordinates.FindBestAxisVectors(U, V);
	U *= Size;
	V *= Size;

	TArray<FVector> Verts;
	Verts.AddUninitialized(4);
	Verts[0] = ClosestPtOnPlane + U + V;
	Verts[1] = ClosestPtOnPlane - U + V;
	Verts[2] = ClosestPtOnPlane + U - V;
	Verts[3] = ClosestPtOnPlane - U - V;

	TArray<int32> Indices;
	Indices.AddUninitialized(6);
	Indices[0] = 0; Indices[1] = 2; Indices[2] = 1;
	Indices[3] = 1; Indices[4] = 2; Indices[5] = 3;

	FColor colorIn;
#if ENGINE_MAJOR_VERSION >= 5
	colorIn = PlaneColor.QuantizeRound();
#else
	colorIn = PlaneColor.Quantize();
#endif
	
	// plane quad
	DrawMesh(Verts, Indices, colorIn, static_cast<uint8>(DepthPriority), SanitizeLifetime(LifeTime));

	// arrow indicating normal
	RyDrawDebugDirectionalArrow(ClosestPtOnPlane, ClosestPtOnPlane + PlaneCoordinates * 16.f, 8.f, FColor::White, SanitizeLifetime(LifeTime), static_cast<uint8>(DepthPriority), 0.0f);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddFrustum(const FTransform& FrustumTransform, const FLinearColor FrustumColor,
	const float LifeTime, const float Thickness, const ERyLineBatchDepthPriority DepthPriority)
{
	FVector Vertices[2][2][2];
	for(uint32 Z = 0;Z < 2;Z++)
	{
		for(uint32 Y = 0;Y < 2;Y++)
		{
			for(uint32 X = 0;X < 2;X++)
			{
				FVector4 UnprojectedVertex = FrustumTransform.TransformFVector4(
                    FVector4(
                    (X ? -1.0f : 1.0f),
                    (Y ? -1.0f : 1.0f),
                    (Z ?  0.0f : 1.0f),
                    1.0f
                    )
                    );
				Vertices[X][Y][Z] = FVector(UnprojectedVertex) / UnprojectedVertex.W;
			}
		}
	}

	const float inLifeTime = SanitizeLifetime(LifeTime);

	DrawLine(Vertices[0][0][0], Vertices[0][0][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[1][0][0], Vertices[1][0][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[0][1][0], Vertices[0][1][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[1][1][0], Vertices[1][1][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);

	DrawLine(Vertices[0][0][0], Vertices[0][1][0], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[1][0][0], Vertices[1][1][0], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[0][0][1], Vertices[0][1][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[1][0][1], Vertices[1][1][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);

	DrawLine(Vertices[0][0][0], Vertices[1][0][0], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[0][1][0], Vertices[1][1][0], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[0][0][1], Vertices[1][0][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
	DrawLine(Vertices[0][1][1], Vertices[1][1][1], FrustumColor, static_cast<uint8>(DepthPriority), Thickness, inLifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddCamera(const ACameraActor* CameraActor, const FLinearColor CameraColor,
					const float LifeTime, const ERyLineBatchDepthPriority DepthPriority)
{
	if(!CameraActor)
	{
		return;
	}
	
	static float BaseScale = 4.f;
	static FVector BaseProportions(2.f, 1.f, 1.5f);

	AddCoordinateSystem(CameraActor->GetActorLocation(),
                                                    CameraActor->GetActorRotation(),
                                                    BaseScale,
                                                    LifeTime,
                                                    0.0f,
                                                    DepthPriority);
	
	FVector Extents = BaseProportions * BaseScale;
	RyDrawDebugBox(CameraActor->GetActorLocation(), Extents, CameraActor->GetActorRotation().Quaternion(), CameraColor.ToFColor(true), LifeTime, static_cast<uint8>(DepthPriority), 0.0f);

	// draw "lens" portion
	FRotationTranslationMatrix Axes(CameraActor->GetActorRotation(), CameraActor->GetActorLocation());
	FVector XAxis = Axes.GetScaledAxis( EAxis::X );
	FVector YAxis = Axes.GetScaledAxis( EAxis::Y );
	FVector ZAxis = Axes.GetScaledAxis( EAxis::Z ); 

	FVector LensPoint = CameraActor->GetActorLocation() + XAxis * Extents.X;
	float LensSize = BaseProportions.Z * BaseScale;
	float HalfLensSize = LensSize * FMath::Tan(FMath::DegreesToRadians(CameraActor->GetCameraComponent()->FieldOfView * 0.5f));
	FVector Corners[4] = 
	{
		LensPoint + XAxis * LensSize + (YAxis * HalfLensSize) + (ZAxis * HalfLensSize),
		LensPoint + XAxis * LensSize + (YAxis * HalfLensSize) - (ZAxis * HalfLensSize),
		LensPoint + XAxis * LensSize - (YAxis * HalfLensSize) - (ZAxis * HalfLensSize),
		LensPoint + XAxis * LensSize - (YAxis * HalfLensSize) + (ZAxis * HalfLensSize),
	};

	DrawLine(LensPoint, Corners[0], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
	DrawLine(LensPoint, Corners[1], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
	DrawLine(LensPoint, Corners[2], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
	DrawLine(LensPoint, Corners[3], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);

	DrawLine(Corners[0], Corners[1], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
	DrawLine(Corners[1], Corners[2], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
	DrawLine(Corners[2], Corners[3], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
	DrawLine(Corners[3], Corners[0], CameraColor, static_cast<uint8>(DepthPriority), 0.0f, LifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddBox(const FVector& Center, const FVector& Extent, const FLinearColor LineColor,
	const FRotator Rotation, const float LifeTime, const float Thickness, const ERyLineBatchDepthPriority DepthPriority)
{
	if (Rotation == FRotator::ZeroRotator)
	{
		RyDrawDebugBox(Center, Extent, LineColor.ToFColor(true), LifeTime, SDPG_World, Thickness);
	}
	else
	{
		RyDrawDebugBox(Center, Extent, Rotation.Quaternion(), LineColor.ToFColor(true), LifeTime, SDPG_World, Thickness);
	}
}

void URyLineBatchComponent::RyDrawDebugDirectionalArrow(FVector const& LineStart, FVector const& LineEnd,
	  float ArrowSize, FColor const& Color, float LifeTime, uint8 DepthPriority, float Thickness)
{
	if (ArrowSize <= 0)
	{
		ArrowSize = 10.f;
	}

	DrawLine(LineStart, LineEnd, Color, DepthPriority, Thickness, LifeTime);

	FVector Dir = (LineEnd-LineStart);
	Dir.Normalize();
	FVector Up(0, 0, 1);
	FVector Right = Dir ^ Up;
	if (!Right.IsNormalized())
	{
		Dir.FindBestAxisVectors(Up, Right);
	}
	FVector Origin = FVector::ZeroVector;
	FMatrix TM;
	// get matrix with dir/right/up
	TM.SetAxes(&Dir, &Right, &Up, &Origin);

	// since dir is x direction, my arrow will be pointing +y, -x and -y, -x
	const float ArrowSqrt = FMath::Sqrt(ArrowSize);
	DrawLine(LineEnd, LineEnd + TM.TransformPosition(FVector(-ArrowSqrt, ArrowSqrt, 0)), Color, DepthPriority, Thickness, LifeTime);
	DrawLine(LineEnd, LineEnd + TM.TransformPosition(FVector(-ArrowSqrt, -ArrowSqrt, 0)), Color, DepthPriority, Thickness, LifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::AddArrow(const FVector& LineStart, const FVector& LineEnd, const float ArrowSize,
	const FLinearColor LineColor, const float LifeTime, const float Thickness,
	const ERyLineBatchDepthPriority DepthPriority)
{
	RyDrawDebugDirectionalArrow(LineStart, LineEnd, ArrowSize, LineColor.ToFColor(true), SanitizeLifetime(LifeTime), static_cast<uint8>(DepthPriority), Thickness);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::RyDrawDebugBox(FVector const& Center, FVector const& Box, FColor const& Color, float LifeTime, uint8 DepthPriority, float Thickness)
{
	const float LineLifeTime = SanitizeLifetime(LifeTime);

	DrawLine(Center + FVector( Box.X,  Box.Y,  Box.Z), Center + FVector( Box.X, -Box.Y, Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector( Box.X, -Box.Y,  Box.Z), Center + FVector(-Box.X, -Box.Y, Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector(-Box.X, -Box.Y,  Box.Z), Center + FVector(-Box.X,  Box.Y, Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector(-Box.X,  Box.Y,  Box.Z), Center + FVector( Box.X,  Box.Y, Box.Z), Color, DepthPriority, Thickness, LineLifeTime);

	DrawLine(Center + FVector( Box.X,  Box.Y, -Box.Z), Center + FVector( Box.X, -Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector( Box.X, -Box.Y, -Box.Z), Center + FVector(-Box.X, -Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector(-Box.X, -Box.Y, -Box.Z), Center + FVector(-Box.X,  Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector(-Box.X,  Box.Y, -Box.Z), Center + FVector( Box.X,  Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);

	DrawLine(Center + FVector( Box.X,  Box.Y,  Box.Z), Center + FVector( Box.X,  Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector( Box.X, -Box.Y,  Box.Z), Center + FVector( Box.X, -Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector(-Box.X, -Box.Y,  Box.Z), Center + FVector(-Box.X, -Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
	DrawLine(Center + FVector(-Box.X,  Box.Y,  Box.Z), Center + FVector(-Box.X,  Box.Y, -Box.Z), Color, DepthPriority, Thickness, LineLifeTime);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::RyDrawDebugBox(FVector const& Center, FVector const& Box, const FQuat& Rotation, FColor const& Color, float LifeTime, uint8 DepthPriority, float Thickness)
{
	float const LineLifeTime = SanitizeLifetime(LifeTime);
	TArray<struct FBatchedLine> Lines;

	FTransform const Transform(Rotation);
	FVector Start = Transform.TransformPosition(FVector( Box.X,  Box.Y,  Box.Z));
	FVector End = Transform.TransformPosition(FVector( Box.X, -Box.Y, Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector( Box.X, -Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector(-Box.X, -Box.Y, Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector(-Box.X, -Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector(-Box.X,  Box.Y, Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector(-Box.X,  Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector( Box.X,  Box.Y, Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector( Box.X,  Box.Y, -Box.Z));
	End = Transform.TransformPosition(FVector( Box.X, -Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector( Box.X, -Box.Y, -Box.Z));
	End = Transform.TransformPosition(FVector(-Box.X, -Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector(-Box.X, -Box.Y, -Box.Z));
	End = Transform.TransformPosition(FVector(-Box.X,  Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector(-Box.X,  Box.Y, -Box.Z));
	End = Transform.TransformPosition(FVector( Box.X,  Box.Y, -Box.Z));
	new(Lines )FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector( Box.X,  Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector( Box.X,  Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector( Box.X, -Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector( Box.X, -Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector(-Box.X, -Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector(-Box.X, -Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	Start = Transform.TransformPosition(FVector(-Box.X,  Box.Y,  Box.Z));
	End = Transform.TransformPosition(FVector(-Box.X,  Box.Y, -Box.Z));
	new(Lines) FBatchedLine(Center + Start, Center + End, Color, LineLifeTime, Thickness, DepthPriority);

	DrawLines(Lines);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyLineBatchComponent::ClearLineBatches()
{
	Flush();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
float URyLineBatchComponent::SanitizeLifetime(const float lifeTime)
{
	if(lifeTime < 0.0f)
	{
		return -1.0f;
	}

	return lifeTime;
}
