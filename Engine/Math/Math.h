#pragma once

namespace Mathf
{
#define PI 3.1415926F
#define DEG2RAD( a ) ( ( (a) * PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / PI )

		float Sqrt(float x);
		double SqrtD(double x);
		float MathReverseSqrt(float x);
		int MinusOnePower(int n);
		float Sin(float x);
		float Cos(float x);
		float Tan(float x);
		float Abs(float x);
		float ASin(float x);
		float ACos(float x);
		float CopySign(float x, float sign);
		float ATan2(float y, float x);

		float Min(float x, float y);
		float Max(float x, float y);
		float Round(float x);
		float Ceil(float x);
		float Floor(float x);
		float Mod(float x, float y);
		float Exp(float x);
		float Log(float x);
		float Log10(float x);
		float Pow(float base, float exp);
		float Lerp(float a, float b, float t);
}