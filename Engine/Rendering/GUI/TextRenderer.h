#pragma once
#include "Character.h"
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Rendering/Shader.h>

class TextRenderer
{
private:
	const Shader	*textShader;

	int				textColorLoc;
	int				textProjectionLoc;
	int				textTextureLoc;

	UINT			charVao;
	UINT			charVbo;
	
	StaticArray<Chraracter>	chars;

private:
	void LoadChars(const char *fontPath);

public:
	void Init(const char *fontPath);

	// Render text
	void Draw(const char *string, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, const Color3F &color = Color3F(1.0f, 1.0f, 1.0f));

	static TextRenderer &Instance();
};