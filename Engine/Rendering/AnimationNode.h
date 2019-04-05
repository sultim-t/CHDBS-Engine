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

	const StaticArray<AKeyPosition>	&GetPositionKeys() const;
	const StaticArray<AKeyRotation>	&GetRotationKeys() const;
	const StaticArray<AKeyScale>	&GetScaleKeys() const;

	bool GetInterpolatedPosition(float t, Vector3 &outPosition) const;
	bool GetInterpolatedRotation(float t, Quaternion &outRotation) const;
	bool GetInterpolatedScale(float t, Vector3 &outScale) const;
};

inline AnimationNode::AnimationNode(const StaticArray<AKeyPosition>& positionKeys, const StaticArray<AKeyRotation>& rotationKeys, const StaticArray<AKeyScale>& scaleKeys)
{
	this->positionKeys	= positionKeys;
	this->rotationKeys	= rotationKeys;
	this->scaleKeys		= scaleKeys;
}

inline const StaticArray<AKeyPosition>& AnimationNode::GetPositionKeys() const
{
	return positionKeys;
}

inline const StaticArray<AKeyRotation>& AnimationNode::GetRotationKeys() const
{
	return rotationKeys;
}

inline const StaticArray<AKeyScale>& AnimationNode::GetScaleKeys() const
{
	return scaleKeys;
}

inline bool AnimationNode::GetInterpolatedPosition(float t, Vector3 & outPosition) const
{
	UINT size = positionKeys.GetSize();

	if (size == 0)
	{
		return false;
	}

	for (UINT i = 0; i < size - 1; i++)
	{
		const AKeyPosition &prev = positionKeys[i];
		const AKeyPosition &next = positionKeys[i + 1];

		if (prev.Time >= t && t < next.Time)
		{
			outPosition = Vector3::Lerp(prev.Value, next.Value, (t - prev.Time) / (next.Time - prev.Time));
			return true;
		}
	}

	return false;
}

inline bool AnimationNode::GetInterpolatedRotation(float t, Quaternion & outRotation) const
{
	UINT size = rotationKeys.GetSize();

	if (size == 0)
	{
		return false;
	}

	for (UINT i = 0; i < size - 1; i++)
	{
		const AKeyRotation &prev = rotationKeys[i];
		const AKeyRotation &next = rotationKeys[i + 1];

		if (prev.Time >= t && t < next.Time)
		{
			outRotation = Quaternion::Lerp(prev.Value, next.Value, (t - prev.Time) / (next.Time - prev.Time));
			return true;
		}
	}

	return false;
}

inline bool AnimationNode::GetInterpolatedScale(float t, Vector3 & outScale) const
{
	UINT size = scaleKeys.GetSize();

	if (size == 0)
	{
		return false;
	}

	for (UINT i = 0; i < size - 1; i++)
	{
		const AKeyScale &prev = scaleKeys[i];
		const AKeyScale &next = scaleKeys[i + 1];

		if (prev.Time >= t && t < next.Time)
		{
			outScale = Vector3::Lerp(prev.Value, next.Value, (t - prev.Time) / (next.Time - prev.Time));
			return true;
		}
	}

	return false;
}
