#include "PhysicsSystem.h"

Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

void PhysicsSystem::Init()
{
	rigidbodies.Init(128);
	colliders.Init(128);
	//collisions.Init(512);
}

void PhysicsSystem::Update()
{
	int size = rigidbodies.GetSize();

	for (int i = 0; i < size; i++)
	{
		rigidbodies[i]->FixedUpdate();
	}

	SolveCollisions();
}

void PhysicsSystem::SolveCollisions()
{
	int size = rigidbodies.GetSize();

	for (int i = 0; i < size; i++)
	{
		for (int j = size - 1; j >= 0 && i != j; j++)
		{
			CollisionInfo info;
			if (rigidbodies[i]->GetCollider().Intersect(rigidbodies[j]->GetCollider(), info))
			{
				rigidbodies[i]->SolveCollisions(info);
				//collisions.Push(info);
			}
		}

		int staticSize = colliders.GetSize();
		for (int j = 0; j < staticSize; j++)
		{
			CollisionInfo info;
			if (rigidbodies[i]->GetCollider().Intersect(*colliders[j], info))
			{
				rigidbodies[i]->SolveCollisions(info);
				//collisions.Push(info);
			}
		}
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

void PhysicsSystem::Register(ICollider * col)
{
	colliders.Push(col);
}
