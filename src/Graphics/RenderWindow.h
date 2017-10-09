#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#define GLFW_INCLUDE_GL_3
#define GLM_FORCE_RADIANS

#include "Glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

#include "../Core/Input.h"

class RenderWindow
{
public:
	// Constructor
    RenderWindow();
	// Initialisation Constructor
    RenderWindow(const std::string &windowName, const int &width, const int &height, bool fullscreen = true);
	// Deinitialisation
    ~RenderWindow();

	// Intialise
	bool Initialise(const std::string &windowName, const int &width, const int &height, bool fullscreen = true);
	//Set the Clear Colour
	void ClearColor(float r, float g, float b, float a);
    // Clear window
	void Clear();
	// Poll input events
    void PollEvents();
	// Display window
	void Display();
	// Check if windows is opened
    bool IsOpened() const;

	glm::vec2 GetWindowSize() const;

	// Get window handle from GLFW
	GLFWwindow* GetWindowHandle();

	// Callbacks
    static void ErrorCallback(int error, const char *description);
	static void CloseWindowCallback(GLFWwindow* window);
	static void ResizeCallback(GLFWwindow* window, int width, int height);
	glm::vec2 GetMousePosition();

	// Input
    bool IsKeyPressed(Key key);
	bool IsKeyReleased(Key key);
	bool IsMousePressed(Mouse button);
	bool IsMouseReleased(Mouse button);
	
private:
    GLFWwindow          *mRenderWindow;
};

#endif // _RENDER_WINDOW_H_
