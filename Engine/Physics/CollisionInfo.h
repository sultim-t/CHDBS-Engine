#pragma once
#include "CollisionContact.h"
#include "CollisionType.h"
#include "ICollider.h"
#include "PhysicMaterial.h"
#include "BroadCollisionInfo.h"
#include <Engine/Math/Vector.h>

// Contains collision info
struct CollisionInfo
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

	CollisionContact Contact;

public:
	CollisionInfo(CollisionType type);

	// Copy collision info from broad phase
	CollisionInfo(const BroadCollisionInfo &info);

	// Get restitution according to combine option in CollThis
	float GetRestitution() const;
	// Get static friction according to combine option in CollThis
	float GetStaticFriction() const;
	// Get dynamic friction according to combine option in CollThis
	float GetDynamicFriction() const;
};

