#ifndef PHYSICS_FACTORY
#define PHYSICS_FACTORY

#include <json/json.hpp>
#include <glm/common.hpp>
#include "RigidBody.h"
#include "RigidWorld.h"

class PhysicsFactory {
public:
	PhysicsFactory(std::unique_ptr<RigidWorld> rWorld);
private:
	RigidBody createRigidBody(nlohmann::json body); //Return a rigidBodywrapper object with the information from the json file.
	void setWorld(std::unique_ptr<RigidWorld> rWorld);
	RigidWorld getWorld();

	std::unique_ptr<RigidWorld> factoryWorld;
};

#endif // PHYSICS_CONTROLLER