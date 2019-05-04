#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/ResourceManager/ModelResource.h>
#include "AnimationKey.h"

class VertexAnimation
{
private:
	// Contains time values and path to models
	const VertexAnimatedResource *vertAnim;
	float animLength;

private:
	int GetModelIndex(float time);

public:
	void Init(const char *path);

	// Animate using given time, animated result will be in "tempVerts" array
	// "tempVerts" is an array of each vertex in each mesh
	void Animate(StaticArray<StaticArray<Vertex5>*> &tempVerts, float time);

	const VertexAnimatedResource *GetVertexAnimationResource() const;

	const String &GetName() const;
	// Get length in seconds
	float GetLength() const;
};
