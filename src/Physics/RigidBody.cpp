#include "RigidBody.h"
#include "MotionState.h"

// ** int shape needs to be redefined in each method as a mesh object; not sure what the type is that we're using for that **
RigidBody::RigidBody(float mass, btMotionState *motionState, btCollisionShape *shape, glm::vec3 localInertia)
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(mass), motionState, shape, btVector3(localInertia.x, localInertia.y, localInertia.z)); //All the information needed to construct the rigidbody in the wrapper.
	mRigidBody = new btRigidBody(rbInfo);
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

RigidBody::~RigidBody()
{
	delete mRigidBody;
}
