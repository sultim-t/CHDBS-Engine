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
};
