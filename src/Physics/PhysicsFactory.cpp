#include "PhysicsFactory.h"
#include "../Physics/MotionState.h"

//Constructor.
PhysicsFactory::PhysicsFactory(RigidWorld *world)
{
	mPhysicsWorld = world;
}

//Takes as input the body json file and then returns a rigidbody.
RigidBody* PhysicsFactory::createRigidBody(nlohmann::json body) 
{ 
	btCollisionShape *shape = nullptr;

	if (!body["Shape"].is_null()) 
	{
		if (body["Shape"] == "Box") 
		{
			std::vector<float> boxExtent = body["Shape"]["Radius"];
			if (boxExtent.size() == 3)
				shape = new btBoxShape(btVector3(boxExtent[0], boxExtent[1], boxExtent[2]));
		}
		else if (body["Shape"] == "Circle") 
		{
			float radius = body["Shape"]["Radius"];
			shape = new btSphereShape(radius);
		}
		else if (body["Shape"] == "Convex")
		{

		}
		else
		{
			shape = new btSphereShape(1.0f);
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

	// NEED TO GET TRANSFORM COMPONENT TO SET THE RIGIDBODY!

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(inertia.x, inertia.y, inertia.z); //Calculate the inertia.
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btMotionState* motionState = new MotionState();

	RigidBody *rigidBody = new RigidBody(mass, motionState, shape, inertia);
	mPhysicsWorld->addRigidBody(rigidBody);

	return rigidBody; //Return the rigidbody wrapper object.
}