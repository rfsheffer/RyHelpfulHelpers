// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyMathEasing.generated.h"

/** The different easings available */
UENUM(Blueprintable)
enum class ERyMathEasingType : uint8
{
	Linear,
	QuadraticEaseIn,
	QuadraticEaseOut,
	QuadraticEaseInOut,
	CubicEaseIn,
	CubicEaseOut,
	CubicEaseInOut,
	QuarticEaseIn,
	QuarticEaseOut,
	QuarticEaseInOut,
	QuinticEaseIn,
	QuinticEaseOut,
	QuinticEaseInOut,
	SineEaseIn,
	SineEaseOut,
	SineEaseInOut,
	CircularEaseIn,
	CircularEaseOut,
	CircularEaseInOut,
	ExponentialEaseIn,
	ExponentialEaseOut,
	ExponentialEaseInOut,
	ElasticEaseIn,
	ElasticEaseOut,
	ElasticEaseInOut,
	BackEaseIn,
	BackEaseOut,
	BackEaseInOut,
	BounceEaseIn,
	BounceEaseOut,
	BounceEaseInOut
};

/**
 * Math Library for common easing functions.
 * Uses AHEasing internally. Copyright (c) 2011, Auerhaus Development, LLC
 * Under the Do What The Fuck You Want To Public License, Version 2
 */
UCLASS(meta=(BlueprintThreadSafe))
class RYRUNTIME_API URyMathEasing : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Determine the current easing value of alpha (See https://easings.net/ for visual output examples)
	 * @param easing - The easing to use
	 * @param alpha - The alpha, clamped 0-1
	 * @return The easing value, see https://easings.net/ to see visually what values between 0 and 1 will produce.
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Easing")
	static float EaseFloat(const ERyMathEasingType easing = ERyMathEasingType::Linear, float alpha = 0.0f);

	/**
	 * Ease a vector from start to target
	 * @param easing - The easing to use
	 * @param start - The starting vector
	 * @param target - The target vector
	 * @param alpha - The alpha, clamped 0-1
	 * @return The vector eased from start to target depending on the alpha
	 */
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Easing")
	static FVector EaseVector(const ERyMathEasingType easing, const FVector& start, const FVector& target, const float alpha = 0.0f);

	/**
	* Ease a rotator from start to target
	* @param easing - The easing to use
	* @param start - The starting rotator
	* @param target - The target rotator
	* @param alpha - The alpha, clamped 0-1
	* @return The rotator eased from start to target depending on the alpha
	*/
	UFUNCTION(BlueprintPure, Category = "RyRuntime|Math|Easing")
	static FRotator EaseRotator(const ERyMathEasingType easing, const FRotator& start, const FRotator& target, const float alpha = 0.0f);
};
