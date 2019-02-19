#include "PhysicsSystem.h"

void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update()
{
	FOREACHLINKEDLIST(Rigidbody*, rbPtr, rigidbodies)
	{
		Rigidbody *rb = *rbPtr;

		rb->FixedUpdate();
	}
}

PhysicsSystem &PhysicsSystem::Instance()
{
	static PhysicsSystem instance;
	return instance;
}
