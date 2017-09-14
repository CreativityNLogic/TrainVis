#include <iostream>

#include "Application.h"

#include "Graphics/RenderWindow.h"

#include "Systems/PhysicSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"

#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/GraphicsComponent.h"

#include "Physics/RigidBody.h"

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
	if (!mRenderWindow->Initialise("TrainVis", 1920, 1080, false))
		return false;

	//===============================================
	// Place declaration variables or initialise objects that need error checking eg if(!obj.init())
	//===============================================
	mCamera.reset(new Camera(glm::vec3(0.0f, 5.0f, 60.0f), 20.0f, 0.1f));
	mPhysicWorld.reset(new RigidWorld());
	mEntityFactory.reset(new EntityFactory(entities, mPhysicWorld.get()));

	systems.add<MovementSystem>(mRenderWindow.get());
	systems.add<PhysicSystem>();
	systems.add<RenderSystem>()->setCamera(mCamera.get());
	systems.configure();

	mEntityFactory->createFromLevelFile("../../assets/scenes/TrainVis.scene");

	mCamera->SetYaw(180.0f);
	mCamera->SetProjection(90.0f, (float)mRenderWindow->GetWindowSize().x / (float)mRenderWindow->GetWindowSize().y, 0.01f, 100000.0f);
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
	// Step physics simulation
	mPhysicWorld->stepSimulation(deltaTime, 1);

	//-----------------------------------
	// Camera 
	if (mRenderWindow->IsKeyPressed(Key::W))
		mCamera->ProcessKeyboard(FORWARD, (float)deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::S))
		mCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::A))
		mCamera->ProcessKeyboard(LEFT_CAM, (float)deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::D))
		mCamera->ProcessKeyboard(RIGHT_CAM, (float)deltaTime);

	glm::vec2 newMousePos = mRenderWindow->GetMousePosition();
	glm::vec2 mouseDelta(newMousePos.x - mOldMousePos.x, newMousePos.y - mOldMousePos.y);
	mCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
	mOldMousePos = newMousePos;
	
	//-----------------------------------
	// Update systems
	systems.update<PhysicSystem>(deltaTime);
	systems.update<RenderSystem>(deltaTime);
	systems.update<MovementSystem>(deltaTime);
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
