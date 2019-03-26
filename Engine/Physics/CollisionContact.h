#pragma once
#include <Engine/Math/Vector.h>

struct CollisionContact
{
	Vector3 Point;
	Vector3 Normal;
	float Penetration;
};