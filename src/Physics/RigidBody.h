#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>
#include "RigidWorld.h"

class RigidBody
{
public:
	RigidBody(btRigidBody::btRigidBodyConstructionInfo info, RigidWorld world);
	virtual ~RigidBody();

	RigidBody* rebuildBody(int shape, const glm::vec3 &inertia, float inMass, const glm::vec3 &origin);
	
	glm::vec3 getOrigin() const;
	btMotionState* getMotionState();
	btRigidBody * getRigidBody();

private:
	btRigidBody* holder;
	btRigidBody* constructHelper(int shape, const glm::vec3 &inertia, float inMass, const glm::vec3 &origin);
};

#endif // RIGIDBODY_H