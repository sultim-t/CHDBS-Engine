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

void Rigidbody::SolveCollisions(const CollisionInfo &info)
{
	ASSERT(this == info.RbThis);

	float penetration = info.Contact.Penetration;
	Vector3 normal = info.Contact.Normal.GetNormalized();
	Rigidbody *rbOther = info.RbOther;

	// position correction
	float slop = 0.01f;
	float percent = 0.2f;

	// if other is a static collider, then assume that its mass = Inf 
	float invMassOther = rbOther == nullptr ? 0.0f : rbOther->inversedMass;
	float invMassThis = this->inversedMass;



	Vector3 correction = normal * Max(penetration - slop, 0.0f) / (invMassThis + invMassOther) * percent;

	transform->Translate(correction * invMassThis);
	
	if (rbOther != nullptr)
	{
		rbOther->transform->Translate(correction * (-invMassOther));
	}



	Vector3 relativeVelocity = rbOther != nullptr ? this->velocity - rbOther->velocity : this->velocity;
	float contactVel = Vector3::Dot(relativeVelocity, normal);

	// velocities are separating
	if (contactVel > 0)
	{
		return;
	}

	// calculate restitution
	float restitution = info.GetRestitution();

	// calculate impulse scalar
	float i = -contactVel * (1.0f + restitution);
	i /= invMassThis + invMassOther;

	Vector3 impulse = normal * i;



	// inertia tensor
	float I = 1.0f / 32.0f; // hard coded for sphere

	// friction
	Vector3 tanDir = relativeVelocity - normal * (Vector3::Dot(relativeVelocity, normal));
	tanDir.Normalize();
	Vector3 r = info.Contact.Point - transform->GetPosition();
	Vector3 frictionImpulse = tanDir * (-info.GetStaticFriction());
	frictionImpulse /= inversedMass + 1.0f / 32.0f * Vector3::Dot(Vector3::Cross(Vector3::Cross(r, tanDir), r), tanDir);

	impulse += frictionImpulse;




	// if exist other rigidbody
	if (rbOther != nullptr)
	{
		// change its velocity
		rbOther->velocity -= impulse * invMassOther;
	}

	this->velocity += impulse * invMassThis;
}

const ICollider &Rigidbody::GetCollider() const
{
	return *collider;
}

#define PROPERTY_KEY_MASS		"Mass"
#define PROPERTY_KEY_VELOCITY	"Velocity"
#define PROPERTY_KEY_NOGRAVITY	"NoGravity"

#define PROPERTY_KEY_COLTYPE	"ColliderType"
#define PROPERTY_VAL_COL_AABB	"AABB"
#define PROPERTY_VAL_COL_SPHERE	"Sphere"

#define PROPERTY_KEY_COLOFFSET	"ColliderOffset"

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
	else
	{
		String s = "Wrong Rigidbody property: ";
		s += value;

		Logger::Print(s);
	}
}