#include "Particle.h"

Particle::Particle()
{}

void Particle::Clear()
{
	position = velocity = Vector3(0.0f);
	gravityMultiplier = rotation = scale = 0.0f;
	lifetime = 1.0f;
}

bool Particle::Update()
{
	Vector3 gravity(0.0f, 9.8f, 0.0f);
	velocity -= gravity * gravityMultiplier;
	position += velocity * Time::GetDeltaTime();

	elapsedTime += Time::GetDeltaTime();
	
	return elapsedTime < lifetime;
}
