#pragma once
#include <Engine/Math/Vector.h>

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords; // uvs
};

struct Vertex5
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoords; // uvs
	Vector3 Tangent;
	Vector3 Bitangent;
};