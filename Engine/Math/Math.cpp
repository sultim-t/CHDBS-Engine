#include "Math.h"
#include <cmath>

float Mathf::Sqrt(float x)
{
	return sqrtf(x);
}

double Mathf::SqrtD(double x)
{
	return sqrt(x);
}

float Mathf::MathReverseSqrt(float x)
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

int Mathf::MinusOnePower(int n)
{
	if (n % 2 == 0)
	{
		return 1;
	}

	return -1;
}

float Mathf::Sin(float x)
{
	return sinf(x);
}

float Mathf::Cos(float x)
{
	return cosf(x);
}

float Mathf::Tan(float x)
{
	return tanf(x);
}

float Mathf::Abs(float x)
{
	return fabs(x);
}

float Mathf::ASin(float x)
{
	return asinf(x);
}

float Mathf::ACos(float x)
{
	return acosf(x);
}

float Mathf::CopySign(float x, float sign)
{
	return copysignf(x, sign);
}

float Mathf::ATan2(float y, float x)
{
	return atan2f(y, x);
}

float Mathf::Min(float x, float y)
{
	return fminf(x, y);
}

float Mathf::Max(float x, float y)
{
	return fmaxf(x, y);
}

float Mathf::Round(float x)
{
	return roundf(x);
}

float Mathf::Ceil(float x)
{
	return ceilf(x);
}

float Mathf::Floor(float x)
{
	return floorf(x);
}

float Mathf::Mod(float x, float y)
{
	return fmodf(x,y);
}

float Mathf::Exp(float x)
{
	return expf(x);
}

float Mathf::Log(float x)
{
	return logf(x);
}

float Mathf::Log10(float x)
{
	return log10f(x);
}

float Mathf::Pow(float base, float exp)
{
	return powf(base, exp);
}

float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}
