#include "BasicMath.h"
#include <cmath>

float Sqrt(float x)
{
	return sqrtf(x);
}

double SqrtD(double x)
{
	return sqrt(x);
}

// JohnCarmack's
float MathReverseSqrt(float x)
{
	long i;
	float x2, y;
	const float threeHalfs = 1.5f;

	x2 = x * 0.5f;
	y = x;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threeHalfs - (x2 * y * y));

	return y;
}

int MinusOnePower(int n)
{
	if (n % 2 == 0)
	{
		return 1;
	}

	return -1;
}

inline float Sin(float x)
{
	return sinf(x);
}

inline float Cos(float x)
{
	return cosf(x);
}

float Tan(float x)
{
	return tanf(x);
}

inline float Abs(float x)
{
	return fabs(x);
}

inline float ASin(float x)
{
	return asinf(x);
}

inline float ACos(float x)
{
	return acosf(x);
}

inline float CopySign(float x, float sign)
{
	return copysignf(x, sign);
}

inline float ATan2(float y, float x)
{
	return atan2f(y, x);
}

/*
// Taylor series
float SinRad(float x)
{
	// clamp
	float x0 = x - (int)(x / (float)PI) * (float)PI;

	// first iter with n = 0
	float result = x0; 

	for (int n = 1; n <= 4; n++)
	{
		float p = MinusOnePower(n) * x0;

		// calculating factorial and power
		for (int i = 2; i <= 2 * n + 1; i++) 
		{
			p *= x0;
			p /= i;
		}

		result += p;
	}

	return result;
}

float CosRad(float x)
{
	// clamp
	float x0 = x - (int)(x / (float)PI) * (float)PI; 

	// first iter with n = 0
	float result = 1; 

	for (int n = 1; n <= 4; n++)
	{
		float p = MinusOnePower(n) * x0;

		// calculating factorial and power
		for (int i = 2; i <= 2 * n; i++) 
		{
			p *= x0;
			p /= i;
		}

		result += p;
	}

	return result;
}
*/