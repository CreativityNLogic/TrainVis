#ifndef RIGIDWORLD_H_
#define RIGIDWORLD_H_

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>

#include "RigidBody.h"

class RigidWorld
{
private:
	btDiscreteDynamicsWorld* holder;
public:
	RigidWorld(glm::vec3* gravity);
	void addRigidBody(RigidBody* container);
	void removeAllBodies();
	void removeRigidBody(RigidBody* toBeRemoved);
	void stepSimulation(float framerate, float step);
	virtual ~RigidWorld();
};

#endif // RIGIDWORLD_H_