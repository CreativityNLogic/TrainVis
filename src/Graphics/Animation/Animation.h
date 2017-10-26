#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/glm.hpp>
#include <vector>

#include "KeyFrame.h"

class Animation
{
public:
	Animation() :
		mLength(0)
	{

	}

	Animation(float lengthInSeconds, std::vector<KeyFrame> frames) :
		mKeyFrames(frames),
		mLength(lengthInSeconds)
	{

	}

	float GetLength() const
	{
		return mLength;
	}

	std::vector<KeyFrame> GetKeyFrames() const
	{
		return mKeyFrames;
	}

private:
	std::vector<KeyFrame> mKeyFrames;
	float mLength;
};

#endif // ANIMATION_H