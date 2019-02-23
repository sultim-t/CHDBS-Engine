#include "Random.h"
#include <stdlib.h>
#include <Engine/Math/Vector.h>

bool Random::GetBool()
{
	return rand() % 2 != 0;
}

int Random::GetInt(int max)
{
	ASSERT(max <= RAND_MAX);
	return rand() % (max + 1);
}

int Random::GetInt(int min, int max)
{
	ASSERT(max - min < RAND_MAX);
	return (rand() % (max + 1 - min)) + min;
}

float Random::GetFloat()
{
	return (float)rand() / (float)(RAND_MAX);
}

float Random::GetFloat(float max)
{
	return (float)rand() / (float)(RAND_MAX) * max;
}

float Random::GetFloat(float min, float max)
{
	return ((float)rand() / (float)(RAND_MAX) * (max - min)) + min;
}

Vector3 Random::GetOnSphere()
{
	Vector3 result = GetInsideSphere();

	// to make unit
	float length = result.Length();
	if (length == 0)
	{
		// there is some chance to get (0,0,0)
		// so return some other value
		return Vector3(1, 0, 0);
	}
	
	// inverted length
	length = 1.0f / length;

	// normalize
	for (int i = 0; i < 3; i++)
	{
		result[i] *= length;
	}

	return result;
}

Vector3 Random::GetOnSphere(float r)
{
	return GetOnSphere() * r;
}

Vector3 Random::GetInsideSphere()
{
	Vector3 result;

	for (int i = 0; i < 3; i++)
	{
		result[i] = Random::GetFloat(-1.0f, 1.0f);
	}

	return result;
}

Vector3 Random::GetInsideSphere(float r)
{
	return GetInsideSphere() * r;
}

Vector3 Random::GetInsideBox(const Vector3 &extent)
{
	Vector3 result;

	for (int i = 0; i < 3; i++)
	{
		result[i] = extent[i] * GetFloat(-1.0f, 1.0f);
	}

	return result;
}
