#pragma once
#include <Engine/Math/Vector.h>

enum class CollisionType
{
	Rigidbodies,
	RigidbodyStatic
};

struct CollisionInfo
{
	Vector3 Point;
	Vector3 Normal;
	const ICollider *CollThis;
	const ICollider *CollOther;
};