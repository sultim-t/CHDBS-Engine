#include "PhysicsSystem.h"

Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

void PhysicsSystem::Init()
{
	rigidbodies.Init(128);
	colliders.Init(128);
	collisions.Init(512);
}

void PhysicsSystem::Update()
{
	int size = rigidbodies.GetSize();

	for (int i = 0; i < size; i++)
	{
		rigidbodies[i]->FixedUpdate();
	}
}

void PhysicsSystem::SolveCollisions()
{
	int size = rigidbodies.GetSize();

	for (int i = 0; i < size; i++)
	{
		
	}
}

PhysicsSystem &PhysicsSystem::Instance()
{
	static PhysicsSystem instance;
	return instance;
}

void PhysicsSystem::Register(Rigidbody * rb)
{
	rigidbodies.Push(rb);
}

void PhysicsSystem::Register(const ICollider * col)
{
}
