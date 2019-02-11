#pragma once
#include "Texture.h"

class Cubemap : public Texture
{
public:
	void LoadCubemap(const StaticArray<const char*> &paths);
	void Activate(INT i) const override;
};