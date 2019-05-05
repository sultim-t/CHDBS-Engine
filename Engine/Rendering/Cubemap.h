#pragma once
#include "ITexture.h"
#include <Engine/DataStructures/StaticArray.h>

class Cubemap : public ITexture
{
public:
	// Default constructor
	Cubemap();

	// Init cubemap from 6 files.
	// Order: right, left, top, bottom, front, back
	void LoadCubemap(const StaticArray<String> &sidesNames);

	// Activate this cubemap
	void Activate(int i) const override;
	// Deactivate this cubemap
	void Deactivate() const override;
};