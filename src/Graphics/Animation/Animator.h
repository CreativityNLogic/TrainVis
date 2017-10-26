#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <cmath>
#include <map>
#include <vector>

#include "AnimatedMesh.h"

class Animator
{
public:
	Animator() :
		mAnimatedMesh(nullptr),
		mCurrentAnimation(nullptr),
		mAnimationTime(0)
	{
	}

	Animator(AnimatedMesh *mesh) :
		mAnimatedMesh(mesh),
		mCurrentAnimation(nullptr),
		mAnimationTime(0)
	{

	}

	void PlayAnimation(Animation *animation)
	{
		mCurrentAnimation = animation;
		mAnimationTime = 0;
	}

	void Update(float deltaTime)
	{
		if (mCurrentAnimation == nullptr)
			return;

		increaseAnimationTime(deltaTime);
		std::map<std::string, glm::mat4> currentPose = calculateCurrentAnimationPose();
		applyPoseToJoints(currentPose, mAnimatedMesh->GetRootJoint(), glm::mat4());
	}

private:
	void increaseAnimationTime(float deltaTime)
	{
		mAnimationTime += deltaTime;
		if (mAnimationTime > mCurrentAnimation->GetLength())
			mAnimationTime = std::fmodf(mAnimationTime, mCurrentAnimation->GetLength());
	}

	std::map<std::string, glm::mat4> calculateCurrentAnimationPose()
	{
		std::vector<KeyFrame> frames = getPreviousAndNextFrames();
		float progression = calculateProgression(frames[0], frames[1]);
		return interpolatePoses(frames[0], frames[1], progression);
	}

	void applyPoseToJoints(std::map<std::string, glm::mat4> currentPose, Joint joint, glm::mat4 parentTransform)
	{
		glm::mat4 currentLocalTransform = currentPose[joint.GetName()];
		glm::mat4 currentTransform = parentTransform * currentLocalTransform;
		for (auto &child : joint.GetChildren())
			applyPoseToJoints(currentPose, child, currentTransform);
		currentTransform = currentTransform * joint.GetInverseBindTransform();
		joint.SetTransform(currentTransform);
	}

	std::vector<KeyFrame> getPreviousAndNextFrames()
	{
		std::vector<KeyFrame> allFrames = mCurrentAnimation->GetKeyFrames();

		KeyFrame previousFrame = allFrames[0];
		KeyFrame nextFrame = allFrames[0];
		for (int i = 1; i < allFrames.size(); i++)
		{
			nextFrame = allFrames[i];
			if (nextFrame.GetTimeStamp() > mAnimationTime)
				break;
			previousFrame = allFrames[1];
		}

		return std::vector<KeyFrame> { previousFrame, nextFrame };
	}

	float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame)
	{
		float totalTime = nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp();
		float currentTime = mAnimationTime - previousFrame.GetTimeStamp();
		return currentTime / totalTime;
	}

	std::map<std::string, glm::mat4> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression)
	{
		std::map<std::string, glm::mat4> currentPose;
		for (auto iter = previousFrame.GetJointKeyFrames().begin(); iter != previousFrame.GetJointKeyFrames().end(); iter++)
		{
			JointTransform previousTransform = previousFrame.GetJointKeyFrames().find(iter->first)->second;
			JointTransform nextTransform = nextFrame.GetJointKeyFrames().find(iter->first)->second;
			JointTransform currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, progression);
			currentPose[iter->first] = currentTransform.GetLocalTransform();
		}

		return currentPose;
	}

private:
	AnimatedMesh *mAnimatedMesh;
	Animation *mCurrentAnimation;
	float mAnimationTime;
};

#endif // ANIMATOR_H