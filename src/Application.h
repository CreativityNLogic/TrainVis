#ifndef _APPLCATION_H_
#define _APPLCATION_H_

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
};

#endif // _APPLCATION_H_