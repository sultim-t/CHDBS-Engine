#pragma once

#include "AnimationNode.h"
#include <Engine/DataStructures/StaticArray.h>

class Animation
{
private:
	float duration;
	float ticksPerSecond;

	StaticArray<AnimationNode*> animationNodes;

	// String name;

public:
	inline Animation(float duration, float ticksPerSecond, const StaticArray<AnimationNode*> &animationNodes);
	inline ~Animation();

	inline float GetDuration() const;
	inline float GetTicksPerSecond() const;
	inline const StaticArray<AnimationNode*> &GetAnimationNodes() const;
};

inline Animation::Animation(float duration, float ticksPerSecond, const StaticArray<AnimationNode*>& animationNodes)
{
	ASSERT(duration >= 0.0f && ticksPerSecond >= 0.0f);

	this->duration = duration;
	this->ticksPerSecond = ticksPerSecond;
	this->animationNodes = animationNodes;
}

inline Animation::~Animation()
{
	for (UINT i = 0; i < animationNodes.GetSize(); i++)
	{
		delete animationNodes[i];
	}
}

inline float Animation::GetDuration() const
{
	return duration;
}

inline float Animation::GetTicksPerSecond() const
{
	return ticksPerSecond;
}

inline const StaticArray<AnimationNode*>& Animation::GetAnimationNodes() const
{
	return animationNodes;
}
