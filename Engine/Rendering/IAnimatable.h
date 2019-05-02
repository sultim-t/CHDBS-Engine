#pragma once

class IAnimatable
{
public:
	// Play animation with given index
	virtual void PlayAnimation(int animationIndex, float startTime = 0.0f) = 0;
	//// Is any animation playing?
	//virtual bool IsPlaying() = 0;
	//// Get current playing animation index
	//// If no animation, will return -1
	//virtual int GetCurrentAnimation() = 0;
	//// Get length of animation
	//virtual float GetAnimationLength(int animationIndex) = 0;
};