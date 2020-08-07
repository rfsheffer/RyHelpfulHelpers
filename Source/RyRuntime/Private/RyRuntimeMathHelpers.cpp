// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeMathHelpers.h"
#include "RyRuntimeModule.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeMathHelpers::FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation, 
                                                                   const float EdgePercent, FVector2D& OutScreenPosition, 
                                                                   float& OutRotationAngleDegrees, bool &bIsOnScreen)
{
	bIsOnScreen = false;
	OutRotationAngleDegrees = 0.f;

	if (!GEngine) return;

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	if (!World) return;

	APlayerController* PlayerController = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);

	if (!PlayerController) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());

	if (!PlayerCharacter) return;

	FVector Forward = PlayerCharacter->GetActorForwardVector();
	FVector Offset = (InLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();

	float DotProduct = FVector::DotProduct(Forward, Offset);
	bool bLocationIsBehindCamera = (DotProduct < 0);

	if (bLocationIsBehindCamera)
	{
		// For behind the camera situation, we cheat a little to put the
		// marker at the bottom of the screen so that it moves smoothly
		// as you turn around. Could stand some refinement, but results
		// are decent enough for most purposes.

		FVector DiffVector = InLocation - PlayerCharacter->GetActorLocation();
		FVector Inverted = DiffVector * -1.f;
		FVector NewInLocation = PlayerCharacter->GetActorLocation() * Inverted;

		NewInLocation.Z -= 5000;

		PlayerController->ProjectWorldLocationToScreen(NewInLocation, OutScreenPosition);
		OutScreenPosition.Y = (EdgePercent * ViewportCenter.X) * 2.f;
		OutScreenPosition.X = -ViewportCenter.X - OutScreenPosition.X;
	}

	PlayerController->ProjectWorldLocationToScreen(InLocation, OutScreenPosition);//*ScreenPosition);

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

	float Cos = cosf(AngleRadians);
	float Sin = -sinf(AngleRadians);

	OutScreenPosition = FVector2D(ViewportCenter.X + (Sin * 180.f), ViewportCenter.Y + Cos * 180.f);

	float m = Cos / Sin;

	FVector2D ScreenBounds = ViewportCenter * EdgePercent;

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
    float LocalAngle = 0, LocalRadian = 0, LocalXRadius = 0, LocalYRadius = 0, LocalTangent = 0, LocalY = 0, LocalX = 0;
    FVector2D LocalPointRelativeToCenter;

    // Angle / Radian
    LocalAngle = UKismetMathLibrary::NormalizeAxis(TheAngle + 90.0f);
    LocalRadian = UKismetMathLibrary::DegreesToRadians(LocalAngle);
    if(LocalRadian < 0.0f)
    {
        LocalRadian += (PI * 2.0f);
    }
    LocalTangent = FMath::Tan(LocalRadian);

    // X / Y Radius
    LocalXRadius = TheSize.X / 2.0f;
    LocalYRadius = TheSize.Y / 2.0f;

    LocalY = LocalXRadius * LocalTangent;

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
        LocalX = LocalYRadius / LocalTangent;
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
