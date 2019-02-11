#pragma once

#include "Texture.h"

class FramebufferTexture: public Texture
{
private:
	UINT fbo;

public:
	void Create(INT width, INT height);
	UINT GetFBO();
};