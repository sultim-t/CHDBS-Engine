#pragma once
#include <Engine/Systems/ISystem.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Physics/CollisionInfo.h>

class PhysicsSystem : public ISystem
{
private:
	// Stores all rigidbodies with colliders
	DynamicArray<Rigidbody*> rigidbodies;
	// Stores all colliders without rigidbodies
	DynamicArray<Rigidbody*> colliders;
	// Stores all collisions, changed after each update
	DynamicArray<CollisionInfo> collisions;

public:
	// Global gravity
	static Vector3 Gravity;

public:
	void Init() override;
	void Update() override;

	void SolveCollisions();

	// Get instance of system
	static PhysicsSystem &Instance();

public:
	// Register dynamic physics object
	void Register(Rigidbody *rb);
	// Register static physics object
	void Register(const ICollider *col);
};