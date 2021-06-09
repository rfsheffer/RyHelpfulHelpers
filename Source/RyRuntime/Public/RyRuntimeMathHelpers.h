// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/UnitConversion.h"

#include "RyRuntimeMathHelpers.generated.h"

// Copy of the Unreal EUnit enum for blueprint unit conversion access
UENUM(BlueprintType)
enum class ERyUnit : uint8
{
	/** Scalar distance/length units */
	Micrometers, Millimeters, Centimeters, Meters, Kilometers,
    Inches, Feet, Yards, Miles,
    Lightyears,

    /** Angular units */
    Degrees, Radians,

    /** Speed units */
    MetersPerSecond, KilometersPerHour, MilesPerHour,

    /** Temperature units */
    Celsius, Farenheit, Kelvin,

    /** Mass units */
    Micrograms, Milligrams, Grams, Kilograms, MetricTons,
    Ounces, Pounds, Stones,

    /** Force units */
    Newtons, PoundsForce, KilogramsForce,

    /** Frequency units */
    Hertz, Kilohertz, Megahertz, Gigahertz, RevolutionsPerMinute,

    /** Data Size units */
    Bytes, Kilobytes, Megabytes, Gigabytes, Terabytes,

    /** Luminous flux units, luminous intensity, illuminance, luminance */
    Lumens UMETA(Hidden),
	Candela UMETA(Hidden),
	Lux UMETA(Hidden),
	CandelaPerMeter2 UMETA(Hidden),

    /** Time units */
    Milliseconds, Seconds, Minutes, Hours, Days, Months, Years,

    /** Pixel density units */
    PixelsPerInch UMETA(Hidden),

    /** Arbitrary multipliers */
    Percentage UMETA(Hidden),	Multiplier UMETA(Hidden),

    /** Symbolic entry, not specifiable on meta data */
    Unspecified  UMETA(Hidden)
};

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
    UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Utility")
    static float CalculateCatenary(float X, float scalingFactor);

	/**
	 * Convert a unit value to another unit value
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Utility")
	static float ConvertUnit(const float value, const ERyUnit from, const ERyUnit to);

	/**
	 * Interpolate a normal vector Current to Target, by interpolating the angle between those vectors with constant step.
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Interpolation", DisplayName = "InterpNormalRotationTo")
	static FVector VInterpNormalRotationTo(const FVector& current, const FVector& target, const float deltaTime, const float rotationSpeedDegrees);

	/**
	 * Takes a startRotation and finds the shortest path to endRotation.
	 * Input rotations are clamped, and output rotation is clamped. [0-360]
	 * Positive output means 'endRotation; is towards a increasing value of 'startRotation'
	 * Negative output means 'endRotation' is towards a decreasing value of 'startRotation'
	 *
	 * @param startRotation - The rotation towards endRotation
	 * @param endRotation - The rotation from startRotation
	 *
	 * @return a new rotation component value
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Rotations")
	static float ShortestRotationPath(const float startRotation, const float endRotation);

	// Do two rotations equal?
	// Clamps both rotations to consistent limits and compares them using an Error Tolerance
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Rotations")
	static bool RotationsEqual(const float rotation1, const float rotation2, const float ErrorTolerance = 1.e-6f);

	/**
	 * Rotate inCurrent towards inTarget at a rate of speed
	 * NOTE: This clamps your rotation to be between [0, 360].
	 *       If calculated newRotation distance to target is less than checkTolerance, newRotation is clamped to inTarget.
	 * @param inCurrentRotation - The current rotation
	 * @param inDestinationRotation - The destination rotation
	 * @param deltaTime - Time since last update (usually deltaSeconds)
	 * @param speed - The speed to rotate towards
	 * @param newRotation - The new rotation
	 * @param atTarget - True if the target was reached
	 * @param checkTolerance - When checking if inCurrent is equal to inTarget, the floating point tolerance to check against.
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Rotations", meta=(AdvancedDisplay = "6"))
	static void RotationInterpolate(const float inCurrentRotation, const float inDestinationRotation, const float deltaTime, const float speed, float& newRotation, bool& atTarget, const float checkTolerance = 1.e-6f);

	/**
	 * Get the next rotation increment from inCurrentRotation to inDestinationRotation
	 * @param inCurrentRotation - The current rotation
	 * @param inDestinationRotation - The destination
	 * @param deltaTime - Time since last update (usually deltaSeconds)
	 * @param speed - The speed to rotate towards the destination
	 * @return The increment towards the destination. If Zero, the destination has been reached
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Rotations", meta=(AdvancedDisplay = "4"))
	static float GetRotationIncrement(const float inCurrentRotation, const float inDestinationRotation, const float deltaTime, const float speed, const float checkTolerance = 1.e-6f);

	/** Convert to Quaternion representation of this Rotator. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToQuat (Rotator)", CompactNodeTitle = "->", ScriptMethod = "Quaternion", Keywords = "cast convert", BlueprintAutocast), Category = "RyRuntime|Math|Conversions")
    static FQuat Rotator_Quat(const FRotator& R);

	// Returns inFloat as a negative value, even if inFloat is already negative.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Float", meta=(DisplayName = "Negate", CompactNodeTitle = "NEG"))
	static float MakeNegative(const float inFloat);

	// Makes inFloat as a negative value, even if inFloat is already negative.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|Math|Float", meta=(DisplayName = "NegateInline", CompactNodeTitle = "NEG"))
	static void MakeNegativeInline(UPARAM(ref) float& inFloat);

	// Same as Abs, returns a positive float value.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Float", meta=(DisplayName = "Positive", CompactNodeTitle = "POS"))
	static float MakePositive(const float inFloat);

	// Same as Abs, makes inFloat a positive float value.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|Math|Float", meta=(DisplayName = "PositiveInline", CompactNodeTitle = "POS"))
    static void MakePositiveInline(UPARAM(ref) float& inFloat);

	// A constant used by the engine denoting an invalid index.
	// Array Find operations return IndexNone if an element could not be found.
	// Most returned indices are IndexNone if invalid.
	// Sometimes IndexNone is passed into functions to show the user doesn't care what the index is.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Index", meta=(DisplayName = "Index None", CompactNodeTitle = "INDEX_NONE"))
	static int32 IndexNone();

    /**
	* Converts a world location to screen position for HUD drawing. This differs from the results of FSceneView::WorldToScreen
	* in that it returns a position along the edge of the screen for offscreen locations
	*
	* @param    WorldContextObject - World context
	* @param    InLocation	- The world space location to be converted to screen space
	* @param	EdgePercent - How close to the edge of the screen, 1.0 = at edge, 0.0 = at center of screen. .9 or .95 is usually desirable
	* @param	OutScreenPosition - the screen coordinates for HUD drawing
	* @param	OutRotationAngleDegrees - The angle to rotate a hud element if you want it pointing toward the offscreen indicator, 0� if onscreen
	* @param	bIsOnScreen - True if the specified location is in the camera view (may be obstructed)
	* @param    playerIndex - The index of the local player
	*/
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext, AdvancedDisplay = "6"), Category = "RyRuntime|Math|HUD")
	static void FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation,
													   const float EdgePercent, FVector2D& OutScreenPosition,
													   float& OutRotationAngleDegrees, bool &bIsOnScreen, const int32 playerIndex = 0);

	// With TheSize of the surface ex (800 x 600) and TheAngle from the center of the surface, what is the point on TheSize where TheAngle would hit?
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

	/** Returns the 2D dot product of two 3d vectors, Z axis excluded. - see http://mathworld.wolfram.com/DotProduct.html */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Dot Product 2D", CompactNodeTitle = "dot2D", ScriptMethod = "Dot2D", ScriptOperator = "|"), Category="RyRuntime|Math|Vector" )
    static float Dot_VectorVector2D(FVector A, FVector B);

	/** Returns true if vector A is equal to vector B (A == B) within a specified error tolerance, ignoring Z */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Equal (Vector) 2D", CompactNodeTitle = "==2D", ScriptMethod = "IsNearEqual2D", Keywords = "== equal"), Category="RyRuntime|Math|Vector")
    static bool EqualEqual_VectorVector2D(FVector A, FVector B, float ErrorTolerance = 1.e-4f);

	/** Returns true if vector A is not equal to vector B (A != B) within a specified error tolerance, ignoring Z */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Not Equal (Vector) 2D", CompactNodeTitle = "!=2D", ScriptMethod = "IsNotNearEqual2D"), Category="RyRuntime|Math|Vector")
    static bool NotEqual_VectorVector2D(FVector A, FVector B, float ErrorTolerance = 1.e-4f);

	/** Rotate the world forward vector by the given rotation, excluding the Z axis */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Get Forward Vector 2D", ScriptMethod = "GetForwardVector2D", Keywords="rotation rotate"), Category="RyRuntime|Math|Vector")
    static FVector GetForwardVector2D(FRotator InRot);

	/** Rotate the world right vector by the given rotation, excluding the Z axis */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Get Right Vector 2D", ScriptMethod = "GetRightVector2D", Keywords="rotation rotate"), Category="RyRuntime|Math|Vector")
    static FVector GetRightVector2D(FRotator InRot);

	/** Rotate the world up vector by the given rotation, excluding the Z axis */
	UFUNCTION(BlueprintPure, meta=(DisplayName = "Get Up Vector 2D", ScriptMethod = "GetUpVector2D", Keywords="rotation rotate"), Category="RyRuntime|Math|Vector")
    static FVector GetUpVector2D(FRotator InRot);

	/** Create a box from points */
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box", meta=(Keywords="construct build"))
	static void MakeBoxFromPoints(const TArray<FVector>& points, FBox& box);

	/** Adds to this bounding box to include a given array of points. */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void ExtendBoxByPoints(UPARAM(ref) FBox& box, const TArray<FVector>& points);

	/** Make a box from origin and extent */
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box", meta=(Keywords="construct build"))
	static void MakeBoxFromOriginExtend(UPARAM(ref) FBox& box, const FVector& origin, const FVector& extent);

	/** Adds to this bounding box to include a new bounding volume. */
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void ExtendByBox(UPARAM(ref) FBox& box, const FBox& otherBox);

	/** 
	* Increases the box size.
	*
	* @param amount - The size to increase the volume by.
	*/
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void ExpandBoxByAmount(UPARAM(ref) FBox& box, float amount);

	/**
	* Increases the box size.
	*
	* @param v - The size to increase the volume by.
	*/
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void ExpandBoxByVec(UPARAM(ref) FBox& box, const FVector& v);

	/**
	* Increases the box size.
	*
	* @param negativeDir - The size to increase the volume by in the negative direction (positive values move the bounds outwards)
	* @param positiveDir - The size to increase the volume by in the positive direction (positive values move the bounds outwards)
	*/
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void ExpandBoxByVecDirs(UPARAM(ref) FBox& box, const FVector& negativeDir, const FVector& positiveDir);

	/** 
	* Shifts the bounding box position.
	*
	* @param offset - The vector to shift the box by.
	*/
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void ShiftBoxBy(UPARAM(ref) FBox& box, const FVector& offset);

	/** 
	* Moves the center of bounding box to new destination.
	*
	* @param destination - The destination point to move center of box to.
	* @return A new bounding box.
	*/
	UFUNCTION(BlueprintCallable, Category="RyRuntime|Math|Box")
	static void MoveBoxTo(UPARAM(ref) FBox& box, const FVector& destination);

	/**
	* Gets the center point of this box.
	*
	* @return The center point.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static FVector GetBoxCenter(const FBox& box);

	/**
	* Gets the center and extents of this box.
	*
	* @param center Will contain the box center point.
	* @param extents Will contain the extent around the center.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static void GetBoxCenterAndExtents(const FBox& box, FVector& center, FVector& extents);

	/**
	* Calculates the closest point on or inside the box to a given point in space.
	*
	* @param point - The point in space.
	* @return The closest point on or inside the box.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static FVector GetBoxClosestPointTo(const FBox& box, const FVector& point);

	/**
	* Gets the extents of this box.
	*
	* @return The box extents.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static FVector GetBoxExtent(const FBox& box);

	/**
	* Gets the size of this box.
	*
	* @return The box size.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static FVector GetBoxSize(const FBox& box);

	/**
	* Gets the volume of this box.
	*
	* @return The box volume.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static float GetBoxVolume(const FBox& box);

	/**
	* Checks whether the given bounding box intersects another bounding box.
	* @param box - The first bounding box
	* @param otherBox - The second bounding box
	* @return true if the boxes intersect, false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool BoxIntersects(const FBox& box, const FBox& otherBox);

	/**
	* Checks whether the given bounding box intersects this bounding box in the XY plane.
	* @param box - The first bounding box
	* @param otherBox - The second bounding box
	* @return true if the boxes intersect in the XY Plane, false otherwise.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool BoxIntersectsXY(const FBox& box, const FBox& otherBox);

	/**
	* Returns the overlap Box of two box
	*
	* @param boxA - The first bounding box
	* @param boxB - The second bounding box
	* @param boxOut - the overlap box. It can be 0 if they don't overlap
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static void GetOverlapBox(const FBox& boxA, const FBox& boxB, FBox& boxOut);

	/**
	* Gets a bounding volume transformed by a FTransform object.
	*
	* @param boxToTransform - The box to transform
	* @param transform - The transformation object.
	* @param transformedBox - The transformed box
	* @see TransformProjectBy
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static void TransformBoxBy(const FBox& boxToTransform, const FTransform& transform, FBox& transformedBox);

	/** 
	* Checks whether the given location is inside this box.
	* 
	* @param in - The location to test for inside the bounding volume.
	* @return true if location is inside this volume.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool IsPositionInsideBox(const FVector& in, const FBox& box);

	/** 
	* Checks whether the given location is inside or on this box.
	* 
	* @param in - The location to test for inside the bounding volume.
	* @return true if location is inside this volume.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool IsPositionInsideOrOnBox(const FVector& in, const FBox& box);

	/** 
	* Checks whether a given box is fully encapsulated by this box.
	* 
	* @param other - The box to test for encapsulation within the bounding volume.
	* @return true if box is inside this volume.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool IsBoxInsideOtherBox(const FBox& box, const FBox& other);

	/** 
	* Checks whether the given location is inside this box in the XY plane.
	* 
	* @param in - The location to test for inside the bounding box.
	* @return true if location is inside this box in the XY plane.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool IsPositionInsideBoxXY(const FVector& in, const FBox& box);

	/** 
	* Checks whether the given box is fully encapsulated by this box in the XY plane.
	* 
	* @param other The box to test for encapsulation within the bounding box.
	* @return true if box is inside this box in the XY plane.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static bool IsBoxInsideOtherBoxXY(const FBox& box, const FBox& other);

	/**
	* Get a textual representation of a box.
	*
	* @return A string describing the box.
	*/
	UFUNCTION(BlueprintPure, Category="RyRuntime|Math|Box")
	static FString BoxToString(const FBox& box);
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::SinH(float A)
{
    return sinhf(A);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::CosH(float A)
{
    return coshf(A);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::CalculateCatenary(float X, float scalingFactor)
{
    return scalingFactor * coshf(X / scalingFactor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::ConvertUnit(const float value, const ERyUnit from, const ERyUnit to)
{
	return FUnitConversion::Convert<float>(value, static_cast<EUnit>(from), static_cast<EUnit>(to));
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::RotationsEqual(const float rotation1, const float rotation2, const float ErrorTolerance)
{
	return FMath::Abs(ShortestRotationPath(rotation1, rotation2)) <= ErrorTolerance;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FQuat URyRuntimeMathHelpers::Rotator_Quat(const FRotator& R)
{
	return R.Quaternion();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::MakeNegative(const float inFloat)
{
	return FMath::Abs(inFloat) * -1.0f;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::MakeNegativeInline(float& inFloat)
{
	inFloat = FMath::Abs(inFloat) * -1.0f;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::MakePositive(const float inFloat)
{
	return FMath::Abs(inFloat);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::MakePositiveInline(float& inFloat)
{
	inFloat = FMath::Abs(inFloat);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
int32 URyRuntimeMathHelpers::IndexNone()
{
	return INDEX_NONE;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
uint8 URyRuntimeMathHelpers::And_ByteByte(uint8 A, uint8 B)
{
    return A & B;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
uint8 URyRuntimeMathHelpers::Xor_ByteByte(uint8 A, uint8 B)
{
    return A ^ B;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
uint8 URyRuntimeMathHelpers::Or_ByteByte(uint8 A, uint8 B)
{
    return A | B;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
uint8 URyRuntimeMathHelpers::Not_Byte(uint8 A)
{
    return ~A;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
uint8 URyRuntimeMathHelpers::ShiftLeft_Byte(uint8 val, int32 shift)
{
    return val << shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
uint8 URyRuntimeMathHelpers::ShiftRight_Byte(uint8 val, int32 shift)
{
    return val >> shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
int32 URyRuntimeMathHelpers::ShiftLeft_Int(int32 val, int32 shift)
{
    return val << shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
int32 URyRuntimeMathHelpers::ShiftRight_Int(int32 val, int32 shift)
{
    return val >> shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
int64 URyRuntimeMathHelpers::ShiftLeft_Int64(int64 val, int32 shift)
{
    return val << shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
int64 URyRuntimeMathHelpers::ShiftRight_Int64(int64 val, int32 shift)
{
    return val >> shift;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::Dot_VectorVector2D(FVector A, FVector B)
{
	A.Z = 0; B.Z = 0;
	return FVector::DotProduct(A, B);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::EqualEqual_VectorVector2D(FVector A, FVector B, float ErrorTolerance)
{
	A.Z = 0; B.Z = 0;
	return A.Equals(B, ErrorTolerance);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::NotEqual_VectorVector2D(FVector A, FVector B, float ErrorTolerance)
{
	A.Z = 0; B.Z = 0;
	return !A.Equals(B, ErrorTolerance);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetForwardVector2D(FRotator InRot)
{
	return InRot.Vector().GetSafeNormal2D();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetRightVector2D(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::Y).GetSafeNormal2D();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetUpVector2D(FRotator InRot)
{
	return FRotationMatrix(InRot).GetScaledAxis(EAxis::Z).GetSafeNormal2D();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::MakeBoxFromPoints(const TArray<FVector>& points, FBox& box)
{
	box.Init();
	for (const FVector& point : points)
	{
		box += point;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::ExtendBoxByPoints(FBox& box, const TArray<FVector>& points)
{
	for (const FVector& point : points)
	{
		box += point;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::MakeBoxFromOriginExtend(FBox& box, const FVector& origin, const FVector& extent)
{
	box.Min = origin - extent;
	box.Max = origin + extent;
	box.IsValid = 1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::ExtendByBox(FBox& box, const FBox& otherBox)
{
	box += otherBox;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::ExpandBoxByAmount(FBox& box, float amount)
{
	const FVector amnt(amount, amount, amount);
	box.Min -= amnt;
	box.Max += amnt;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::ExpandBoxByVec(FBox& box, const FVector& v)
{
	box.Min -= v;
	box.Max += v;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::ExpandBoxByVecDirs(FBox& box, const FVector& negativeDir, const FVector& positiveDir)
{
	box.Min -= negativeDir;
	box.Max += positiveDir;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::ShiftBoxBy(FBox& box, const FVector& offset)
{
	box.Min += offset;
	box.Max += offset;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::MoveBoxTo(FBox& box, const FVector& destination)
{
	const FVector offset = destination - box.GetCenter();
	box.Min += offset;
	box.Max += offset;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetBoxCenter(const FBox& box)
{
	return (box.Min + box.Max) * 0.5f;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::GetBoxCenterAndExtents(const FBox& box, FVector& center, FVector& extents)
{
	extents = box.GetExtent();
	center = box.Min + extents;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetBoxClosestPointTo(const FBox& box, const FVector& point)
{
	return box.GetClosestPointTo(point);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetBoxExtent(const FBox& box)
{
	return box.GetExtent();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FVector URyRuntimeMathHelpers::GetBoxSize(const FBox& box)
{
	return box.Max - box.Min;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
float URyRuntimeMathHelpers::GetBoxVolume(const FBox& box)
{
	return box.GetVolume();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::BoxIntersects(const FBox& box, const FBox& otherBox)
{
	return box.Intersect(otherBox);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::BoxIntersectsXY(const FBox& box, const FBox& otherBox)
{
	return box.IntersectXY(otherBox);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::GetOverlapBox(const FBox& boxA, const FBox& boxB, FBox& boxOut)
{
	boxOut = boxA.Overlap(boxB);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
void URyRuntimeMathHelpers::TransformBoxBy(const FBox& boxToTransform, const FTransform& transform,
	FBox& transformedBox)
{
	transformedBox = boxToTransform.TransformBy(transform);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::IsPositionInsideBox(const FVector& in, const FBox& box)
{
	return box.IsInside(in);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::IsPositionInsideOrOnBox(const FVector& in, const FBox& box)
{
	return box.IsInsideOrOn(in);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::IsBoxInsideOtherBox(const FBox& box, const FBox& other)
{
	return box.IsInside(other);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::IsPositionInsideBoxXY(const FVector& in, const FBox& box)
{
	return box.IsInsideXY(in);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
bool URyRuntimeMathHelpers::IsBoxInsideOtherBoxXY(const FBox& box, const FBox& other)
{
	return box.IsInsideXY(other);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FORCEINLINE
FString URyRuntimeMathHelpers::BoxToString(const FBox& box)
{
	return box.ToString();
}
