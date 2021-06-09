//
//  easing.c
//
//  Copyright (c) 2011, Auerhaus Development, LLC
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What The Fuck You Want
//  To Public License, Version 2, as published by Sam Hocevar. See
//  http://sam.zoy.org/wtfpl/COPYING for more details.
//

#include "easing.h"
#include <math.h>

#ifdef M_PI
#undef M_PI
#endif
#ifdef M_PI_2
#undef M_PI_2
#endif
#define M_PI 3.1415926535897932f
#define M_PI_2 1.57079632679f

// Modeled after the line y = x
AHFloat LinearInterpolation(AHFloat p)
{
	return p;
}

// Modeled after the parabola y = x^2
AHFloat QuadraticEaseIn(AHFloat p)
{
	return p * p;
}

// Modeled after the parabola y = -x^2 + 2x
AHFloat QuadraticEaseOut(AHFloat p)
{
	return -(p * (p - 2.f));
}

// Modeled after the piecewise quadratic
// y = (1/2)((2x)^2)             ; [0, 0.5)
// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
AHFloat QuadraticEaseInOut(AHFloat p)
{
	if(p < 0.5f)
	{
		return 2.f * p * p;
	}
	else
	{
		return (-2.f * p * p) + (4.f * p) - 1.f;
	}
}

// Modeled after the cubic y = x^3
AHFloat CubicEaseIn(AHFloat p)
{
	return p * p * p;
}

// Modeled after the cubic y = (x - 1)^3 + 1
AHFloat CubicEaseOut(AHFloat p)
{
	AHFloat f = (p - 1.f);
	return f * f * f + 1.f;
}

// Modeled after the piecewise cubic
// y = (1/2)((2x)^3)       ; [0, 0.5)
// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
AHFloat CubicEaseInOut(AHFloat p)
{
	if(p < 0.5f)
	{
		return 4.f * p * p * p;
	}
	else
	{
		AHFloat f = ((2.f * p) - 2.f);
		return 0.5f * f * f * f + 1.f;
	}
}

// Modeled after the quartic x^4
AHFloat QuarticEaseIn(AHFloat p)
{
	return p * p * p * p;
}

// Modeled after the quartic y = 1 - (x - 1)^4
AHFloat QuarticEaseOut(AHFloat p)
{
	AHFloat f = (p - 1.f);
	return f * f * f * (1.f - p) + 1.f;
}

// Modeled after the piecewise quartic
// y = (1/2)((2x)^4)        ; [0, 0.5)
// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
AHFloat QuarticEaseInOut(AHFloat p) 
{
	if(p < 0.5f)
	{
		return 8.f * p * p * p * p;
	}
	else
	{
		AHFloat f = (p - 1.f);
		return -8.f * f * f * f * f + 1.f;
	}
}

// Modeled after the quintic y = x^5
AHFloat QuinticEaseIn(AHFloat p) 
{
	return p * p * p * p * p;
}

// Modeled after the quintic y = (x - 1)^5 + 1
AHFloat QuinticEaseOut(AHFloat p) 
{
	AHFloat f = (p - 1.f);
	return f * f * f * f * f + 1.f;
}

// Modeled after the piecewise quintic
// y = (1/2)((2x)^5)       ; [0, 0.5)
// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
AHFloat QuinticEaseInOut(AHFloat p) 
{
	if(p < 0.5f)
	{
		return 16.f * p * p * p * p * p;
	}
	else
	{
		AHFloat f = ((2.f * p) - 2.f);
		return  0.5f * f * f * f * f * f + 1.f;
	}
}

// Modeled after quarter-cycle of sine wave
AHFloat SineEaseIn(AHFloat p)
{
	return sinf((p - 1.f) * M_PI_2) + 1.f;
}

// Modeled after quarter-cycle of sine wave (different phase)
AHFloat SineEaseOut(AHFloat p)
{
	return sinf(p * M_PI_2);
}

// Modeled after half sine wave
AHFloat SineEaseInOut(AHFloat p)
{
	return 0.5f * (1.f - cosf(p * M_PI));
}

// Modeled after shifted quadrant IV of unit circle
AHFloat CircularEaseIn(AHFloat p)
{
	return 1.f - sqrtf(1.f - (p * p));
}

// Modeled after shifted quadrant II of unit circle
AHFloat CircularEaseOut(AHFloat p)
{
	return sqrtf((2.f - p) * p);
}

// Modeled after the piecewise circular function
// y = (1/2)(1 - sqrtf(1 - 4x^2))           ; [0, 0.5)
// y = (1/2)(sqrtf(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
AHFloat CircularEaseInOut(AHFloat p)
{
	if(p < 0.5f)
	{
		return 0.5f * (1.f - sqrtf(1.f - 4.f * (p * p)));
	}
	else
	{
		return 0.5f * (sqrtf(-((2.f * p) - 3.f) * ((2.f * p) - 1.f)) + 1.f);
	}
}

// Modeled after the exponential function y = 2^(10(x - 1))
AHFloat ExponentialEaseIn(AHFloat p)
{
	return (p == 0.f) ? p : powf(2.f, 10.f * (p - 1.f));
}

// Modeled after the exponential function y = -2^(-10x) + 1
AHFloat ExponentialEaseOut(AHFloat p)
{
	return (p == 1.f) ? p : 1.f - powf(2.f, -10.f * p);
}

// Modeled after the piecewise exponential
// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
AHFloat ExponentialEaseInOut(AHFloat p)
{
	if(p == 0.f || p == 1.f) return p;
	
	if(p < 0.5f)
	{
		return 0.5f * powf(2.f, (20.f * p) - 10.f);
	}
	else
	{
		return -0.5f * powf(2.f, (-20.f * p) + 10.f) + 1.f;
	}
}

// Modeled after the damped sine wave y = sinf(13pi/2*x)*powf(2, 10 * (x - 1))
AHFloat ElasticEaseIn(AHFloat p)
{
	return sinf(13.f * M_PI_2 * p) * powf(2.f, 10.f * (p - 1.f));
}

// Modeled after the damped sine wave y = sinf(-13pi/2*(x + 1))*powf(2, -10x) + 1
AHFloat ElasticEaseOut(AHFloat p)
{
	return sinf(-13.f * M_PI_2 * (p + 1.f)) * powf(2.f, -10.f * p) + 1.f;
}

// Modeled after the piecewise exponentially-damped sine wave:
// y = (1/2)*sinf(13pi/2*(2*x))*powf(2, 10 * ((2*x) - 1))      ; [0,0.5)
// y = (1/2)*(sinf(-13pi/2*((2x-1)+1))*powf(2,-10(2*x-1)) + 2) ; [0.5, 1]
AHFloat ElasticEaseInOut(AHFloat p)
{
	if(p < 0.5f)
	{
		return 0.5f * sinf(13.f * M_PI_2 * (2.f * p)) * powf(2.f, 10.f * ((2.f * p) - 1.f));
	}
	else
	{
		return 0.5f * (sinf(-13.f * M_PI_2 * ((2.f * p - 1.f) + 1.f)) * powf(2, -10 * (2.f * p - 1.f)) + 2.f);
	}
}

// Modeled after the overshooting cubic y = x^3-x*sinf(x*pi)
AHFloat BackEaseIn(AHFloat p)
{
	return p * p * p - p * sinf(p * M_PI);
}

// Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sinf((1-x)*pi))
AHFloat BackEaseOut(AHFloat p)
{
	AHFloat f = (1.f - p);
	return 1.f - (f * f * f - f * sinf(f * M_PI));
}

// Modeled after the piecewise overshooting cubic function:
// y = (1/2)*((2x)^3-(2x)*sinf(2*x*pi))           ; [0, 0.5)
// y = (1/2)*(1-((1-x)^3-(1-x)*sinf((1-x)*pi))+1) ; [0.5, 1]
AHFloat BackEaseInOut(AHFloat p)
{
	if(p < 0.5f)
	{
		AHFloat f = 2.f * p;
		return 0.5f * (f * f * f - f * sinf(f * M_PI));
	}
	else
	{
		AHFloat f = (1.f - (2.f * p - 1.f));
		return 0.5f * (1.f - (f * f * f - f * sinf(f * M_PI))) + 0.5f;
	}
}

AHFloat BounceEaseIn(AHFloat p)
{
	return 1.f - BounceEaseOut(1.f - p);
}

AHFloat BounceEaseOut(AHFloat p)
{
	if(p < 4.f / 11.f)
	{
		return (12.f * p * p) / 16.f;
	}
	else if(p < 8.f / 11.f)
	{
		return (363.f / 40.f * p * p) - (99.f / 10.f * p) + 17.f /5.f;
	}
	else if(p < 9.f / 10.f)
	{
		return (4356.f / 361.f * p * p) - (35442.f / 1805.f * p) + 16061.f / 1805.f;
	}
	else
	{
		return (54.f / 5.f * p * p) - (513.f / 25.f * p) + 268.f / 25.f;
	}
}

AHFloat BounceEaseInOut(AHFloat p)
{
	if(p < 0.5f)
	{
		return 0.5f * BounceEaseIn(p * 2.f);
	}
	else
	{
		return 0.5f * BounceEaseOut(p * 2.f - 1.f) + 0.5f;
	}
}
