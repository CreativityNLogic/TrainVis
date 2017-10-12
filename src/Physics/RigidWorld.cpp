#include "RigidWorld.h"

RigidWorld::RigidWorld()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mCollisionDispatcher = std::make_unique<btCollisionDispatcher>(mCollisionConfig.get());
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep
	mBroadphase = std::make_unique<btDbvtBroadphase>();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = std::make_unique<btSequentialImpulseConstraintSolver>();

	mPhysicsWorld = std::make_unique<btDiscreteDynamicsWorld>(mCollisionDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfig.get());
	mPhysicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
}

RigidWorld::RigidWorld(const glm::vec3 &gravity)
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mCollisionDispatcher = std::make_unique<btCollisionDispatcher>(mCollisionConfig.get());
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep
	mBroadphase = std::make_unique<btDbvtBroadphase>();
	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
	
	mPhysicsWorld = std::make_unique<btDiscreteDynamicsWorld>(mCollisionDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfig.get());
	mPhysicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

RigidWorld::~RigidWorld()
{
	removeAllBodies();
}

void RigidWorld::addRigidBody(RigidBody* container)
{
	mPhysicsWorld->addRigidBody(container->getRigidBody());
}

void RigidWorld::addRigidBody(RigidBody* container, int group, int mask)
{
	mPhysicsWorld->addRigidBody(container->getRigidBody(), group, mask);
}

void RigidWorld::removeAllBodies()
{
	for (int i = mPhysicsWorld->getNumCollisionObjects() - 1; i >= 0;i--)
	{
		btCollisionObject* obj = mPhysicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		
		if (body && body->getMotionState())
			delete body->getMotionState();

		mPhysicsWorld->removeCollisionObject(obj);
		delete obj;
	}
}

/*
	removes first instance of a RigidBody from RigidWorld
*/
void RigidWorld::removeRigidBody(RigidBody* toBeRemoved)
{
	btRigidBody* container = toBeRemoved->getRigidBody();
	for (int i = mPhysicsWorld->getNumCollisionObjects() - 1; i >= 0;i--)
	{
		btCollisionObject* obj = mPhysicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body == container)
		{
			if (container && container->getMotionState())
				delete body->getMotionState();

			mPhysicsWorld->removeCollisionObject(container);
			delete toBeRemoved;
			break;
		}
	}
}

void RigidWorld::setDebugDraw(btIDebugDraw *debugDraw)
{
	mPhysicsWorld->setDebugDrawer(debugDraw);
}

void RigidWorld::debugDrawWorld()
{
	mPhysicsWorld->debugDrawWorld();
}

void RigidWorld::stepSimulation(double framerate, int maxSubSteps, double fixedTimestep)
{
	if(mPhysicsWorld != nullptr)
		mPhysicsWorld->stepSimulation(btScalar(framerate), maxSubSteps, fixedTimestep);
}