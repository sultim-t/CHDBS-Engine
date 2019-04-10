#include "Rigidbody.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include <Engine/Memory/Memory.h>
#include <Engine/Systems/PhysicsSystem.h>

#include <Engine/Base/Random.h>

CLASSDEFINITION(IComponent, Rigidbody)

void Rigidbody::Init()
{
	transform = &GetOwner().GetTransform();

	acceleration = Vector3(0.0f);

	allForces.Init(8);
	allImpulses.Init(8);

	if (collider->GetColliderType() == ColliderType::Sphere)
	{
		((SphereCollider*)collider)->SetTransform(transform);
	}
	else if(collider->GetColliderType() == ColliderType::AABB)
	{
		((AABBCollider*)collider)->SetTransform(transform);
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

void Rigidbody::RemoveAllForces()
{
	allForces.Clear();
}

Vector3 &Rigidbody::GetVelocity()
{
	return velocity;
}

const Vector3 &Rigidbody::GetVelocity() const
{
	return velocity;
}

const Vector3 &Rigidbody::GetAcceleration() const
{
	return acceleration;
}

float Rigidbody::GetMass()
{
	return 1.0f / inversedMass;
}

float Rigidbody::GetInversedMass()
{
	return inversedMass;
}

void Rigidbody::SetMass(float mass)
{
	ASSERT(mass != 0.0f);
	inversedMass = 1.0f / mass;
}

void Rigidbody::FixedUpdate()
{
	// update position
	transform->GetPosition() += velocity * Time::GetFixedDeltaTime();

	Vector3 force(0.0f);

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

	acceleration = force * inversedMass;

	if (!NoGravity)
	{
		acceleration += PhysicsSystem::Gravity;
	}

	velocity += acceleration * Time::GetFixedDeltaTime();
}

const ICollider &Rigidbody::GetCollider() const
{
	return *collider;
}

#define PROPERTY_KEY_MASS		"Mass"
#define PROPERTY_KEY_VELOCITY	"Velocity"
#define PROPERTY_KEY_NOGRAVITY	"NoGravity"

// Colliders
#define PROPERTY_KEY_COLTYPE	"ColliderType"
#define PROPERTY_VAL_COL_AABB	"AABB"
#define PROPERTY_VAL_COL_SPHERE	"Sphere"

#define PROPERTY_KEY_COLOFFSET	"ColliderOffset"

#define PROPERTY_KEY_COLSTATICFRICTION	"StaticFriction"
#define PROPERTY_KEY_COLDYNAMICFRICTION	"DynamicFriction"
#define PROPERTY_KEY_COLFRICTCOMBINE	"FrictionCombine"

#define PROPERTY_KEY_COLRESTITUTION		"Restitution"
#define PROPERTY_KEY_COLRESTITCOMBINE	"RestitutionCombine"

#define PROPERTY_KEY_COLCOMBINEAVERAGE	"CombineAverage"
#define PROPERTY_KEY_COLCOMBINEMIN		"CombineMinimum"
#define PROPERTY_KEY_COLCOMBINEMAX		"CombineMaximum"
#define PROPERTY_KEY_COLCOMBINEMULTIPLY	"CombineMultiply"

// for spheres
#define PROPERTY_KEY_COLRADIUS	"SphereRadius"

// for aabbs
#define PROPERTY_KEY_COLMAX		"AABBMax"
#define PROPERTY_KEY_COLMIN		"AABBMin"

void Rigidbody::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_MASS)
	{
		inversedMass = 1.0f / value.ToFloat();
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
			((AABBCollider*)collider)->GetAABBRef().Move(offset);
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
		((AABBCollider*)collider)->GetAABBRef().SetMax(m);
	}
	else if (key == PROPERTY_KEY_COLMIN)
	{
		ASSERT(collider != nullptr);
		ASSERT(collider->GetColliderType() == ColliderType::AABB);

		Vector3 m = value.ToVector3();
		((AABBCollider*)collider)->GetAABBRef().SetMin(m);
	}
	else if (key == PROPERTY_KEY_NOGRAVITY)
	{
		NoGravity = true;
	}
	else if (key == PROPERTY_KEY_COLSTATICFRICTION)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetStaticFriction(value.ToFloat());
	}
	else if (key == PROPERTY_KEY_COLDYNAMICFRICTION)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetDynamicFriction(value.ToFloat());
	}
	else if (key == PROPERTY_KEY_COLFRICTCOMBINE)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetFrictionCombine(GetCombineOption(value));
	}
	else if (key == PROPERTY_KEY_COLRESTITUTION)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetRestitution(value.ToFloat());
	}
	else if (key == PROPERTY_KEY_COLRESTITCOMBINE)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetRestitutionCombine(GetCombineOption(value));
	}
	else
	{
		String s = "Wrong Rigidbody property: ";
		s += value;

		Logger::Print(s);
	}
}

PhysicMaterialCombine Rigidbody::GetCombineOption(const String & value)
{
	if (value == PROPERTY_KEY_COLCOMBINEAVERAGE)
	{
		return PhysicMaterialCombine::Average;
	}
	else if (value == PROPERTY_KEY_COLCOMBINEMIN)
	{
		return PhysicMaterialCombine::Minimum;
	}
	else if (value == PROPERTY_KEY_COLCOMBINEMAX)
	{
		return PhysicMaterialCombine::Maximum;
	}
	else if (value == PROPERTY_KEY_COLCOMBINEMULTIPLY)
	{
		return PhysicMaterialCombine::Multiply;
	}

	ASSERT(0);
}