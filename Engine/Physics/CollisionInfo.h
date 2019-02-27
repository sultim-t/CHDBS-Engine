#pragma once
#include "ICollider.h"
#include <Engine/Math/Vector.h>

class CollisionInfo
{
public:
	Vector3 point, normal;
	ICollider &colliderA, &colliderB;
};