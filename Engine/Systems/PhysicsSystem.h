#pragma once
#include <Engine/Systems/ISystem.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/DataStructures/LinkedList.h>

class PhysicsSystem : public ISystem
{
private:
	LinkedList<Rigidbody*> rigidbodies;

public:
	static Vector3 Gravity;

public:
	void Init() override;
	void Update() override;

	// Get instance of system
	static PhysicsSystem &Instance();

public:
	// Register dynamic physics object
	void Register(Rigidbody *rb);
	// Register static physics object
	void Register(const ICollider *col);
};