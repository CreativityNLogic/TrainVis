#ifndef _BASEAPPLCATION_H_
#define _BASEAPPLCATION_H_

//*********************************************
//	Base Application
//
//	A base application class that can be extended for 
//	other various types of applications. These can be swapped 
//	in the int main() function
//*********************************************

#include <memory>

class RenderWindow;

class BaseApplication
{
public:
	// Constructor
	BaseApplication();

	// Interface Functions
	virtual bool Initialise() = 0;
	virtual void Terminate() = 0;
	virtual void Update(double deltaTime) = 0;

	// Start main loop
	void StartLoop();
	// Quit application
	void Quit();
	// Check if application is running
	bool IsRunning();

	// Get variable deltatime
	double GetDeltaTime() const;

protected:
	std::unique_ptr<RenderWindow> mRenderWindow;

private:
	bool mIsRunning;
	double mDeltaTime;
};

#endif // _BASEAPPLICATION_HPP_