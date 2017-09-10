#include "PhysicsController.h"

PhysicsController::PhysicsController() {} //Constructor.

//Takes as input the body json file and then returns a rigidbody.
RigidBody PhysicsController::getRigidBody(nlohmann::json body) 
{ 
	int shapeType; //Get the value that determines the shape.
	if (!body["Shape"].is_null()) 
	{
		if (body["Shape"] == "Box")
			shapeType = 0;
		else if (body["Shape"] == "Circle")
			shapeType = 1;
		if (body["Shape"] == "Convex")
			shapeType = 2;
	}

	glm::vec3 *inertia; //Get the inertia value.
	if (body["Inertia"][0].is_number() && body["Inertia"][1].is_number() && body["Inertia"][2].is_number()) 
	{
		inertia[0] = body["Inertia"][0];
		inertia[1] = body["Inertia"][0];
		inertia[2] = body["Inertia"][0];
	}
	
	float mass; //Get the mass value.
	if (body["Mass"].is_number())
		mass = body["Mass"];

	glm::vec3 *origin; //Get the origin value.
	if (body["Origin"][0].is_number() && body["Origin"][1].is_number() && body["Origin"][2].is_number()) 
	{
		origin[0] = body["Origin"][0];
		origin[1] = body["Origin"][1];
		origin[2] = body["Origin"][2];
	}

	return RigidBody::RigidBody(shapeType, inertia, mass, origin); //Return the rigidbody wrapper object.
}