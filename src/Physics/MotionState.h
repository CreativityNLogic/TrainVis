#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H


#include <bullet/btBulletDynamicsCommon.h>

#include "../Components/TransformComponent.h"

class MotionState : public btMotionState
{
protected:
	TransformComponent *mTransformComponent;

public:
	MotionState() :
		mTransformComponent(nullptr)
	{

	}

	MotionState(TransformComponent *transComp) :
		mTransformComponent(nullptr)
	{
		mTransformComponent = transComp;
	}

	virtual ~MotionState()
	{
	}

	void SetTransformComponent(TransformComponent *transComp)
	{
		mTransformComponent = transComp;
	}

	virtual void getWorldTransform(btTransform &worldTrans) const
	{
		if (mTransformComponent == nullptr)
			return;

		btVector3 pos(mTransformComponent->Position.x, mTransformComponent->Position.y, mTransformComponent->Position.z);

		btQuaternion quat;
		quat.setEuler(mTransformComponent->Rotation.x, mTransformComponent->Rotation.y, mTransformComponent->Rotation.z);
		worldTrans = btTransform(quat, pos);
	}

	virtual void setWorldTransform(const btTransform &worldTrans)
	{
		if (mTransformComponent == nullptr)
			return;

		btQuaternion rot = worldTrans.getRotation();

		btVector3 pos = worldTrans.getOrigin();
		mTransformComponent->Position = glm::vec3(pos.x(), pos.y(), pos.z());
	}
};


#endif // MOTIONSTATE_H