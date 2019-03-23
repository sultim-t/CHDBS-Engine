#pragma once
#include "CollisionType.h"

// Contains collision info from broad phase
struct BroadCollisionInfo
{
	// Type of collision
	// Note: if collision is between rigidbody and static collider, RbOther is null
	CollisionType Type;

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

	inline BroadCollisionInfo(CollisionType type) :
		Type(type),
		CollThis(nullptr),
		CollOther(nullptr),
		RbThis(nullptr),
		RbOther(nullptr) {}
};