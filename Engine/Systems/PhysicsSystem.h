#pragma once
#include <Engine/Systems/ISystem.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Physics/CollisionInfo.h>

class PhysicsSystem : public ISystem
{
private:
	// Reference to rigidbodies (with colliders) array
	const DynamicArray<Rigidbody*>	*rigidbodies;

	// Reference to colliders (without rigidbodies) array
	const DynamicArray<ICollider*>	*colliders;

	// Stores all collisions, from broad phase
	// They will be potentially resolved in main phase
	DynamicArray<BroadCollisionInfo>	collisions;

public:
	// Global gravity
	static Vector3 Gravity;

public:
	void Init() override;
	void Update() override;

	// Broad phase of collision detection
	void GetApproximateCollisions();
	// Main phase of collision detection
	void SolveCollisions();

	// Solve collision for objects in collision info
	void SolveCollision(const CollisionInfo &info, int contactIndex);
	// Position correction
	void ApplyPositionCorrection(Rigidbody *rbThis, Rigidbody *rbOther, float invMassThis, float invMassOther, const Vector3 &normal, float penetration);
	// Calculate friction vector
	Vector3 CalculateFriction(const Vector3 &relativeVelocity, const Vector3 &normal, float invMass, float impulse, float staticFriction, float dynamicFriction);

	// Get instance of system
	static PhysicsSystem &Instance();

public:
	//// Register dynamic physics object
	//void Register(Rigidbody *rb);
	//// Register static physics object
	//void Register(ICollider *col);
	
	// Register rigidbodies and colliders to simulate physics
	void Register(const DynamicArray<Rigidbody*> *rigidbodies, const DynamicArray<ICollider*> *colliders);
	// Unregister rigidbodies and colliders
	void Reset();

	// Check segment from pos to pos+dir*distance
	// If returns true, result of cast will be in info
	bool Raycast(const Vector3 &pos, const Vector3 &dir, const float distance, RaycastInfo &info);
	// Check ray from pos with direction dir
	// If returns true, result of cast will be in info
	bool Raycast(const Vector3 &pos, const Vector3 &dir, RaycastInfo &info);
};