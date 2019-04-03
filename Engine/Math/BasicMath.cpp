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

float Min(float x, float y)
{
	return fminf(x, y);
}

float Max(float x, float y)
{
	return fmaxf(x, y);
}

float Round(float x)
{
	return roundf(x);
}

float Ceil(float x)
{
	return ceilf(x);
}

float Floor(float x)
{
	return floorf(x);
}

float Mod(float x, float y)
{
	return fmodf(x,y);
}

float Exp(float x)
{
	return expf(x);
}

float Log(float x)
{
	return logf(x);
}

float Log10(float x)
{
	return log10f(x);
}

float Pow(float base, float exp)
{
	return powf(base, exp);
}
