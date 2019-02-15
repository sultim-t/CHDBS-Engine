#include "CParticleSystem.h"
#include <Engine/Memory/Memory.h>

CLASSDEFINITION(IComponent, CParticleSystem)

CParticleSystem::~CParticleSystem()
{
	SYSALLOCATOR.Free(particles);
}

void CParticleSystem::Init()
{
	particles = (Particle*)SYSALLOCATOR.Allocate(sizeof(Particle) * maxParticleCount);
}

void CParticleSystem::Update()
{
//	for (UINT i = 0)
}

void CParticleSystem::SetProperty(const String & key, const String & value)
{
}
