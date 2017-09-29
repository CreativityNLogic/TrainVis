#include <iostream>

#include "Application.h"

#include "Graphics/RenderWindow.h"

#include "Systems/PhysicSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/CameraSystem.h"

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
	mPhysicWorld.reset(new RigidWorld());
	mEntityFactory.reset(new EntityFactory(entities, mPhysicWorld.get()));

	systems.add<CameraSystem>(mRenderWindow.get());
	systems.add<MovementSystem>(mRenderWindow.get());
	systems.add<PhysicSystem>();
	systems.add<RenderSystem>()->setCamera(mCamera.get());
	systems.configure();

	mEntityFactory->createFromLevelFile("../../assets/scenes/TrainVis.scene");

	entityx::Entity entity = entities.create();
	auto &cameraComp = entity.assign<CameraComponent>(glm::vec3(0.0f, 5.0f, 60.0f), 20.0f, 0.1f);
	cameraComp->Camera.SetYaw(180.0f);
	cameraComp->Camera.SetProjection(90.0f, (float)mRenderWindow->GetWindowSize().x / (float)mRenderWindow->GetWindowSize().y, 0.01f, 100000.0f);

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
	mPhysicWorld->stepSimulation(1.0f/60.0f, 1);
	
	//-----------------------------------
	// Update systems
	systems.update<CameraSystem>(deltaTime);
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
