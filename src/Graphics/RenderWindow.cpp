#include "RenderWindow.h"
#include <glm/glm.hpp>
#include <iostream>

RenderWindow::RenderWindow() :
    mRenderWindow(nullptr)
{

}

RenderWindow::RenderWindow(const std::string &windowName, const int &width, const int &height, bool fullscreen) :
    mRenderWindow(nullptr)
{
    Initialise(windowName, width, height, fullscreen);
}

RenderWindow::~RenderWindow()
{
    glfwDestroyWindow(mRenderWindow);
    glfwTerminate();
}

bool RenderWindow::Initialise(const std::string &windowName, const int &width, const int &height, bool fullscreen)
{
    glfwSetErrorCallback(ErrorCallback);
	
	// Initialise glfw
	if (!glfwInit())
	{
		std::cout << "Cannot initialise GLFW!" << std::endl;
		return false;
	}

	// Window hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if(fullscreen)
        mRenderWindow = glfwCreateWindow(width, height, windowName.c_str(), glfwGetPrimaryMonitor(), nullptr);
    else
        mRenderWindow = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	

	if(!mRenderWindow)
    {
		std::cout << "Cannot initialise window!" << std::endl;
        glfwTerminate();
        return false;
    }

     glfwMakeContextCurrent(mRenderWindow);

	// Callbacks
	glfwSetFramebufferSizeCallback(mRenderWindow, ResizeCallback);
	glfwSetWindowCloseCallback(mRenderWindow, CloseWindowCallback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(mRenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set up context with GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Cannot initialise GLAD!" << std::endl;
		glfwTerminate();
		return false;
	}

	// Enable certain opengl params
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    return true;
}

void RenderWindow::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderWindow::Display()
{
    glfwSwapBuffers(mRenderWindow);
}

void RenderWindow::PollEvents()
{
    glfwPollEvents();
}

bool RenderWindow::IsOpened() const
{
    return !glfwWindowShouldClose(mRenderWindow);
}

void RenderWindow::ErrorCallback(int error, const char *description)
{
    std::cerr << description << ": " << error << std::endl;
}

void RenderWindow::CloseWindowCallback(GLFWwindow *window)
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderWindow::ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




glm::vec2 RenderWindow::GetMousePosition() {
	double x; double y; 
	glfwGetCursorPos(mRenderWindow, &x, &y); 
	return glm::vec2(x, y); 
}

bool RenderWindow::IsKeyPressed(Key key)
{
    if (glfwGetKey(mRenderWindow, key) == GLFW_PRESS)
        return true;
    else
        return false;
}

bool RenderWindow::IsKeyReleased(Key key)
{
	if (glfwGetKey(mRenderWindow, key) == GLFW_RELEASE)
		return true;
	else
		return false;
}

bool RenderWindow::IsMousePressed(Mouse button)
{
	if (glfwGetMouseButton(mRenderWindow, button) == GLFW_PRESS)
		return true;
	else
		return false;
}

bool RenderWindow::IsMouseReleased(Mouse button)
{
	if (glfwGetMouseButton(mRenderWindow, button) == GLFW_RELEASE)
		return true;
	else
		return false;
}

