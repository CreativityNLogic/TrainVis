#include "RigidBody.h"



/*
	int shape needs to be redefined in each method as a mesh object; not sure what the type is that we're using for that
*/
RigidBody::RigidBody(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin)
{
	holder = constructHelper(shape, inertia, inMass, origin);
}

btRigidBody* RigidBody::constructHelper(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin)
{
	btCollisionShape* mesh = new btConvexHullShape(shape);

	btTransform meshTransform;
	meshTransform.setIdentity();
	meshTransform.setOrigin(btVector3(origin->x, origin->y, origin->z));

	btScalar mass(inMass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(inertia->x, inertia->y, inertia->z);
	if (isDynamic)
		mesh->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(meshTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, mesh, localInertia);
	return new btRigidBody(rbInfo);
}

RigidBody* RigidBody::rebuildBody(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin)
{
	delete holder;
	holder = constructHelper(shape, inertia, inMass, origin);
	return this;
}

glm::vec3* RigidBody::getOrigin()
{
	btVector3 pos = holder->getCenterOfMassPosition();
	glm::vec3* returnable = new glm::vec3(pos.getX(), pos.getY(), pos.getZ());
	return returnable;
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
