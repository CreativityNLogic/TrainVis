#include "PhysicsFactory.h"

//Constructor.
PhysicsFactory::PhysicsFactory(RigidWorld *world)
{
	mPhysicsWorld = world;
}

//Takes as input the body json file and then returns a rigidbody.
RigidBody* PhysicsFactory::createRigidBody(nlohmann::json body) 
{ 
	int shapeType = 0; //Get the value that determines the shape.
	if (!body["Shape"].is_null()) 
	{
		btCollisionShape *shape = nullptr;
		if (body["Shape"] == "Box") 
		{
			 //shape = new btBoxShape();
		}
		else if (body["Shape"] == "Circle") 
		{
			//shape = new btSphereShape();
		}
		else if (body["Shape"] == "Convex")
		{

		}
		else
		{
			// Make default shape...
		}
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

	btCollisionShape* shape = new btConvexHullShape(); //Shape will always be a convex?

	btTransform shapeTransform; //Create a transform object.
	shapeTransform.setIdentity(); //Needed?
	shapeTransform.setOrigin(btVector3(body["Transform"]["Origin"][0], body["Transform"]["Origin"][1], body["Transform"]["Origin"][2])); //Set position.
	
	btQuaternion q; //Set Rotation.
	q.setEuler(body["Transform"]["Rotation"][0], body["Transform"]["Rotation"][1], body["Transform"]["Rotation"][2]);
	shapeTransform.setRotation(q);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(inertia.x, inertia.y, inertia.z); //Calculate the inertia.
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(shapeTransform);

	RigidBody *rigidBody = new RigidBody(mass, myMotionState, shape, inertia);
	mPhysicsWorld->addRigidBody(rigidBody);
	return rigidBody; //Return the rigidbody wrapper object.
}