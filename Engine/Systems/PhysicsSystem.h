#pragma once
#include <Engine/Systems/ISystem.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/DataStructures/LinkedList.h>

class PhysicsSystem : public ISystem
{
private:
	LinkedList<Rigidbody*> rigidbodies;

public:
	void Init() override;
	void Update() override;

	// Get instance of system
	static PhysicsSystem &Instance();
};