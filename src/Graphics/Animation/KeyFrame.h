#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <map>

#include "JointTransform.h"

class KeyFrame
{
public:
	KeyFrame(float timeStamp, std::map<std::string, JointTransform> jointKeyFrames) :
		mTimeStamp(timeStamp),
		mPose(jointKeyFrames)
	{

	}

public:
	float GetTimeStamp() const
	{
		return mTimeStamp;
	}

	std::map<std::string, JointTransform> GetJointKeyFrames()
	{
		return mPose;
	}

private:
	float mTimeStamp;
	std::map<std::string, JointTransform> mPose;
};

#endif // KEYFRAME_H