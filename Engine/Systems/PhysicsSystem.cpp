#include "PhysicsSystem.h"

Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

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

void PhysicsSystem::Register(Rigidbody * rb)
{
	rigidbodies.Add(rb);
}

void PhysicsSystem::Register(const ICollider * col)
{
}
