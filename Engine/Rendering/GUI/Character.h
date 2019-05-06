#pragma once
#include <Engine/Math/Vector.h>

struct Chraracter
{
	UINT		TextureID;	// graphics texture id
	VectorI2	Size;
	VectorI2	Bearing;	// offset from baseline
	UINT		Advance;
};