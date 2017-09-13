#include "RigidBody.h"
#include "MotionState.h"

RigidBody::RigidBody()
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, new btDefaultMotionState(), new btSphereShape(1.0f), btVector3()); //All the information needed to construct the rigidbody in the wrapper.
	mRigidBody = new btRigidBody(rbInfo);
}

RigidBody::RigidBody(float mass, btMotionState *motionState, btCollisionShape *shape, glm::vec3 localInertia)
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, btVector3(localInertia.x, localInertia.y, localInertia.z)); //All the information needed to construct the rigidbody in the wrapper.
	mRigidBody = new btRigidBody(rbInfo);
}

RigidBody::~RigidBody()
{
	delete mRigidBody;
}

glm::vec3 RigidBody::getOrigin() const
{
	btVector3 pos = mRigidBody->getCenterOfMassPosition();
	return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

btMotionState* RigidBody::getMotionState()
{
	return mRigidBody->getMotionState();
}

btRigidBody* RigidBody::getRigidBody()
{
	return mRigidBody;
}