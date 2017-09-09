#pragma once
#include "bullet\btBulletCollisionCommon.h"
#include "bullet\btBulletDynamicsCommon.h"
#include "glm\common.hpp"

class RigidBody{
private:
	btRigidBody* holder;

	btRigidBody* constructHelper(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin);


public:
	RigidBody(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin);
	RigidBody* rebuildBody(int shape, glm::vec3* inertia, float inMass, glm::vec3* origin);
	glm::vec3* getOrigin();
	btMotionState* getMotionState();
	btRigidBody * getRigidBody();
	virtual ~RigidBody();

};

