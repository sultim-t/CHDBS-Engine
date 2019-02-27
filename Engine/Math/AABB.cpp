#include "AABB.h"

AABB::AABB()
{ }

AABB::AABB(const Vector3 &vec)
{
	for (int i = 0; i < 3; i++)
	{
		minBounds[i] = maxBounds[i] = vec[i];
	}
}

AABB::AABB(const Vector3 &center, const float radius)
{
	for (int i = 0; i < 3; i++)
	{
		minBounds[i] = center[i] - radius;
		maxBounds[i] = center[i] + radius;
	}
}

AABB::AABB(const Vector3 &vec1, const Vector3 &vec2)
{
	for (int i = 0; i < 3; i++)
	{
		minBounds[i] = vec1[i] < vec2[i] ? vec1[i] : vec2[i];
		maxBounds[i] = vec1[i] > vec2[i] ? vec1[i] : vec2[i];
	}
}

bool AABB::IsEmpty() const
{
	for (int i = 0; i < 3; i++)
	{
		if (minBounds[i] > maxBounds[i])
		{
			return true;
		}
	}

	return false;
}

void AABB::Nullify()
{
	for (int i = 0; i < 3; i++)
	{
		minBounds[i] = 0.0f;
		maxBounds[i] = 0.0f;
	}
}

void AABB::Move(const Vector3 &vec)
{
	minBounds += vec;
	maxBounds += vec;
}

bool AABB::Contains(const AABB &aabb) const
{
	for (int i = 0; i < 3; i++)
	{
		if (aabb.minBounds[i] < minBounds[i] || aabb.maxBounds[i] > maxBounds[i])
		{
			return false;
		}
	}

	return true;
}

bool AABB::IsContained(const AABB &aabb) const
{
	for (int i = 0; i < 3; i++)
	{
		if (aabb.minBounds[i] > minBounds[i] || aabb.maxBounds[i] < maxBounds[i])
		{
			return false;
		}
	}

	return true;
}

AABB AABB::GetUnion(const AABB &aabb1, const AABB &aabb2)
{	
	AABB result;

	for (int i = 0; i < 3; i++)
	{
		result.minBounds[i] = aabb1.minBounds[i] < aabb2.minBounds[i] ? aabb1.minBounds[i] : aabb2.minBounds[i];
		result.maxBounds[i] = aabb1.maxBounds[i] > aabb2.maxBounds[i] ? aabb1.maxBounds[i] : aabb2.maxBounds[i];
	}

	return result;
}

AABB AABB::GetIntersection(const AABB &aabb1, const AABB &aabb2)
{
	AABB result;

	for (int i = 0; i < 3; i++)
	{
		result.minBounds[i] = aabb1.minBounds[i] > aabb2.minBounds[i] ? aabb1.minBounds[i] : aabb2.minBounds[i];
		result.maxBounds[i] = aabb1.maxBounds[i] < aabb2.maxBounds[i] ? aabb1.maxBounds[i] : aabb2.maxBounds[i];
	}

	if (result.IsEmpty())
	{
		result.Nullify();
	}

	return result;
}

AABB AABB::GetUnion(const AABB &aabb) const
{
	return GetUnion(*this, aabb);
}

AABB AABB::GetIntersection(const AABB &aabb) const
{
	return GetIntersection(*this, aabb);
}

Vector3 AABB::GetClosestPoint(const Vector3 & p) const
{
	Vector3 result;

	for (int i = 0; i < 3; i++)
	{
		float v = p[i];

		if (v < minBounds[i])
		{
			v = minBounds[i];
		}
		
		if (v > maxBounds[i])
		{
			v = maxBounds[i];
		}

		result[i] = v;
	}

	return result;
}

float AABB::DistanceSqrTo(const Vector3 & p) const
{
	float sqDist = 0.0f;

	for (int i = 0; i < 3; i++) 
	{
		float v = p[i];
		
		if (v < minBounds[i])
		{
			sqDist += (minBounds[i] - v) * (minBounds[i] - v);
		}
		
		if (v > maxBounds[i])
		{
			sqDist += (v - maxBounds[i]) * (v - maxBounds[i]);
		}
	}

	return sqDist;
}

float AABB::DistanceTo(const Vector3 & p) const
{
	return Sqrt(DistanceSqrTo(p));
}
