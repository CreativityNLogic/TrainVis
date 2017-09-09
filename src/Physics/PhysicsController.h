#ifndef PHYSICS_CONTROLLER
#define PHYSICS_CONTROLLER

#include <JSON/json.hpp>
#include <glm/common.hpp>
#include "RigidBody.h"

class PhysicsController {
public:
	PhysicsController();
private:
	RigidBody getRigidBody(nlohmann::json body); //Return a rigidBodywrapper object with the information from the json file.
};

#endif PHYSICS_CONTROLLER