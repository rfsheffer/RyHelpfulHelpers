// Copyright 2020-2023 Solar Storm Interactive


#include "RyRuntimeMathHelpers.h"

#include "RyRuntimeModule.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Public/UnrealClient.h"

static_assert(ERyUnit::Unspecified == static_cast<ERyUnit>(EUnit::Unspecified), "ERyUnit isn't aligned to EUnit!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeMathHelpers::ApplyGaussianSpread(const FVector& vecDir, const float spreadDegrees, float alpha,
	const float biasMin, const float biasMax)
{
	const FVector vecSpread = FVector(FMath::DegreesToRadians(spreadDegrees / 2.0f));
	const FVector vecRight = vecDir.Rotation().RotateVector(FVector::RightVector);
	const FVector vecUp = vecDir.Rotation().RotateVector(FVector::UpVector);

	// get circular gaussian spread
	float x, y, z;

	if (alpha > 1.0f)
	{
		alpha = 1.0f;
	}
	else if (alpha < 0.0f)
	{
		alpha = 0.0f;
	}

	// 1.0 gaussian, 0.0 is flat, -1.0 is inverse gaussian
	const float shotBias = ((biasMax - biasMin) * alpha) + biasMin;
	const float flatness = (FMath::Abs(shotBias) * 0.5f);

	do
	{
		x = FMath::FRandRange(-1.0f, 1.0f) * flatness + FMath::FRandRange(-1.0f, 1.0f) * (1.0f - flatness);
		y = FMath::FRandRange(-1.0f, 1.0f) * flatness + FMath::FRandRange(-1.0f, 1.0f) * (1.0f - flatness);
		if (shotBias < 0.0f)
		{
			x = (x >= 0.0f) ? 1.0f - x : -1.0f - x;
			y = (y >= 0.0f) ? 1.0f - y : -1.0f - y;
		}
		z = x * x + y * y;
	} while (z > 1.0f);

	return vecDir + x * vecSpread.Y * vecRight + y * vecSpread.Z * vecUp;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeMathHelpers::VInterpNormalRotationTo(const FVector& current, const FVector& target,
													   const float deltaTime, const float rotationSpeedDegrees)
{
	return FMath::VInterpNormalRotationTo(current, target, deltaTime, rotationSpeedDegrees);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
float URyRuntimeMathHelpers::ShortestRotationPath(const float startRotation, const float endRotation)
{
	const float clampA = FRotator::ClampAxis(startRotation);
	const float clampB = FRotator::ClampAxis(endRotation);

	float d1, d2;
	if(clampA > clampB)
	{
		d1 = clampA - clampB;
		d2 = (360.0f + clampB) - clampA;
	}
	else
	{
		d1 = (360.0f + clampA) - clampB;
		d2 = clampB - clampA;
	}

	return d1 > d2 ? d2 : -d1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeMathHelpers::RotationInterpolate(const float inCurrentRotation, const float inDestinationRotation,
												const float deltaTime, const float speed, float& newRotation,
												bool& atTarget, const float checkTolerance)
{
	const float increment = GetRotationIncrement(inCurrentRotation, inDestinationRotation, deltaTime, speed, checkTolerance);
	if(increment == 0.0f)
	{
		atTarget = true;
		newRotation = FRotator::ClampAxis(inDestinationRotation);
		return;
	}

	atTarget = false;
	newRotation = FRotator::ClampAxis(inCurrentRotation + increment);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
float URyRuntimeMathHelpers::GetRotationIncrement(const float inCurrentRotation, const float inDestinationRotation,
											      const float deltaTime, const float speed, const float checkTolerance)
{
	if(RotationsEqual(inCurrentRotation, inDestinationRotation, checkTolerance))
	{
		return 0.0f;
	}

	const float increment = FMath::Abs(speed * deltaTime);
	const float rotationPath = ShortestRotationPath(inCurrentRotation, inDestinationRotation);
	if(FMath::Abs(rotationPath) > increment)
	{
		return increment * FMath::Sign(rotationPath);
	}

	return rotationPath;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeMathHelpers::FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation, 
                                                                   const float EdgePercent, FVector2D& OutScreenPosition, 
                                                                   float& OutRotationAngleDegrees, bool &bIsOnScreen, const int32 playerIndex)
{
	bIsOnScreen = false;
	OutRotationAngleDegrees = 0.f;

	if (!GEngine)
	{
		return;
	}

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, playerIndex) : nullptr);
	if (!PlayerController)
	{
		return;
	}

	ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	const FVector Forward = PlayerCharacter->GetActorForwardVector();
	const FVector Offset = (InLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();

	const float DotProduct = FVector::DotProduct(Forward, Offset);
	const bool bLocationIsBehindCamera = (DotProduct < 0);
	if (bLocationIsBehindCamera)
	{
		// For behind the camera situation, we cheat a little to put the
		// marker at the bottom of the screen so that it moves smoothly
		// as you turn around. Could stand some refinement, but results
		// are decent enough for most purposes.

		const FVector DiffVector = InLocation - PlayerCharacter->GetActorLocation();
		const FVector Inverted = DiffVector * -1.f;
		FVector NewInLocation = PlayerCharacter->GetActorLocation() * Inverted;

		NewInLocation.Z -= 5000;

		PlayerController->ProjectWorldLocationToScreen(NewInLocation, OutScreenPosition);
		OutScreenPosition.Y = (EdgePercent * ViewportCenter.X) * 2.f;
		OutScreenPosition.X = -ViewportCenter.X - OutScreenPosition.X;
	}

	PlayerController->ProjectWorldLocationToScreen(InLocation, OutScreenPosition); // * ScreenPosition);

	// Check to see if it's on screen. If it is, ProjectWorldLocationToScreen is all we need, return it.	
	if (OutScreenPosition.X >= 0.f && OutScreenPosition.X <= ViewportSize.X
		&& OutScreenPosition.Y >= 0.f && OutScreenPosition.Y <= ViewportSize.Y)
	{

		bIsOnScreen = true;
		return;
	}

	OutScreenPosition -= ViewportCenter;

	float AngleRadians = FMath::Atan2(OutScreenPosition.Y, OutScreenPosition.X);
	AngleRadians -= FMath::DegreesToRadians(90.f);

	OutRotationAngleDegrees = FMath::RadiansToDegrees(AngleRadians) + 180.f;

	const float Cos = cosf(AngleRadians);
	const float Sin = -sinf(AngleRadians);

	OutScreenPosition = FVector2D(ViewportCenter.X + (Sin * 180.f), ViewportCenter.Y + Cos * 180.f);

	const float m = Cos / Sin;

	const FVector2D ScreenBounds = ViewportCenter * EdgePercent;

	if (Cos > 0)
	{
		OutScreenPosition = FVector2D(ScreenBounds.Y / m, ScreenBounds.Y);
	}
	else
	{
		OutScreenPosition = FVector2D(-ScreenBounds.Y / m, -ScreenBounds.Y);
	}

	if (OutScreenPosition.X > ScreenBounds.X)
	{
		OutScreenPosition = FVector2D(ScreenBounds.X, ScreenBounds.X*m);
	}
	else if (OutScreenPosition.X < -ScreenBounds.X)
	{
		OutScreenPosition = FVector2D(-ScreenBounds.X, -ScreenBounds.X*m);
	}

	OutScreenPosition += ViewportCenter;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector2D URyRuntimeMathHelpers::FindEdgeOf2DSquare(const FVector2D &TheSize, const float TheAngle)
{
    // Angle / Radian
    const float LocalAngle = UKismetMathLibrary::NormalizeAxis(TheAngle + 90.0f);
    float LocalRadian = UKismetMathLibrary::DegreesToRadians(LocalAngle);
    if(LocalRadian < 0.0f)
    {
        LocalRadian += (PI * 2.0f);
    }
    const float LocalTangent = FMath::Tan(LocalRadian);

    // X / Y Radius
    const float LocalXRadius = TheSize.X / 2.0f;
    const float LocalYRadius = TheSize.Y / 2.0f;

    const float LocalY = LocalXRadius * LocalTangent;

	FVector2D LocalPointRelativeToCenter;
    if(FMath::Abs(LocalY) <= LocalYRadius)
    {
        if((LocalRadian < (PI / 2.0f)) || (LocalRadian > (PI + (PI / 2.0f))))
        {
            // Right
            LocalPointRelativeToCenter.Set(LocalXRadius, LocalY);
        }
        else
        {
            // Left
            LocalPointRelativeToCenter.Set(LocalXRadius * -1.0f, LocalY * -1.0f);
        }
    }
    else
    {
        const float LocalX = LocalYRadius / LocalTangent;
        if(LocalRadian < PI)
        {
            // Bottom
            LocalPointRelativeToCenter.Set(LocalX, LocalYRadius);
        }
        else
        {
            // Top
            LocalPointRelativeToCenter.Set(LocalX * -1.0f, LocalYRadius * -1.0f);
        }
    }

    // Return
    return (TheSize / 2.0f) + LocalPointRelativeToCenter;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector2D URyRuntimeMathHelpers::ResizeBoxToFitInScreenSpace(const FVector2D boxSize, const FVector2D screenSize, const FVector2D fracOfScreen)
{
	FVector2D boxOut;
	const FVector2D maxSize(screenSize.X * fracOfScreen.X, screenSize.Y * fracOfScreen.Y);
	
	// Constraint to X, scaling Y if needed
	if(maxSize.X < boxSize.X)
	{
		boxOut.X = maxSize.X;
		boxOut.Y = boxSize.Y * (maxSize.X / boxSize.X);
	}
	else
	{
		boxOut = boxSize;
	}

	// Constraint to Y, scaling X if needed
	if(maxSize.Y < boxOut.Y)
	{
		boxOut.X *= maxSize.Y / boxOut.Y;
		boxOut.Y = maxSize.Y;
	}

	return boxOut;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeMathHelpers::PositionAndScaleBoxIntoScreenSpace(const FVector2D boxSize, const FVector2D screenSize,
															   const FVector2D fracPosOnScreen, const FVector2D fracSizeOnScreen,
															   const ERyXScreenAnchor anchorX, const ERyYScreenAnchor anchorY, const FVector2D boxAnchorFrac,
															   FVector2D& boxPosOut, FVector2D& boxSizeOut)
{
	boxSizeOut = ResizeBoxToFitInScreenSpace(boxSize, screenSize, fracSizeOnScreen);

	const FVector2D areaOrigin(screenSize.X * fracPosOnScreen.X, screenSize.Y * fracPosOnScreen.Y);
	const FVector2D areaSize(screenSize.X * fracSizeOnScreen.X, screenSize.Y * fracSizeOnScreen.Y);
	
	if(anchorX == ERyXScreenAnchor::LEFT)
	{
		boxPosOut.X = areaOrigin.X - boxSizeOut.X * boxAnchorFrac.X;
	}
	else if(anchorX == ERyXScreenAnchor::MIDDLE)
	{
		boxPosOut.X = areaOrigin.X + (areaSize.X / 2.0f - boxSizeOut.X * boxAnchorFrac.X);
	}
	else if(anchorX == ERyXScreenAnchor::RIGHT)
	{
		boxPosOut.X = areaOrigin.X + (areaSize.X - boxSizeOut.X * boxAnchorFrac.X);
	}
	
	if(anchorY == ERyYScreenAnchor::TOP)
	{
		boxPosOut.Y = areaOrigin.Y - boxSizeOut.Y * boxAnchorFrac.Y;
	}
	else if(anchorY == ERyYScreenAnchor::MIDDLE)
	{
		boxPosOut.Y = areaOrigin.Y + (areaSize.Y / 2.0f - boxSizeOut.Y * boxAnchorFrac.Y);
	}
	else if(anchorY == ERyYScreenAnchor::BOTTOM)
	{
		boxPosOut.Y = areaOrigin.Y + (areaSize.Y - boxSizeOut.Y * boxAnchorFrac.Y);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeMathHelpers::ByteSetBit(uint8& byte, uint8 bit, bool val)
{
	uint8 toSet = val;
	toSet <<= bit;
	uint8 mask = 1;
	mask <<= bit;
	byte &= ~mask;
	byte |= toSet;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeMathHelpers::ByteIsBitSet(uint8 byte, uint8 bit)
{
	return (byte & (1 << bit)) != 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeMathHelpers::IntSetBit(int32& intIn, uint8 bit, bool val)
{
	int32 toSet = val;
	toSet <<= bit;
	int32 mask = 1;
	mask <<= bit;
	intIn &= ~mask;
	intIn |= toSet;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeMathHelpers::IntIsBitSet(int32 intIn, uint8 bit)
{
	return (intIn & (1 << bit)) != 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeMathHelpers::GetPlaneOrigin(const FPlane& plane)
{
	return plane.GetOrigin();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeMathHelpers::GetPlaneNormal(const FPlane& plane)
{
	return plane.GetNormal();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
float URyRuntimeMathHelpers::GetPlaneDistanceToPoint(const FPlane& plane, const FVector& point)
{
	return plane.PlaneDot(point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FPlane URyRuntimeMathHelpers::GetPlaneFlipped(const FPlane& plane)
{
	return plane.Flip();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyRuntimeMathHelpers::GetDirectionToHitMovingTarget(const FVector& sourceLocation,
	const float sourceVelocityMagnitude, const FVector& targetLocation, const FVector& targetVelocity)
{
	// https://www.gamedeveloper.com/programming/shooting-a-moving-target
	
	const float velocityDelta = targetVelocity.SizeSquared() - FMath::Square(sourceVelocityMagnitude);

	const FVector deltaLocation = targetLocation - sourceLocation;
	const float b = 2.f * FVector::DotProduct(targetVelocity, deltaLocation);
	
	const float deltaSquared = deltaLocation.SizeSquared();

	const float discriminant = b * b - 4.f * velocityDelta * deltaSquared; 

	float time;
	if(discriminant >= 0.f)
	{
		time = 2.f * deltaSquared / (FMath::Sqrt(discriminant) - b);
	}
	else
	{
		time = 0.f;
		UE_LOG(LogRyRuntime, Warning, TEXT("GetDirectionToHitMovingTarget failed to find result. Returning target location!"));
	}
    
	return (targetLocation + targetVelocity * time - sourceLocation).GetSafeNormal();
}
