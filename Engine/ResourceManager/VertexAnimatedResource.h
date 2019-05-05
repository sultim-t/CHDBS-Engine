#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Base/StringTuple.h>

typedef AnimationKey<const ModelResource*> VertexAnimationKey;

// Represents one vertex animation, contains loaded models
class VertexAnimatedResource
{
	friend class ResourceManager;

private:
	StaticArray<VertexAnimationKey> animationNodes;
	float duration;
	float ticksPerSecond;
	String name;

public:
	VertexAnimatedResource();
	// Default destructor
	~VertexAnimatedResource();

	const StaticArray<VertexAnimationKey> &GetAnimationNodes() const;
	const String &GetName() const;	
	
	// Get animation duration in ticks
	float GetDuration() const;
	// Get count of ticks per second
	float GetTicksPerSecond() const;
};

inline VertexAnimatedResource::VertexAnimatedResource()
	: ticksPerSecond(1.0f) { }

inline VertexAnimatedResource::~VertexAnimatedResource()
{ }

inline const StaticArray<VertexAnimationKey>& VertexAnimatedResource::GetAnimationNodes() const
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

inline float VertexAnimatedResource::GetTicksPerSecond() const
{
	return ticksPerSecond;
}
