// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "Math/RyMathEasing.h"
#include "AHEasing/easing.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
float URyMathEasing::EaseFloat(const ERyMathEasingType easing, float alpha)
{
	alpha = FMath::Clamp(alpha, 0.0f, 1.0f);
	switch(easing)
	{
		case ERyMathEasingType::Linear:
		{
			break;
		}
		case ERyMathEasingType::QuadraticEaseIn:
		{
			alpha = QuadraticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::QuadraticEaseOut:
		{
			alpha = QuadraticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::QuadraticEaseInOut:
		{
			alpha = QuadraticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::CubicEaseIn:
		{
			alpha = CubicEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::CubicEaseOut:
		{
			alpha = CubicEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::CubicEaseInOut:
		{
			alpha = CubicEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::QuarticEaseIn:
		{
			alpha = QuarticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::QuarticEaseOut:
		{
			alpha = QuarticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::QuarticEaseInOut:
		{
			alpha = QuarticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::QuinticEaseIn:
		{
			alpha = QuinticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::QuinticEaseOut:
		{
			alpha = QuinticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::QuinticEaseInOut:
		{
			alpha = QuinticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::SineEaseIn:
		{
			alpha = SineEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::SineEaseOut:
		{
			alpha = SineEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::SineEaseInOut:
		{
			alpha = SineEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::CircularEaseIn:
		{
			alpha = CircularEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::CircularEaseOut:
		{
			alpha = CircularEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::CircularEaseInOut:
		{
			alpha = CircularEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::ExponentialEaseIn:
		{
			alpha = ExponentialEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::ExponentialEaseOut:
		{
			alpha = ExponentialEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::ExponentialEaseInOut:
		{
			alpha = ExponentialEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::ElasticEaseIn:
		{
			alpha = ElasticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::ElasticEaseOut:
		{
			alpha = ElasticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::ElasticEaseInOut:
		{
			alpha = ElasticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::BackEaseIn:
		{
			alpha = BackEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::BackEaseOut:
		{
			alpha = BackEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::BackEaseInOut:
		{
			alpha = BackEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::BounceEaseIn:
		{
			alpha = BounceEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::BounceEaseOut:
		{
			alpha = BounceEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::BounceEaseInOut:
		{
			alpha = BounceEaseInOut(alpha);
			break;
		}
	}

	return alpha;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FVector URyMathEasing::EaseVector(const ERyMathEasingType easing, const FVector& start, const FVector& target, const float alpha)
{
	if(alpha == 0.0f)
	{
		return start;
	}
	if(alpha == 1.0f)
	{
		return target;
	}
	return start + URyMathEasing::EaseFloat(easing, alpha) * (target - start);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FRotator URyMathEasing::EaseRotator(const ERyMathEasingType easing, const FRotator& start, const FRotator& target, const float alpha)
{
	if(alpha == 0.0f)
	{
		return start;
	}
	if(alpha == 1.0f)
	{
		return target;
	}

    const FRotator Delta = (target - start).GetNormalized();
    
    // If steps are too small, just return Target and assume we have reached our destination.
    if (Delta.IsNearlyZero())
    {
    	return target;
    }

    const FRotator DeltaMove = Delta * URyMathEasing::EaseFloat(easing, alpha);
    return (start + DeltaMove).GetNormalized();
}
