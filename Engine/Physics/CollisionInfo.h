#pragma once
#include "ICollider.h"
#include <Engine/Math/Vector.h>

class CollisionInfo
{
public:
	Vector3 normal;
	ICollider &colliderA, &colliderB;
};