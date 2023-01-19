// Copyright 2020-2023 Solar Storm Interactive

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
			alpha = AH::QuadraticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::QuadraticEaseOut:
		{
			alpha = AH::QuadraticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::QuadraticEaseInOut:
		{
			alpha = AH::QuadraticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::CubicEaseIn:
		{
			alpha = AH::CubicEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::CubicEaseOut:
		{
			alpha = AH::CubicEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::CubicEaseInOut:
		{
			alpha = AH::CubicEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::QuarticEaseIn:
		{
			alpha = AH::QuarticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::QuarticEaseOut:
		{
			alpha = AH::QuarticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::QuarticEaseInOut:
		{
			alpha = AH::QuarticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::QuinticEaseIn:
		{
			alpha = AH::QuinticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::QuinticEaseOut:
		{
			alpha = AH::QuinticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::QuinticEaseInOut:
		{
			alpha = AH::QuinticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::SineEaseIn:
		{
			alpha = AH::SineEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::SineEaseOut:
		{
			alpha = AH::SineEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::SineEaseInOut:
		{
			alpha = AH::SineEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::CircularEaseIn:
		{
			alpha = AH::CircularEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::CircularEaseOut:
		{
			alpha = AH::CircularEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::CircularEaseInOut:
		{
			alpha = AH::CircularEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::ExponentialEaseIn:
		{
			alpha = AH::ExponentialEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::ExponentialEaseOut:
		{
			alpha = AH::ExponentialEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::ExponentialEaseInOut:
		{
			alpha = AH::ExponentialEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::ElasticEaseIn:
		{
			alpha = AH::ElasticEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::ElasticEaseOut:
		{
			alpha = AH::ElasticEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::ElasticEaseInOut:
		{
			alpha = AH::ElasticEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::BackEaseIn:
		{
			alpha = AH::BackEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::BackEaseOut:
		{
			alpha = AH::BackEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::BackEaseInOut:
		{
			alpha = AH::BackEaseInOut(alpha);
			break;
		}
		case ERyMathEasingType::BounceEaseIn:
		{
			alpha = AH::BounceEaseIn(alpha);
			break;
		}
		case ERyMathEasingType::BounceEaseOut:
		{
			alpha = AH::BounceEaseOut(alpha);
			break;
		}
		case ERyMathEasingType::BounceEaseInOut:
		{
			alpha = AH::BounceEaseInOut(alpha);
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
