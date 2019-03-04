#pragma once
#include <Engine/Systems/ISystem.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Physics/CollisionInfo.h>

class PhysicsSystem : public ISystem
{
private:
	// Stores all rigidbodies with colliders
	DynamicArray<Rigidbody*>	rigidbodies;
	// Stores all colliders without rigidbodies
	DynamicArray<ICollider*>	colliders;
	// Stores all collisions, changed after each update
	// DynamicArray<CollisionInfo> collisions;

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
	void Register(ICollider *col);

	// Check segment from pos to pos+dir*distance
	// If returns true, result of cast will be in info
	bool Raycast(const Vector3 &pos, const Vector3 &dir, const float distance, RaycastInfo &info);
	// Check ray from pos with direction dir
	// If returns true, result of cast will be in info
	bool Raycast(const Vector3 &pos, const Vector3 &dir, RaycastInfo &info);
};