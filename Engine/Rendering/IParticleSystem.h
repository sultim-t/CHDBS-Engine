#pragma once
#include <Engine/Rendering/ICamera.h>

class IParticleSystem
{
public:
	virtual void BindCamera(const ICamera *cam) = 0;
	virtual void Render() = 0;
};