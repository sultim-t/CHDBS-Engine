#pragma once
#include "Texture.h"

class Cubemap : public Texture
{
public:
	template<UINT N>
	void LoadCubemap(const StaticArray<const char*, N> &paths);
	void Activate(INT i) const override;
};