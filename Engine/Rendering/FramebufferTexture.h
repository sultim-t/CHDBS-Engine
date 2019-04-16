#pragma once
#include "ITexture.h"

class FramebufferTexture: public ITexture
{
private:
	// Current id
	UINT fbo;

public:
	// Register framebuffer
	void Create(int width, int height);
	// Get framebuffer ID
	UINT GetFBO() const;

	// Activate this framebffer texture
	virtual void Activate(int i) const override;
};