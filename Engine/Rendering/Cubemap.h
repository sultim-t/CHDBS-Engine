#pragma once
#include "Texture.h"

class Cubemap : public Texture
{
public:
	void LoadCubemap(const StaticArray<const char*, 6> &paths);
	void Activate(INT i) const override;
};