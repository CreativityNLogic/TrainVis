#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include <glm/glm.hpp>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "../Components/TransformComponent.h"

class MotionState : public btMotionState
{
protected:
	TransformComponent *mTransformComponent;
	btTransform mInitialPosition;

public:
	MotionState(TransformComponent *comp)
	{
		mTransformComponent = comp;
		if (mTransformComponent != nullptr)
		{
			mInitialPosition = btTransform(btQuaternion(comp->Rotation.x, comp->Rotation.y, comp->Rotation.z, comp->Rotation.w), btVector3(comp->Position.x, comp->Position.y, comp->Position.z));
		}
	}

	virtual ~MotionState()
	{
	}

	void setTranformComponent(TransformComponent *comp)
	{
		mTransformComponent = comp;
	}

	virtual void getWorldTransform(btTransform &worldTrans) const
	{
		worldTrans = mInitialPosition;
	}

	virtual void setWorldTransform(const btTransform &worldTrans)
	{
		if (mTransformComponent == nullptr)
			return; 

		btQuaternion rot = worldTrans.getRotation();
		mTransformComponent->Rotation = glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
		btVector3 pos = worldTrans.getOrigin();
		mTransformComponent->Position = glm::vec3(pos.x(), pos.y(), pos.z());
	}
};

#endif // MOTIONSTATE_H