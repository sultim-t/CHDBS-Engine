#pragma once
#include "Texture.h"

class Cubemap : public ITexture
{
public:
	Cubemap();

	// Init cubemap from 6 files.
	// Order: right, left, top, bottom, front, back
	void LoadCubemap(const Array<const char*, 6> &paths);
	void ActivateCubemap(int index) const;
};