#include "Rigidbody.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

#include <Engine/Memory/Memory.h>
#include <Engine/Systems/PhysicsSystem.h>

#include <Engine/Components/CBoxCollider.h>
#include <Engine/Components/CSphereCollider.h>
#include <Engine/Components/CMeshCollider.h>

#include <Engine/Base/Random.h>

CLASSDEFINITION(IComponent, Rigidbody)

void Rigidbody::Init()
{
	acceleration = Vector3(0.0f);

	// allocate memory
	allForces.Init(8);
	allImpulses.Init(8);
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
	GetOwner().GetTransform().GetPosition() += velocity * Time::GetFixedDeltaTime();

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

const ICollider *Rigidbody::GetCollider() const
{
	const ICollider *box = (ICollider*)GetOwner().GetComponent<CBoxCollider>();
	const ICollider *sphere = (ICollider*)GetOwner().GetComponent<CSphereCollider>();

	if (box != nullptr)
	{
		return box;
	}
	else if (sphere != nullptr)
	{
		return sphere;
	}

	return nullptr;
}

#define PROPERTY_KEY_MASS		"Mass"
#define PROPERTY_KEY_VELOCITY	"Velocity"
#define PROPERTY_KEY_NOGRAVITY	"NoGravity"

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
	else if (key == PROPERTY_KEY_NOGRAVITY)
	{
		NoGravity = value.ToBool();
	}
	else
	{
		String s = "Wrong Rigidbody property: ";
		s += value;

		Logger::Print(s);
	}
}
