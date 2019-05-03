#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/ResourceManager/ModelResource.h>
#include "AnimationKey.h"

typedef AnimationKey<const ModelResource*> VertexAnimationKey;

class VertexAnimation
{
	// temp public
public:
	String name;
	float duration;
	float ticksPerSecond;

	// All pairs model-time in this animation
	StaticArray<VertexAnimationKey> keys;

private:
	int GetModelIndex(float time);

public:
	// Temp contructor
	VertexAnimation() {}


	VertexAnimation(const char *name, float duration, float ticksPerSecond = 30.0f);
	// Empty destructor
	~VertexAnimation();

	void Init(int keyCount);

	// Animate using given time, animated result will be in "tempVerts" array
	// "tempVerts" is an array of each vertex in each mesh
	void Animate(StaticArray<StaticArray<Vertex5>*> &tempVerts, float time);
};
