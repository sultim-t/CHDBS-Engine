#pragma once

#define PI 3.1415926F
#define DEG2RAD( a ) ( ( (a) * PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / PI )
#define ABS(x)  ((x) > 0? (x) : -(x))

// approximate comparison
#define EPSILONEQUAL 0.001f
#define EPSILONESQRT 0.0001f

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