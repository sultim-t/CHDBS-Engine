#pragma once
#include <Engine/Math/Vector.h>

struct CollisionInfo
{
	Vector3 Point;
	Vector3 Normal;
	const ICollider *CollThis;
	const ICollider *CollOther;
};