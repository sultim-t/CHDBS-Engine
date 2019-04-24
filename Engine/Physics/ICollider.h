#pragma once
#include "CollisionInfo.h"
#include "PhysicMaterial.h"
#include <Engine/Math/Sphere.h>
#include <Engine/Math/Transform.h>

enum class ColliderType
{
	AABB,
	Sphere,
	Mesh
};

enum class PhysicsMessage
{
	CollisionEnter,
	CollisionExit,
	TriggerEnter,
	TriggerExit
};

// Represents collider
class ICollider
{
public:
	bool IsTrigger;

	// Get current collider type
	virtual ColliderType GetColliderType() const = 0;

	// Every collider have a bounding sphere to precheck collisions
	virtual Sphere GetBoundingSphere() const = 0;

	// Intersect this collider with other one, collision info is generated
	virtual bool Intersect(const ICollider &col, CollisionInfo &info) const = 0;

	// Get physic material of this collider
	virtual PhysicMaterial GetPhysicMaterial() const = 0;

	// Get current tranformation of collider
	virtual const Transform &GetTransform() const = 0;

	// Send physics message to all other components
	//virtual void SendPhysicsMessage() = 0;
};