#ifndef _APPLCATION_H_
#define _APPLCATION_H_

#include <memory>

#include "Core/BaseApplication.h"

//*********************************************
//	Application
//
//	A graphical application that will display a train
//	moving
//	Note: This application can be swapped to a different
//	application class once it inherits BaseApplication
//	eg class SomeApplication : public BaseApplication
//*********************************************

#include "Graphics/Shader.h"
#include "Graphics/Model.h"

class Application : public BaseApplication
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
	std::unique_ptr<Model> mTrainModel;
	std::unique_ptr<Shader> mTrainShader;
};

#endif // _APPLCATION_H_