#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Base/StringTuple.h>

struct VertAnimNode
{
	// Path to model
	String	ModelPath;
	float	Time;
};

class VertexAnimatedResource
{
	friend class ResourceManager;

private:
	StaticArray<VertAnimNode> animationNodes;
	float duration;
	String name;

public:
	// Empty constructor
	VertexAnimatedResource();
	// Default destructor
	~VertexAnimatedResource();

	const StaticArray<VertAnimNode> &GetAnimationNodes() const;
	const String &GetName() const;
	float GetDuration() const;
};

inline VertexAnimatedResource::VertexAnimatedResource()
{ }

inline VertexAnimatedResource::~VertexAnimatedResource()
{ }

inline const StaticArray<VertAnimNode>& VertexAnimatedResource::GetAnimationNodes() const
{
	return animationNodes;
}

inline const String &VertexAnimatedResource::GetName() const
{
	return name;
}

inline float VertexAnimatedResource::GetDuration() const
{
	return duration;
}
