#pragma once

#include "AnimationKey.h"
#include <Engine/DataStructures/StaticArray.h>

class AnimationNode
{
private:
	StaticArray<AKeyPosition>	positionKeys;
	StaticArray<AKeyRotation>	rotationKeys;
	StaticArray<AKeyScale>		scaleKeys;
};