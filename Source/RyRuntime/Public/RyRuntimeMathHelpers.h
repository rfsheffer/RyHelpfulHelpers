// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyRuntimeMathHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions for mathematics.
*/
UCLASS(meta=(BlueprintThreadSafe))
class RYRUNTIME_API URyRuntimeMathHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /** Returns the hyperbolic sine of A (expects Radians)*/
	UFUNCTION(BlueprintPure, meta=(DisplayName = "SinH (Radians)", CompactNodeTitle = "SINH"), Category="RyRuntime|Math|Trig")
	static float SinH(float A);

    /** Returns the hyperbolic cosine of A (expects Radians)*/
	UFUNCTION(BlueprintPure, meta=(DisplayName = "CosH (Radians)", CompactNodeTitle = "COSH"), Category="RyRuntime|Math|Trig")
	static float CosH(float A);

    /** 
     Calculates the catenary of X at a specific scaling factor
     The scaling factor may be thought of as the ratio between the horizontal tension on the cable and the weight 
     of the cable per unit length. A lowscaling factor will therefore result in a deeper curve.
    */
    UFUNCTION(BlueprintPure, Category = "Math|Utility")
    static float CalculateCatenary(float X, float scalingFactor);

    /**
	* Converts a world location to screen position for HUD drawing. This differs from the results of FSceneView::WorldToScreen in that it returns a position along the edge of the screen for offscreen locations
	*
	* @param		InLocation	- The world space location to be converted to screen space
	* @param		EdgePercent - How close to the edge of the screen, 1.0 = at edge, 0.0 = at center of screen. .9 or .95 is usually desirable
	* @outparam	OutScreenPosition - the screen coordinates for HUD drawing
	* @outparam	OutRotationAngleDegrees - The angle to rotate a hud element if you want it pointing toward the offscreen indicator, 0° if onscreen
	* @outparam	bIsOnScreen - True if the specified location is in the camera view (may be obstructed)
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext), Category = "RyRuntime|Math|HUD")
	static void FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation, const float EdgePercent, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen);

	UFUNCTION(BlueprintCallable, Category = "RyRuntime|Math|HUD")
	static FVector2D FindEdgeOf2DSquare(const FVector2D &TheSize, const float TheAngle);

    /** Bitwise AND (A & B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise AND", CompactNodeTitle = "&", Keywords = "& and", CommutativeAssociativeBinaryOperator = "true"), Category="RyRuntime|Math|Byte")
	static uint8 And_ByteByte(uint8 A, uint8 B);

	/** Bitwise XOR (A ^ B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise XOR", CompactNodeTitle = "^", Keywords = "^ xor"), Category="RyRuntime|Math|Byte")
	static uint8 Xor_ByteByte(uint8 A, uint8 B);

	/** Bitwise OR (A | B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise OR", CompactNodeTitle = "|", Keywords = "| or", CommutativeAssociativeBinaryOperator = "true"), Category="RyRuntime|Math|Byte")
	static uint8 Or_ByteByte(uint8 A, uint8 B);

	/** Bitwise NOT (~A) */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Bitwise NOT", CompactNodeTitle = "~", Keywords = "~ not"), Category = "RyRuntime|Math|Byte")
	static uint8 Not_Byte(uint8 A);

    /** Bitwise Shift Left << */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise Shift Left", CompactNodeTitle = "<<", Keywords = "<< shift"), Category="RyRuntime|Math|Byte")
	static uint8 ShiftLeft_Byte(uint8 val, int32 shift = 1);

    /** Bitwise Shift Right >> */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise Shift Right", CompactNodeTitle = ">>", Keywords = ">> shift"), Category="RyRuntime|Math|Byte")
	static uint8 ShiftRight_Byte(uint8 val, int32 shift = 1);

    /** Bitwise Shift Left << */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise Shift Left", CompactNodeTitle = "<<", Keywords = "<< shift"), Category="RyRuntime|Math|Integer")
	static int32 ShiftLeft_Int(int32 val, int32 shift = 1);

    /** Bitwise Shift Right >> */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise Shift Right", CompactNodeTitle = ">>", Keywords = ">> shift"), Category="RyRuntime|Math|Integer")
	static int32 ShiftRight_Int(int32 val, int32 shift = 1);

    /** Bitwise Shift Left << */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise Shift Left", CompactNodeTitle = "<<", Keywords = "<< shift"), Category="RyRuntime|Math|Integer64")
	static int64 ShiftLeft_Int64(int64 val, int32 shift = 1);

    /** Bitwise Shift Right >> */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Bitwise Shift Right", CompactNodeTitle = ">>", Keywords = ">> shift"), Category="RyRuntime|Math|Integer64")
	static int64 ShiftRight_Int64(int64 val, int32 shift = 1);
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
float URyRuntimeMathHelpers::SinH(float A)
{
    return sinhf(A);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
float URyRuntimeMathHelpers::CosH(float A)
{
    return coshf(A);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
float URyRuntimeMathHelpers::CalculateCatenary(float X, float scalingFactor)
{
    return scalingFactor * coshf(X / scalingFactor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
uint8 URyRuntimeMathHelpers::And_ByteByte(uint8 A, uint8 B)
{
    return A & B;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
uint8 URyRuntimeMathHelpers::Xor_ByteByte(uint8 A, uint8 B)
{
    return A ^ B;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
uint8 URyRuntimeMathHelpers::Or_ByteByte(uint8 A, uint8 B)
{
    return A | B;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
uint8 URyRuntimeMathHelpers::Not_Byte(uint8 A)
{
    return ~A;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
uint8 URyRuntimeMathHelpers::ShiftLeft_Byte(uint8 val, int32 shift)
{
    return val << shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
uint8 URyRuntimeMathHelpers::ShiftRight_Byte(uint8 val, int32 shift)
{
    return val >> shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
int32 URyRuntimeMathHelpers::ShiftLeft_Int(int32 val, int32 shift)
{
    return val << shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
int32 URyRuntimeMathHelpers::ShiftRight_Int(int32 val, int32 shift)
{
    return val >> shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
int64 URyRuntimeMathHelpers::ShiftLeft_Int64(int64 val, int32 shift)
{
    return val << shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE_DEBUGGABLE
int64 URyRuntimeMathHelpers::ShiftRight_Int64(int64 val, int32 shift)
{
    return val >> shift;
}
