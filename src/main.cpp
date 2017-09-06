#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>

void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void errorCallback(int error, const char* description)
{
	std::cout << "Error: " << description << std::endl;
}

void handleInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(int argc, char **argv)
{
	//-------------------------------------------------------------------------
	GLFWwindow *window;

	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
	{
		std::cout << "Cannot initialise GLFW!" << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(960, 540, "Train Vis", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Cannot initialise window!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Cannot initialise GLAD!" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	//-------------------------------------------------------------------------
	//	PLACE INITIAL CODE HERE
	//-------------------------------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		handleInput(window);

		// Change background colour here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//-------------------------------------------------------------------------
		// PLACE LOOP CODE HERE
		//-------------------------------------------------------------------------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
