#include "RigidBody.h"

// ** int shape needs to be redefined in each method as a mesh object; not sure what the type is that we're using for that **
RigidBody::RigidBody(btRigidBody::btRigidBodyConstructionInfo info, RigidWorld world)
{
	holder = constructHelper(shape, inertia, inMass, origin); //Do yo thing.
}

btRigidBody* RigidBody::constructHelper(int shape, const glm::vec3 &inertia, float inMass, const glm::vec3 &origin)
{
}

RigidBody* RigidBody::rebuildBody(int shape, const glm::vec3 &inertia, float inMass, const glm::vec3 &origin)
{
	delete holder;
	holder = constructHelper(shape, inertia, inMass, origin);
	return this;
}

glm::vec3 RigidBody::getOrigin() const
{
	btVector3 pos = holder->getCenterOfMassPosition();
	return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

btMotionState* RigidBody::getMotionState()
{
	return holder->getMotionState();
}

btRigidBody* RigidBody::getRigidBody()
{
	return holder;
}

RigidBody::~RigidBody()
{
	delete holder;
}
