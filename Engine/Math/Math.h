#pragma once

#define PI 3.1415926F
#define DEG2RAD( a ) ( ( (a) * PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / PI )

extern "C" float Sqrt(float x);
extern "C" double SqrtD(double x);
extern "C" float MathReverseSqrt(float x);
extern "C" int MinusOnePower(int n);
extern "C" float Sin(float x);
extern "C" float Cos(float x);
extern "C" float Tan(float x);
extern "C" float Abs(float x);
extern "C" float ASin(float x);
extern "C" float ACos(float x);
extern "C" float CopySign(float x, float sign);
extern "C" float ATan2(float y, float x);

extern "C" float Min(float x, float y);
extern "C" float Max(float x, float y);
extern "C" float Round(float x);
extern "C" float Ceil(float x);
extern "C" float Floor(float x);
extern "C" float Mod(float x, float y);
extern "C" float Exp(float x);
extern "C" float Log(float x);
extern "C" float Log10(float x);
extern "C" float Pow(float base, float exp);
extern "C" float Lerp(float a, float b, float t);
