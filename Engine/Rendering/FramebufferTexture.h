#pragma once

#include "Texture.h"

class FramebufferTexture: public ITexture
{
private:
	UINT fbo;

public:
	void Create(int width, int height);
	UINT GetFBO() const;
};