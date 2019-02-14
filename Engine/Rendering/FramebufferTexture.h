#pragma once

#include "Texture.h"

class FramebufferTexture: public ITexture
{
private:
	UINT fbo;

public:
	void Create(INT width, INT height);
	void Activate(INT i) const override;
	UINT GetFBO();
};