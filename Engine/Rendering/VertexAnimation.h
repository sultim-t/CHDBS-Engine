#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/ResourceManager/ModelResource.h>
#include "AnimationKey.h"

enum class AnimationWrapType
{
	Once,
	Loop
};

// Represents one vertex animation, ready for animating
class VertexAnimation
{
private:
	// Contains time values and path to models
	const VertexAnimatedResource *vertAnim;
	float animLength;

public:
	AnimationWrapType WrapType;

private:
	int GetModelIndex(float time) const;

public:
	// Sets default values
	VertexAnimation();

	void Init(const char *path);

	// Animate using given time, animated result will be in "tempVerts" array
	// "tempVerts" is an array of each vertex in each mesh
	bool Animate(StaticArray<StaticArray<Vertex5>*> &tempVerts, float time) const;

	// Get key models in this animation according to the time
	// Returns false, if no animation
	bool GetModels(float time, const ModelResource* &prevModel, const ModelResource* &nextModel, float &interpFactor) const;

	const VertexAnimatedResource *GetVertexAnimationResource() const;

	const String &GetName() const;
	// Get length in seconds
	float GetLength() const;
};
