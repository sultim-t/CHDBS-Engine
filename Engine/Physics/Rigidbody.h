#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Components/IComponent.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Transform.h>
#include "ICollider.h"
#include "CollisionInfo.h"

class Rigidbody : public IComponent
{
	friend class PhysicsSystem;

	CLASSDECLARATION(Rigidbody)

private:
	// pointer to current transform
	Transform	*transform;
	Vector3		velocity;
	Vector3		acceleration;
	float		mass;

	DynamicArray<Vector3> allForces;
	DynamicArray<Vector3> allImpulses;

	// current rigidbody collider
	// which is attached to current entity
	ICollider *collider;
	
public:
	~Rigidbody();

	void AddForce(const Vector3 &force);
	void AddImpulse(const Vector3 &impulse);

	Vector3 &GetVelocity();
	const Vector3 &GetVelocity() const;
	const Vector3 &GetAcceleration() const;

	void FixedUpdate();
	void SolveCollisions(const CollisionInfo &info);

	const ICollider &GetCollider() const;

public:
	// Initialization as component
	void Init() override;
	// Empty update function
	// Rigidbodies are updated in PhysicsSystem through FixedUpdate()
	void Update() override {};

	void SetProperty(const String &key, const String &value) override;
};