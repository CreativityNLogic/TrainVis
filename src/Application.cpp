#include "Application.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/Camera.h"
#include <iostream>

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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

	//mTrainMaterial.reset(new Material("../../assets/shaders/base.vs", "../../assets/shaders/base.fs"));
	mTrainModel.reset(new Model("../../assets/models/Train.fbx"));

	//-----------------------------------
	// Projection Matrix - You can even switch this with glm::orthographic
	// 90.0f - FOV
	// 4.0f / 3.0f - Aspect Ratio
	// 0.1f - Near Clip Distance
	// 100.0f - Far Clip Distance
	mTrainModel->SetProjection(glm::perspective(90.0f, 4.0f / 3.0f, 0.01f, 10000.0f));
	mTrainModel->SetRotation(glm::vec3(-90.0f, 0.0f, 90.0f));
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
	
	//----Camera 

	float cameraSpeed = 6.5f * (float)deltaTime; // adjust accordingly
	if (mRenderWindow->IsKeyPressed(Key::W))
		cameraPos += cameraSpeed * cameraFront;
	if (mRenderWindow->IsKeyPressed(Key::S))
		cameraPos -= cameraSpeed * cameraFront;
	if (mRenderWindow->IsKeyPressed(Key::A))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (mRenderWindow->IsKeyPressed(Key::D))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	mRenderWindow->Clear(0.2f, 0.3f, 0.3f, 1.0f);

	//===============================================
	// Place code here which will update per frame
	//===============================================
	
	//-----------------------------------
	// Use this shader
	//mTrainMaterial->Bind();

	//-----------------------------------
	// THE CAMERA aka ViewMatrix
	// glm::vec3(3, 0, 3) - Position of camera
	// glm::vec3(0, 0, 0) - Look at target
	// glm::vec3(0, 1, 0) - Up vector
	mTrainModel->SetView(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));

	//-----------------------------------
	// ModelMatrix
	// Translation, Scale and rotation of object all in one matrix
	//mTrainModel->SetTransform(glm::mat4(1.0f));

	//-----------------------------------
	// Draw model
	mTrainModel->Draw();

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