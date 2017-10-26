#ifndef JOINT_H
#define JOINT_H

#include <glm/glm.hpp>
#include <vector>

class Joint
{
public:
	Joint() : mID(-1)
	{

	}

	Joint(const Joint &joint)
	{
		mID = joint.mID;
		mName = joint.mName;
		mChildren = joint.mChildren;
		mTransform = joint.mTransform;
	}

	Joint(unsigned int index, std::string name, glm::mat4 bindLocalTransform) :
		mID(index),
		mName(name),
		mLocalBindTransform(bindLocalTransform)
	{

	}

	void SetID(unsigned int id)
	{
		mID = id;
	}

	unsigned int GetID() const
	{
		return mID;
	}

	void SetName(std::string name)
	{
		mName = name;
	}

	std::string GetName() const
	{
		return mName;
	}

	void AddJoint(const Joint &joint)
	{
		mChildren.push_back(joint);
	}

	void RemoveJoint(unsigned int index)
	{
		if (index >= mChildren.size())
			return;

		mChildren.erase(mChildren.begin() + index);
	}

	void SetJoint(const Joint &joint, unsigned int index)
	{
		if (index >= mChildren.size())
			return;

		mChildren[index] = joint;
	}

	void SetChildren(std::vector<Joint> children);

	std::vector<Joint> GetChildren() const;

	void SetTransform(glm::mat4 transform);

	glm::mat4 GetTransform() const;

	void SetInverseBindTransform(glm::mat4 inverseBindTransform);

	glm::mat4 GetInverseBindTransform() const;

	void CalculateInverseBindTransform(glm::mat4 parentBindTransform);

private:
	std::vector<Joint> mChildren;
	std::string mName;
	unsigned int mID;

	glm::mat4 mTransform;
	glm::mat4 mLocalBindTransform;
	glm::mat4 mInverseBindTransform;
};

#endif // JOINT_H