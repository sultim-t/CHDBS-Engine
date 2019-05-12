#pragma once
#include "CollisionInfo.h"
#include "PhysicMaterial.h"
#include <Engine/Math/Sphere.h>
#include <Engine/Math/Transform.h>
#include <Engine/Base/EventParam.h>

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
private:
	EventParam<CollisionInfo> onTriggerEnter;
	EventParam<CollisionInfo> onTriggerExit;
	EventParam<CollisionInfo> onCollisionEnter;
	EventParam<CollisionInfo> onCollisionExit;

protected:
	PhysicMaterial physicMaterial;

public:
	bool IsTrigger;

public:
	// This event will be called on collision
	//   - if there is rigidbody on this entity 
	//   - and this collider is trigger
	// NOTE: info can be relative to other collider, 
	// so there must be a check this == info->CollThis
	EventParam<CollisionInfo> &OnTriggerEnter();
	// This event will be called on collision
	//   - if there is rigidbody on this entity 
	// NOTE: info can be relative to other collider, 
	// so there must be a check this == info->CollThis
	EventParam<CollisionInfo> &OnCollisionEnter();

	// This event will be called on collision
	//   - if there is rigidbody on this entity 
	//   - and this collider is trigger
	// NOTE: info can be relative to other collider, 
	// so there must be a check this == info->CollThis
	const EventParam<CollisionInfo> &OnTriggerEnter() const;
	// This event will be called on collision
	//   - if there is rigidbody on this entity 
	// NOTE: info can be relative to other collider, 
	// so there must be a check this == info->CollThis
	const EventParam<CollisionInfo> &OnCollisionEnter() const;

	// Not implemented
	EventParam<CollisionInfo> &OnTriggerExit();
	// Not implemented
	EventParam<CollisionInfo> &OnCollisionExit();

	// Not implemented
	const EventParam<CollisionInfo> &OnTriggerExit() const;
	// Not implemented
	const EventParam<CollisionInfo> &OnCollisionExit() const;

public:
	// Default constructor
	ICollider();

	// Get current collider type
	virtual ColliderType GetColliderType() const = 0;

	// Every collider have a bounding sphere to precheck collisions
	virtual Sphere GetBoundingSphere() const = 0;

	// Intersect this collider with other one, collision info is generated
	virtual bool Intersect(const ICollider &col, CollisionInfo &info) const = 0;

	// Get physic material of this collider
	PhysicMaterial &GetPhysicMaterial();
	// Get physic material of this collider
	const PhysicMaterial &GetPhysicMaterial() const;

	// Get current tranformation of collider
	virtual const Transform &GetTransform() const = 0;
};

inline ICollider::ICollider()
{
	onTriggerEnter.Init();
	onTriggerExit.Init();
	onCollisionEnter.Init();
	onCollisionExit.Init();
}

inline PhysicMaterial & ICollider::GetPhysicMaterial()
{
	return physicMaterial;
}

inline const PhysicMaterial & ICollider::GetPhysicMaterial() const
{
	return physicMaterial;
}

inline EventParam<CollisionInfo> &ICollider::OnTriggerEnter()
{
	return onTriggerEnter;
}
inline EventParam<CollisionInfo> &ICollider::OnTriggerExit()
{
	return onTriggerExit;
}
inline EventParam<CollisionInfo> &ICollider::OnCollisionEnter()
{
	return onCollisionEnter;
}
inline EventParam<CollisionInfo> &ICollider::OnCollisionExit()
{
	return onCollisionExit;
}

inline const EventParam<CollisionInfo> &ICollider::OnTriggerEnter() const
{
	return onTriggerEnter;
}
inline const EventParam<CollisionInfo> &ICollider::OnTriggerExit() const
{
	return onTriggerExit;
}
inline const EventParam<CollisionInfo> &ICollider::OnCollisionEnter() const
{
	return onCollisionEnter;
}
inline const EventParam<CollisionInfo> &ICollider::OnCollisionExit() const
{
	return onCollisionExit;
}