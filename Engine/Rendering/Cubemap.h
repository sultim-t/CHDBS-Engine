#pragma once
#include "Texture.h"

class Cubemap : public ITexture
{
public:
	Cubemap();

	void LoadCubemap(const StaticArray<const char*, 6> &paths);
	void Activate(int i) const override;
};