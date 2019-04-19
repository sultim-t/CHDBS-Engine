#include "PhysicsSystem.h"
#include <Engine/Math/Intersection.h>
#include <Engine/Rendering/DebugDrawer.h>

Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

void PhysicsSystem::Init()
{
	//rigidbodies.Init(128);
	//colliders.Init(128);
	collisions.Init(128);
}

void PhysicsSystem::Update()
{
	if (rigidbodies == nullptr)
	{
		return;
	}

	int size = rigidbodies->GetSize();

	for (int i = 0; i < size; i++)
	{
		rigidbodies->operator[](i)->FixedUpdate();
	}
	
	GetApproximateCollisions();
	SolveCollisions();
}

PhysicsSystem &PhysicsSystem::Instance()
{
	static PhysicsSystem instance;
	return instance;
}

//void PhysicsSystem::Register(Rigidbody * rb)
//{
//	rigidbodies.Push(rb);
//}
//
//void PhysicsSystem::Register(ICollider * col)
//{
//	colliders.Push(col);
//}

void PhysicsSystem::Register(const DynamicArray<Rigidbody*>* rigidbodies, const DynamicArray<ICollider*>* colliders)
{
	this->rigidbodies = rigidbodies;
	this->colliders = colliders;
}

void PhysicsSystem::Reset()
{
	// reset pointers to null
	this->rigidbodies = nullptr;
	this->colliders = nullptr;
}

bool PhysicsSystem::Raycast(const Vector3 & pos, const Vector3 & dir, const float distance, RaycastInfo & info)
{
	return false;
}

bool PhysicsSystem::Raycast(const Vector3 & pos, const Vector3 & dir, RaycastInfo & info)
{
	return false;
}

void PhysicsSystem::GetApproximateCollisions()
{	
	// approximate shape to use
	typedef Sphere AShape;
	// intersect function for approximate shapes
	bool(*AIntersect)(const AShape&, const AShape&) = Intersection::SphereSphere;

	int dynamicCount = rigidbodies->GetSize();
	int staticCount = colliders->GetSize();

	// for each rigidbody
	for (int i = 0; i < dynamicCount; i++)
	{
		// get data from this rigidbody
		Rigidbody *rbThis = rigidbodies->operator[](i);
		const ICollider *colThis = rbThis->GetCollider();
		AShape &boundingThis = colThis->GetBoundingSphere();

		// create collision between rigidbodies
		BroadCollisionInfo info = BroadCollisionInfo(CollisionType::Rigidbodies);
		info.CollThis = colThis;
		info.RbThis = rbThis;

		// for each other rigidbody
		for (int j = dynamicCount - 1; j > i; j--)
		{
			Rigidbody *rbOther = rigidbodies->operator[](j);
			const ICollider *colOther = rbOther->GetCollider();
		
			// get approximate shape
			AShape &boundingOther = colOther->GetBoundingSphere();

			if (!AIntersect(boundingThis, boundingOther))
			{
				// ignore non intersected
				continue;
			}

			info.RbOther = rbOther;
			info.CollOther = colOther;

			// add info to process
			collisions.Push(info);
		}
		
		// create collision between dynamic and static
		BroadCollisionInfo infoStatic = BroadCollisionInfo(CollisionType::RigidbodyStatic);
		infoStatic.CollThis = colThis;
		infoStatic.RbThis = rbThis;

		// for each static collider
		for (int j = 0; j < staticCount; j++)
		{
			const ICollider *colOther = colliders->operator[](j);

			// get approximate shape
			AShape &boundingOther = colOther->GetBoundingSphere();

			if (!AIntersect(boundingThis, boundingOther))
			{
				// ignore non intersected
				continue;
			}

			infoStatic.CollOther = colOther;

			// add info to process
			collisions.Push(infoStatic);
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
			SolveCollision(info);
		}
	}

	// clear collisions from this frame
	collisions.Clear();
}

void PhysicsSystem::SolveCollision(const CollisionInfo &info)
{
	ASSERT(info.RbThis != nullptr);

	float penetration = info.Contact.Penetration;
	Vector3 normal = info.Contact.Normal.GetNormalized();

	Rigidbody *rbThis = info.RbThis;
	Rigidbody *rbOther = info.RbOther;

	// if other is a static collider, then assume that its mass = Inf 
	float invMassOther = rbOther == nullptr ? 0.0f : rbOther->inversedMass;
	float invMassThis = rbThis->inversedMass;

	// apply position correction
	ApplyPositionCorrection(rbThis, rbOther, invMassThis, invMassOther, normal, penetration);

	Vector3 relativeVelocity = rbOther != nullptr ? 
		rbThis->velocity - rbOther->velocity : 
		rbThis->velocity;

	float contactVel = Vector3::Dot(relativeVelocity, normal);

	// velocities are separating
	if (contactVel > 0)
	{
		return;
	}

	// calculate restitution
	float restitution = info.GetRestitution();

	// calculate impulse scalar
	float impulse = -contactVel * (1.0f + restitution);
	impulse /= invMassThis + invMassOther;

	// calculate impulse vector
	Vector3 impulseVec = normal * impulse;

	// apply friction impulse
	impulseVec += CalculateFriction(relativeVelocity, normal, invMassThis, impulse, info.GetStaticFriction(), info.GetDynamicFriction());

	// if exist other rigidbody
	if (rbOther != nullptr)
	{
		// change its velocity
		rbOther->velocity -= impulseVec * invMassOther;
	}

	rbThis->velocity += impulseVec * invMassThis;
}

void PhysicsSystem::ApplyPositionCorrection(Rigidbody *rbThis, Rigidbody *rbOther, float invMassThis, float invMassOther,const Vector3 &normal, float penetration)
{	
	float slop = 0.01f;
	float percent = 0.2f;

	// position correction
	Vector3 correction = normal * Max(penetration - slop, 0.0f) / (invMassThis + invMassOther) * percent;

	// change positions
	rbThis->GetOwner().GetTransform().Translate(correction * invMassThis);

	if (rbOther != nullptr)
	{
		rbOther->GetOwner().GetTransform().Translate(correction * (-invMassOther));
	}
}

Vector3 PhysicsSystem::CalculateFriction(const Vector3 &relativeVelocity, const Vector3 &normal, float invMass, float impulse, float staticFriction, float dynamicFriction)
{	
	// get tangent vector
	Vector3 tanDir = relativeVelocity - normal * (Vector3::Dot(relativeVelocity, normal));
	float tanLength = tanDir.Length();

	if (tanLength == 0.0f)
	{
		return Vector3(0.0f);
	}

	// normalize
	tanDir /= tanLength;

	float frictionImpulse = -Vector3::Dot(relativeVelocity, tanDir);
	frictionImpulse /= invMass;

	// calculate friction impulse vector
	if (Abs(frictionImpulse) < impulse * staticFriction)
	{
		return tanDir * frictionImpulse;
	}
	else
	{
		return tanDir * (-impulse) * dynamicFriction;
	}
}