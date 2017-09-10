#ifndef RIGIDWORLD_H
#define RIGIDWORLD_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>

#include "RigidBody.h"

class RigidWorld
{
public:
	RigidWorld(glm::vec3* gravity);
	void addRigidBody(RigidBody* container);
	void removeAllBodies();
	void removeRigidBody(RigidBody* toBeRemoved);
	void stepSimulation(float framerate, float step);
	virtual ~RigidWorld();

private:
	btDiscreteDynamicsWorld* holder;
};

#endif // RIGIDWORLD_H