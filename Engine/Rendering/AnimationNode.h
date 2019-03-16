#pragma once

#include "AnimationKey.h"
#include <Engine/DataStructures/StaticArray.h>

class AnimationNode
{
private:
	StaticArray<AKeyPosition>	positionKeys;
	StaticArray<AKeyRotation>	rotationKeys;
	StaticArray<AKeyScale>		scaleKeys;

public:
	inline AnimationNode(
		const StaticArray<AKeyPosition>	&positionKeys,
		const StaticArray<AKeyRotation>	&rotationKeys,
		const StaticArray<AKeyScale>	&scaleKeys);
};

inline AnimationNode::AnimationNode(const StaticArray<AKeyPosition>& positionKeys, const StaticArray<AKeyRotation>& rotationKeys, const StaticArray<AKeyScale>& scaleKeys)
{
	this->positionKeys	= positionKeys;
	this->rotationKeys	= rotationKeys;
	this->scaleKeys		= scaleKeys;
}