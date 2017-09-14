#include "RigidBody.h"
#include "MotionState.h"

RigidBody::RigidBody()
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, new btDefaultMotionState(), new btSphereShape(1.0f), btVector3()); //All the information needed to construct the rigidbody in the wrapper.
	mRigidBody = new btRigidBody(rbInfo);
}

RigidBody::RigidBody(float mass, btMotionState *motionState, btCollisionShape *shape, glm::vec3 localInertia, glm::vec3 offset) :
	mOffset(offset)
{
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, btVector3(localInertia.x, localInertia.y, localInertia.z)); //All the information needed to construct the rigidbody in the wrapper.
	mRigidBody = new btRigidBody(rbInfo);
}

RigidBody::~RigidBody()
{
	delete mRigidBody;
}

void RigidBody::setOffset(const glm::vec3 &offset)
{
	mOffset = offset;
}

glm::vec3 RigidBody::getOffset() const
{
	return mOffset;
}

btMotionState* RigidBody::getMotionState()
{
	return mRigidBody->getMotionState();
}

btRigidBody* RigidBody::getRigidBody()
{
	return mRigidBody;
}

void RigidBody::ApplyCentralForce(const glm::vec3 &force)
{
	mRigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));
}

void RigidBody::ApplyCentralImpulse(const glm::vec3 &impulse)
{
	mRigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

void RigidBody::ApplyForce(const glm::vec3 &force, const glm::vec3 &relativePos)
{
	mRigidBody->applyForce(btVector3(force.x, force.y, force.z), btVector3(relativePos.x, relativePos.y, relativePos.z));
}

void RigidBody::ApplyImpulse(const glm::vec3 &impulse, const glm::vec3 &relativePos)
{
	mRigidBody->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(relativePos.x, relativePos.y, relativePos.z));
}