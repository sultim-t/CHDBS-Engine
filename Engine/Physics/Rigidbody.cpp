#include "Rigidbody.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include <Engine/Memory/Memory.h>
#include <Engine/Systems/PhysicsSystem.h>

CLASSDEFINITION(IComponent, Rigidbody)

void Rigidbody::Init()
{
	transform = &GetOwner().GetTransform();

	allForces.Init(8);
	allImpulses.Init(8);

	if (collider->GetColliderType() == ColliderType::Sphere)
	{
		((SphereCollider*)collider)->SetTransform(transform);
	}

	PhysicsSystem::Instance().Register(this);
}

Rigidbody::~Rigidbody()
{
	// free allocated memory
	if (collider != nullptr)
	{
		delete collider;
	}
}

void Rigidbody::AddForce(const Vector3 &force)
{
	allForces.Push(force);
}

void Rigidbody::AddImpulse(const Vector3 &impulse)
{
	allImpulses.Push(impulse);
}

void Rigidbody::FixedUpdate()
{
	if (mass == 0.0f)
	{
		return;
	}

	Vector3 force(0.0f, 0.0f, 0.0f);

	// add constant forces
	for (int i = 0; i < allForces.GetSize(); i++)
	{
		force += allForces[i];
	}
	
	// add impulses
	for (int i = 0; i < allImpulses.GetSize(); i++)
	{
		force += allImpulses[i];
	}

	// and clear list
	allImpulses.Clear();

	Vector3 acceleration = force / mass + PhysicsSystem::Gravity;
	velocity += acceleration * Time::GetFixedDeltaTime();
	Vector3 pos = transform->GetPosition() + velocity * Time::GetFixedDeltaTime();

	transform->GetPosition() = pos;
}

void Rigidbody::SolveCollisions(const CollisionInfo &info)
{
	velocity = Vector3::Reflect(velocity, info.Normal);
}

const ICollider & Rigidbody::GetCollider() const
{
	return *collider;
}

#define PROPERTY_KEY_MASS		"mass"
#define PROPERTY_KEY_VELOCITY	"velocity"
#define PROPERTY_KEY_COLTYPE	"colType"

#define PROPERTY_VAL_COL_AABB	"aabb"
#define PROPERTY_VAL_COL_SPHERE	"sphere"

#define PROPERTY_KEY_COLOFFSET	"offset"

// for spheres
#define PROPERTY_KEY_COLRADIUS	"colRadius"

// for aabbs
#define PROPERTY_KEY_COLMAX		"colMax"
#define PROPERTY_KEY_COLMIN		"colMin"

void Rigidbody::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_MASS)
	{
		mass = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_VELOCITY)
	{
		velocity = value.ToVector3();
	}
	else if (key == PROPERTY_KEY_COLTYPE)
	{
		// to create collider, current one must be empty
		ASSERT(collider == nullptr);
		
		if (value == PROPERTY_VAL_COL_AABB)
		{
			// allocate memory
			collider = new AABBCollider();
		}
		else if (value == PROPERTY_VAL_COL_SPHERE)
		{
			// allocate memory
			collider = new SphereCollider();
		}
		else
		{
			Logger::Print("Wrong collider type");
		}
	}
	else if (key == PROPERTY_KEY_COLOFFSET)
	{
		ASSERT(collider != nullptr);

		if (collider->GetColliderType() == ColliderType::AABB)
		{
			Vector3 offset = value.ToVector3();
			((AABBCollider*)collider)->GetAABB().Move(offset);
		}
		else if (collider->GetColliderType() == ColliderType::Sphere)
		{
			Vector3 center = value.ToVector3();
			((SphereCollider*)collider)->GetSphereRef().SetCenter(center);
		}
		else
		{
			Logger::Print("Rigidbody shape is not set");
		}
	}
	else if (key == PROPERTY_KEY_COLRADIUS)
	{
		ASSERT(collider != nullptr);
		ASSERT(collider->GetColliderType() == ColliderType::Sphere);

		((SphereCollider*)collider)->GetSphereRef().SetRadius(value.ToFloat());
	}
	else if (key == PROPERTY_KEY_COLMAX)
	{
		ASSERT(collider != nullptr);
		ASSERT(collider->GetColliderType() == ColliderType::AABB);
		
		Vector3 m = value.ToVector3();
		((AABBCollider*)collider)->GetAABB().SetMax(m);
	}
	else if (key == PROPERTY_KEY_COLMIN)
	{
		ASSERT(collider != nullptr);
		ASSERT(collider->GetColliderType() == ColliderType::AABB);

		Vector3 m = value.ToVector3();
		((AABBCollider*)collider)->GetAABB().SetMin(m);
	}
	else
	{
		String s = "Wrong Rigidbody property: ";
		s += value;

		Logger::Print(s);
	}
}