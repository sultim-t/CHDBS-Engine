#version 330 core

in vec2 TexCoords;
out vec4 OutColor;

uniform sampler2D TextTexture;
uniform vec3 TextColor;

void main()
{
	vec4 glyph = vec4(1.0, 1.0, 1.0, texture(TextTexture, TexCoords).r);
	OutColor = vec4(TextColor, 1.0) * glyph;
}