#include "Rigidbody.h"

void Rigidbody::Init()
{
	transform = GetOwner().GetTransform();

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

void Rigidbody::FixedUpdate()
{
	if (mass == 0.0f)
	{
		return;
	}

	Vector3 force(0.0f, 0.0f, 0.0f);

	// add constant forces
	for (UINT i = 0; i < allForces.GetSize(); i++)
	{
		force += allForces[i];
	}
	
	// add impulses
	for (UINT i = 0; i < allImpulses.GetSize(); i++)
	{
		force += allImpulses[i];
	}

	// and clear list
	allImpulses.Clear();

	Vector3 acceleration = force / mass;
	velocity += acceleration * Time::GetFixedDeltaTime();
	transform.GetPosition() += velocity * Time::GetFixedDeltaTime();
}