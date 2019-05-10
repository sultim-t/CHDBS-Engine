#pragma once

#include "AnimationNode.h"
#include <Engine/DataStructures/StaticArray.h>

class Animation
{
	friend class ResourceManager;

private:
	float duration;
	float ticksPerSecond;

	StaticArray<AnimationNode*> animationNodes;

	String name;

public:
	inline Animation(const char *name, float duration, float ticksPerSecond, int animNodesCount);
	inline ~Animation();

	inline float GetDuration() const;
	inline float GetTicksPerSecond() const;
	inline const String &GetName() const;
	inline const StaticArray<AnimationNode*> &GetAnimationNodes() const;

	// Find animation node with specified name
	inline const AnimationNode *FindAnimationNode(const char *name) const;
};

inline Animation::Animation(const char *name, float duration, float ticksPerSecond, int animNodesCount)
{
	ASSERT(duration >= 0.0f && ticksPerSecond >= 0.0f);

	this->name = name;

	this->duration = duration;
	this->ticksPerSecond = ticksPerSecond;

	this->animationNodes.Init(animNodesCount);
}

inline Animation::~Animation()
{
	for (UINT i = 0; i < animationNodes.GetSize(); i++)
	{
		delete animationNodes[i];
	}
}

inline const String &Animation::GetName() const
{
	return name;
}

inline float Animation::GetDuration() const
{
	return duration;
}

inline float Animation::GetTicksPerSecond() const
{
	return ticksPerSecond;
}

inline const StaticArray<AnimationNode*> &Animation::GetAnimationNodes() const
{
	return animationNodes;
}

inline const AnimationNode *Animation::FindAnimationNode(const char *name) const
{
	for (UINT i = 0; i < animationNodes.GetSize(); i++)
	{
		if (animationNodes[i]->GetNodeName() == name)
		{
			return animationNodes[i];
		}
	}

	return nullptr;
}
