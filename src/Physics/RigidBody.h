#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>

class RigidBody
{
public:
	RigidBody(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin);
	virtual ~RigidBody();

	RigidBody* rebuildBody(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin);
	
	glm::vec3 getOrigin() const;
	btMotionState* getMotionState();
	btRigidBody * getRigidBody();

private:
	btRigidBody* holder;
	btRigidBody* constructHelper(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin);
};

#endif // RIGIDBODY_H