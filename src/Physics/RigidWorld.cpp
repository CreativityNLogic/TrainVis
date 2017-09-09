#include "RigidWorld.h"




RigidWorld::RigidWorld(glm::vec3* gravity)
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	holder = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	holder->setGravity(btVector3(gravity->x, gravity->y,gravity->z));
}

void RigidWorld::addRigidBody(RigidBody* container)
{
	holder->addRigidBody(container->getRigidBody());
}

void RigidWorld::removeAllBodies()
{
	for (int i = holder->getNumCollisionObjects() - 1; i >= 0;i--)
	{
		btCollisionObject* obj = holder->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		holder->removeCollisionObject(obj);
		delete obj;
	}
}


/*
	removes first instance of a RigidBody from RigidWorld
*/
void RigidWorld::removeRigidBody(RigidBody* toBeRemoved)
{
	btRigidBody* container = toBeRemoved->getRigidBody();
	for (int i = holder->getNumCollisionObjects() - 1; i >= 0;i--)
	{
		btCollisionObject* obj = holder->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body == container)
		{
			if (container && container->getMotionState())
			{
				delete body->getMotionState();
			}
			holder->removeCollisionObject(container);
			delete toBeRemoved;
			break;
		}
	}
}

void RigidWorld::stepSimulation(float framerate, float step)
{
	holder->stepSimulation(framerate, step);
}


RigidWorld::~RigidWorld()
{
	removeAllBodies();
	delete holder;
}
