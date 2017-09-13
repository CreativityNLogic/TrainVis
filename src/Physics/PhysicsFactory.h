#ifndef PHYSICS_FACTORY
#define PHYSICS_FACTORY

#include <json/json.hpp>
#include <glm/common.hpp>
#include "RigidBody.h"
#include "RigidWorld.h"

class PhysicsFactory {
public:
	PhysicsFactory(RigidWorld *world);
private:
	RigidBody* createRigidBody(nlohmann::json body); //Return a rigidBodywrapper object with the information from the json file.

	RigidWorld *mPhysicsWorld;
};

#endif // PHYSICS_CONTROLLER