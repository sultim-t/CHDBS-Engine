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

public:
	void Init();
	void LoadChars();

	// Render text
	void Draw(const char *string, float x, float y, float scale, const Color3F &color = Color3F(1.0f, 1.0f, 1.0f));

	static TextRenderer &Instance();
};