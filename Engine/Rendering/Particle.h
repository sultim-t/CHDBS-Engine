#pragma once

#include <Engine/Math/Vector.h>

struct Particle
{
	Vector3 position;
	Vector3 velocity;
	float gravityMultiplier;
	float lifetime;
	float rotation;
	float scale;
	float elapsedTime;
	float flags[5];

public:
	// Empty constructor
	Particle();
	
	// Returns to default
	void Clear();

	// Update position, velocity
	// Returns true if particle is alive
	bool Update();
};