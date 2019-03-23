#include "PhysicsSystem.h"
#include <Engine/Math/Intersection.h>

Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

void PhysicsSystem::Init()
{
	rigidbodies.Init(128);
	colliders.Init(128);
	collisions.Init(128);
}

void PhysicsSystem::Update()
{
	int size = rigidbodies.GetSize();

	for (int i = 0; i < size; i++)
	{
		rigidbodies[i]->FixedUpdate();
	}
	
	GetApproximateCollisions();
	SolveCollisions();
}

void PhysicsSystem::GetApproximateCollisions()
{	
	// approximate spape to use
	typedef Sphere AShape;
	// intersect function for approximate shapes
	bool(*AIntersect)(const AShape&, const AShape&) = Intersection::SphereSphere;

	int dynamicCount = rigidbodies.GetSize();
	int staticCount = colliders.GetSize();

	// for each rigidbody
	for (int i = 0; i < dynamicCount; i++)
	{
		// get data from this rigidbody
		Rigidbody *rbThis = rigidbodies[i];
		const ICollider &colThis = rbThis->GetCollider();
		AShape &boundingThis = colThis.GetBoundingSphere();

		// create collision between rigidbodies
		auto info = BroadCollisionInfo(CollisionType::Rigidbodies);
		info.CollThis = &colThis;
		info.RbThis = rbThis;

		// for each other rigidbody
		for (int j = dynamicCount - 1; j > i; j++)
		{
			Rigidbody *rbOther = rigidbodies[i];
			const ICollider &colOther = rbOther->GetCollider();
		
			// get approximate shape
			AShape &boundingOther = colOther.GetBoundingSphere();

			if (!AIntersect(boundingThis, boundingOther))
			{
				// ignore non intersected
				continue;
			}

			info.RbOther = rbOther;
			info.CollOther = &colOther;

			// add info to process
			collisions.Push(info);
		}
		
		// create collision between dynamic and static
		info.Type = CollisionType::RigidbodyStatic;

		// for each static collider
		for (int j = 0; j < staticCount; j++)
		{
			const ICollider *colOther = colliders[j];

			// get approximate shape
			AShape &boundingOther = colOther->GetBoundingSphere();

			if (!AIntersect(boundingThis, boundingOther))
			{
				// ignore non intersected
				continue;
			}

			info.CollOther = colOther;

			// add info to process
			collisions.Push(info);
		}
	}
}

void PhysicsSystem::SolveCollisions()
{
	int collisionCount = collisions.GetSize();
	
	// for each collision, detected in broad phase
	for (int i = 0; i < collisionCount; i++)
	{
		BroadCollisionInfo &current = collisions[i];

		// there must be rigidbody and colliders
		ASSERT(current.RbThis != nullptr);
		ASSERT(current.CollThis != nullptr);
		ASSERT(current.CollOther != nullptr);

		// copy from broad phase
		CollisionInfo info = CollisionInfo(current);

		// get actual collision info
		if (current.CollThis->Intersect(*current.CollOther, info))
		{
			// solve collision, type is processed there
			current.RbThis->SolveCollisions(info);
		}
	}

	// clear collisions from this frame
	collisions.Clear();
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

bool PhysicsSystem::Raycast(const Vector3 & pos, const Vector3 & dir, const float distance, RaycastInfo & info)
{
	return false;
}

bool PhysicsSystem::Raycast(const Vector3 & pos, const Vector3 & dir, RaycastInfo & info)
{
	return false;
}