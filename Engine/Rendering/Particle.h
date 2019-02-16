#pragma once

#include <Engine/Math/Vector.h>

struct Particle
{
	Vector3	position;
	Vector3	velocity;
	Color4	color;
	// Time from emitting
	float	life;
	float	rotation;
	float	size;
	// Distance from camera to sort
	float	camDistance;

	// For sorting before redendering
	bool operator<(const Particle &p) const
	{
		// reversed, closer particles are rendered last
		return this->camDistance > p.camDistance;
	}
};