#include "TextRenderer.h"
#include <Engine/Math/Projection.h>
#include <Engine/ResourceManager/GlobalSettings.h>
#include <Engine/Rendering/OpenGL.h>

#include <ft2build.h>
#include FT_FREETYPE_H

void TextRenderer::Init()
{
	textShader = Shader::FindShader(SHADER_NAME_TEXT);
	textColorLoc = textShader->GetUniformLocation("TextColor");
	textProjectionLoc = textShader->GetUniformLocation("Projection");
	textTextureLoc = textShader->GetUniformLocation("TextTexture");

	chars.Init(128);
	LoadChars();

	glGenVertexArrays(1, &charVao);
	glGenBuffers(1, &charVbo);
	glBindVertexArray(charVao);
	glBindBuffer(GL_ARRAY_BUFFER, charVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::LoadChars()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		Logger::Print("Freetype::Can't init");
	}

	FT_Face face;
	if (FT_New_Face(ft, "Fonts\\Roboto-Regular.ttf", 0, &face))
	{
		Logger::Print("Freetype::Can't load font");
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Logger::Print("Freetype::Can't load glyph");
			continue;
		}

		UINT texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		chars[c].TextureID = texture;
		chars[c].Size[0] = face->glyph->bitmap.width;
		chars[c].Size[1] = face->glyph->bitmap.rows;
		chars[c].Bearing[0] = face->glyph->bitmap_left;
		chars[c].Bearing[1] = face->glyph->bitmap_top;
		chars[c].Advance = face->glyph->advance.x;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::Draw(const char * string, float x, float y, float scale, const Color3F & color)
{
	const Matrix4 &projection = Projection::Ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textShader->Use();
	textShader->SetVec3(textColorLoc, color);
	textShader->SetMat4(textProjectionLoc, projection);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(charVao);

	int i = 0;
	while (string[i] != '\0')
	{
		const Chraracter &c = chars[(int)string[i]];

		float cx = x + c.Bearing[0] * scale;
		float cy = y -(c.Size[1] - c.Bearing[1]) * scale;

		float w = c.Size[0] * scale;
		float h = c.Size[1] * scale;

		float vertices[6][4] = {
			{ cx,     cy + h,   0.0f, 0.0f },
			{ cx,     cy,       0.0f, 1.0f },
			{ cx + w, cy,       1.0f, 1.0f },

			{ cx,     cy + h,   0.0f, 0.0f },
			{ cx + w, cy,       1.0f, 1.0f },
			{ cx + w, cy + h,   1.0f, 0.0f }
		};

		textShader->SetInt(textTextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, c.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, charVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (c.Advance >> 6) * scale;

		i++;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

TextRenderer &TextRenderer::Instance()
{
	static TextRenderer instance;
	return instance;
}
