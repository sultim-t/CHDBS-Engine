#pragma once
#include "CollisionType.h"
#include "BroadCollisionInfo.h"
#include <Engine/Math/Vector.h>

// Contains collision info
struct CollisionInfo
{
	// Type of collision
	// Note: if collision is between rigidbody and static collider, RbOther is null
	CollisionType Type;

	Vector3 Point;
	Vector3 Normal;
	float Penetration;

	// Collider of this rigidbody
	const ICollider *CollThis;
	// Collider of other rigidbody or static object
	const ICollider *CollOther;
	// This rigidbody
	// Note: can be modified in collision solving
	Rigidbody *RbThis;
	// Other rigidbody
	// Note: maybe null if other is static object
	// Note: can be modified in collision solving
	Rigidbody *RbOther;

	inline CollisionInfo(CollisionType type) : 
		Type(type), 
		CollThis(nullptr),
		CollOther(nullptr),
		RbThis(nullptr),
		RbOther(nullptr) {}

	// Copy collision info from broad phase
	inline CollisionInfo(const BroadCollisionInfo &info)
	{
		Type = info.Type;
		CollThis = info.CollThis;
		CollOther = info.CollOther;
		RbThis = info.RbThis;
		RbOther = info.RbOther;
	}
};

