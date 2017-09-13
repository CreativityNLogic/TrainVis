#ifndef _APPLCATION_H_
#define _APPLCATION_H_

#include <entityx/entityx.h>
#include <memory>

#include "Core/BaseApplication.h"
#include "Graphics/Model.h"
#include "Physics/RigidWorld.h"

//*********************************************
//	Application
//
//	A graphical application that will display a train
//	moving
//	Note: This application can be swapped to a different
//	application class once it inherits BaseApplication
//	eg class SomeApplication : public BaseApplication
//*********************************************

class Application : public BaseApplication, public entityx::EntityX
{
public:
	// Constructor
	Application();
	// Initalisie application
	bool Initialise();
	// Terminate application
	void Terminate();
	// Update application per frame
	void Update(double deltaTime);

private:
	entityx::EntityX mEntityWorld;
	std::unique_ptr<RigidWorld> mPhysicWorld;
	std::unique_ptr<Model> mTrainModel;
	std::unique_ptr<Model> mCubeModel;
	std::unique_ptr<Model> mGroundModel;
	glm::vec2 mOldMousePos;
};

#endif // _APPLCATION_H_