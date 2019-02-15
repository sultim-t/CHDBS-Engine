#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Rendering/Particle.h>

class CParticleSystem : public IComponent
{
	CLASSDECLARATION(CParticleSystem)

public:
	UINT maxParticleCount;

	Vector3 startVelocity;
	float velocityRandomness;

	float startLifetime;
	float lifetimeRandomness;

	bool isLooping;
	float emitRate;

	// array of particles
	Particle *particles;

public:
	~CParticleSystem();

	// Start emit particles with current emit rate
	void Start();
	// Emit particles
	void Emit(UINT count);

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};