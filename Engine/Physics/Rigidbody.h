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
	float		inversedMass;

	DynamicArray<Vector3> allForces;
	DynamicArray<Vector3> allImpulses;

	// current rigidbody collider
	// which is attached to current entity
	ICollider *collider;
	
private:
	void FixedUpdate();
	void SolveCollisions(const CollisionInfo &info);

public:
	~Rigidbody();

	// Add a continuous force to this rigidbody, using its mass
	void AddForce(const Vector3 &force);
	// Add an instant impulse to this rigidbody, using its mass
	void AddImpulse(const Vector3 &impulse);
	// Add a continuous acceleration to this rigidbody, ignoring its mass
	// void AddAcceleration(const Vector3 &acceleration);

	// Remove all forced applided to this rigidbody
	void RemoveAllForces();

	// Get current velocity
	Vector3 &GetVelocity();
	// Get current velocity
	const Vector3 &GetVelocity() const;
	// Get current acceleration
	const Vector3 &GetAcceleration() const;
	// Get collider attached to current entity
	const ICollider &GetCollider() const;
	// Get mass of rigidbody
	float GetMass();
	// Get inversed mass of rigidbody
	float GetInversedMass();
	
	// Set mass of this rigidbody
	void SetMass(float mass);

	// Initialization as component
	void Init() override;
	// Empty update function
	// Rigidbodies are updated in PhysicsSystem through FixedUpdate()
	void Update() override {};

	void SetProperty(const String &key, const String &value) override;
};