#include "Application.h"
#include "Graphics/RenderWindow.h"

#include <iostream>

Application::Application()
{
	//===============================================
	// Place declaration variables here (does not need error checking eg someSting = "Test";)
	//===============================================


	//===============================================
}

bool Application::Initialise()
{
	if (!mRenderWindow->Initialise("TrainVis", 1280, 720, false))
		return false;

	//===============================================
	// Place declaration variables or initialise objects that need error checking eg if(!obj.init())
	//===============================================

	mTrainShader.reset(new Shader("../../assets/shaders/base.vs", "../../assets/shaders/base.fs"));
	mTrainModel.reset(new Model("../../assets/models/Train.fbx"));

	//===============================================
	return true;
}

void Application::Terminate()
{
	//===============================================
	// Any objects that requires clean up
	//===============================================

	//===============================================s
}

void Application::Update(double deltaTime)
{
	if (mRenderWindow->IsKeyPressed(Key::ESCAPE))
		Quit();

	mRenderWindow->Clear(0.2f, 0.3f, 0.3f, 1.0f);

	//===============================================
	// Place code here which will update per frame
	//===============================================
	
	//-----------------------------------
	// Use this shader
	mTrainShader->use();

	//-----------------------------------
	// Projection Matrix - You can even switch this with glm::orthographic
	// 90.0f - FOV
	// 4.0f / 3.0f - Aspect Ratio
	// 0.1f - Near Clip Distance
	// 100.0f - Far Clip Distance
	glm::mat4 projectionMatrix = glm::perspective(90.0f, 4.0f / 3.0f, 0.01f, 10000.0f);

	//-----------------------------------
	// THE CAMERA aka ViewMatrix
	// glm::vec3(3, 0, 3) - Position of camera
	// glm::vec3(0, 0, 0) - Look at target
	// glm::vec3(0, 1, 0) - Up vector
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//-----------------------------------
	// ModelMatrix
	// Translation, Scale and rotation of object all in one matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	//-----------------------------------
	// Model-View-Projection matrix. (MAKE SURE IT'S IN THIS ORDER!)
	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
	mTrainShader->setMat4("MVP", mvp);

	//-----------------------------------
	// Draw model
	mTrainModel->Draw(*mTrainShader.get());

	//===============================================

	mRenderWindow->Display();
	mRenderWindow->PollEvents();
}

// Do not add code here unless it requires console parameters
int main(int argc, char *argv)
{
	std::unique_ptr<BaseApplication> app(new Application());

	if (!app->Initialise())
	{
		std::cout << "Cannot initialise application!" << std::endl;
		return EXIT_FAILURE;
	}

	app->StartLoop();
	app->Terminate();

	return EXIT_SUCCESS;
}