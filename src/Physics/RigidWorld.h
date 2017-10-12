#ifndef RIGIDWORLD_H
#define RIGIDWORLD_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/common.hpp>

#include <memory>

#include "RigidBody.h"

class RigidWorld
{
public:
	RigidWorld();
	RigidWorld(const glm::vec3 &gravity);

	void addRigidBody(RigidBody* container);
	void addRigidBody(RigidBody* container, int group, int mask);

	void removeAllBodies();
	void removeRigidBody(RigidBody* toBeRemoved);
	void stepSimulation(double framerate, int maxSubSteps = 1, double fixedTimestep = 1.0 / 60.0);
	void setDebugDraw(btIDebugDraw *debugDraw);
	void debugDrawWorld();

	virtual ~RigidWorld();

private:
	std::unique_ptr<btDiscreteDynamicsWorld> mPhysicsWorld;
	std::unique_ptr<btDefaultCollisionConfiguration>  mCollisionConfig;
	std::unique_ptr<btCollisionDispatcher> mCollisionDispatcher;
	std::unique_ptr<btBroadphaseInterface>	mBroadphase;
	std::unique_ptr<btSequentialImpulseConstraintSolver> mSolver;
};

#endif // RIGIDWORLD_H