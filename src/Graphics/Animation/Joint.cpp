#include <glm/glm.hpp>
#include <vector>

#include "Joint.h"

Joint::Joint() : mID(-1)
{

}

Joint::Joint(const Joint &joint)
{
	mID = joint.mID;
	mName = joint.mName;
	mChildren = joint.mChildren;
	mTransform = joint.mTransform;
}

Joint::Joint(unsigned int index, std::string name, glm::mat4 bindLocalTransform) :
	mID(index),
	mName(name),
	mLocalBindTransform(bindLocalTransform)
{

}

void Joint::SetID(unsigned int id)
{
	mID = id;
}

unsigned int Joint::GetID() const
{
	return mID;
}

void Joint::SetName(std::string name)
{
	mName = name;
}

std::string Joint::GetName() const
{
	return mName;
}

void Joint::AddJoint(const Joint &joint)
{
	mChildren.push_back(joint);
}

void Joint::RemoveJoint(unsigned int index)
{
	if (index >= mChildren.size())
		return;

	mChildren.erase(mChildren.begin() + index);
}

void Joint::SetJoint(const Joint &joint, unsigned int index)
{
	if (index >= mChildren.size())
		return;

	mChildren[index] = joint;
}

void Joint::SetChildren(std::vector<Joint> children)
{
	mChildren = children;
}

std::vector<Joint> Joint::GetChildren() const
{
	return mChildren;
}

void Joint::SetTransform(glm::mat4 transform)
{
	mTransform = transform;
}

glm::mat4 Joint::SetTransform() const
{
	return mTransform;
}

void Joint::SetInverseBindTransform(glm::mat4 inverseBindTransform)
{
	mInverseBindTransform = inverseBindTransform;
}

glm::mat4 Joint::SetInverseBindTransform() const
{
	return mInverseBindTransform;
}

void Joint::CalculateInverseBindTransform(glm::mat4 parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform * mLocalBindTransform;
	mInverseBindTransform = glm::inverse(bindTransform);

	for (auto &child : mChildren)
		child.CalculateInverseBindTransform(bindTransform);
}