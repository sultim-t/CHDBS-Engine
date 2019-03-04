#pragma once
#include <Engine/Math/Vector.h>

struct RaycastInfo
{
	Vector3 Point;
	Vector3 Normal;
	float Distance;
	ICollider *Collider;
};