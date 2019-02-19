#include "AABB.h"
#include "Sphere.h"

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

bool AABB::Intersect(const AABB &aabb) const
{
	for (int i = 0; i < 3; i++)
	{
		if (maxBounds[i] < aabb.minBounds[i] || maxBounds[i] > aabb.maxBounds[i])
		{
			return false;
		}
	}

	return true;
}

/*bool AABB::Contacts(const AABB &aabb, const float eps) const
{
	for (int i = 0; i < 3; i++)
	{
		if ((maxBounds[i] + eps < aabb.minBounds[i]) || (maxBounds[i] - eps > aabb.maxBounds[i]))
		{
			return false;
		}
	}

	return true;
}*/

bool AABB::Intersect(const Sphere &sphere) const
{
	const Vector3 &center = sphere.GetCenter();
	const float radius = sphere.GetRadius();

	for (int i = 0; i < 3; i++)
	{
		if ((center[i] + radius < minBounds[i]) || (center[i] - radius > maxBounds[i]))
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