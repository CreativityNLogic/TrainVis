#include "BaseApplication.h"
#include "../Graphics/RenderWindow.h"

#include <algorithm>
#include <GLFW/glfw3.h>

BaseApplication::BaseApplication() :
	mIsRunning(false),
	mDeltaTime(1.0/60.0),
	mRenderWindow(new RenderWindow())
{

}

void BaseApplication::Quit() 
{ 
	mIsRunning = false; 
}

bool BaseApplication::IsRunning() 
{ 
	return mIsRunning; 
}

double BaseApplication::GetDeltaTime() const
{
	return mDeltaTime;
}

void BaseApplication::StartLoop()
{
	mIsRunning = true;

	double previousTime = 0.0;
	double currentTime = glfwGetTime();

	while (mIsRunning && mRenderWindow->IsOpened())
	{
		previousTime = currentTime;
		currentTime = glfwGetTime();
		mDeltaTime = currentTime - previousTime;

		Update(mDeltaTime);
	}
}