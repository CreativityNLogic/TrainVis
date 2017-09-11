#include "PhysicsFactory.h"

//Constructor.
PhysicsFactory::PhysicsFactory(std::unique_ptr<RigidWorld> rWorld)
{
	setWorld(rWorld);
}

//The the world that the factory places the rigidBody in.
void PhysicsFactory::setWorld(std::unique_ptr<RigidWorld> rWorld)
{
	std::make_unique<RigidWorld>(rWorld);
}

//Return the world the factory is currently acting upon.
RigidWorld PhysicsFactory::getWorld()
{
	return factoryWorld.get();
}

//Takes as input the body json file and then returns a rigidbody.
RigidBody PhysicsFactory::createRigidBody(nlohmann::json body) 
{ 
	int shapeType = 0; //Get the value that determines the shape.
	if (!body["Shape"].is_null()) 
	{
		btCollisionShape shape;
		if (body["Shape"] == "Box") {
			 shape = new btBoxShape();
		}
		else if (body["Shape"] == "Circle") {
			shape = new btSphereShape();
		}
		if (body["Shape"] == "Convex")
			shapeType = 2;
	}

	glm::vec3 inertia; //Get the inertia value.
	if (body["Inertia"][0].is_number() && body["Inertia"][1].is_number() && body["Inertia"][2].is_number()) 
	{
		inertia[0] = body["Inertia"][0];
		inertia[1] = body["Inertia"][0];
		inertia[2] = body["Inertia"][0];
	}
	
	float mass = 0; //Get the mass value.
	if (body["Mass"].is_number())
		mass = body["Mass"];

	glm::vec3 origin; //Get the origin value.
	if (body["Origin"][0].is_number() && body["Origin"][1].is_number() && body["Origin"][2].is_number()) 
	{
		origin[0] = body["Origin"][0];
		origin[1] = body["Origin"][1];
		origin[2] = body["Origin"][2];
	}

	btCollisionShape* shape = new btConvexHullShape(); //Shape will always be a convex?

	btTransform meshTransform; //??
	meshTransform.setIdentity();
	meshTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(inertia.x, inertia.y, inertia.z); //Calculate the inertia.
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(meshTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia); //All the information needed to construct the rigidbody in the wrapper.

	return RigidBody::RigidBody(rbInfo, factoryWorld); //Return the rigidbody wrapper object.
}