#include <iostream>

#include "Application.h"

#include "Graphics/RenderWindow.h"

#include "Systems/PhysicSystem.h"

#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"

Application::Application()
{
	//-------------------------------------
	// Place declaration variables here (does not need error checking eg someSting = "Test";)

	//-------------------------------------
}

bool Application::Initialise()
{
	if (!mRenderWindow->Initialise("TrainVis", 1280, 720, false))
		return false;
	//-------------------------------------
	// Place declaration variables or initialise objects that need error checking eg if(!obj.init())

	systems.add<PhysicSystem>();
	systems.configure();

	entityx::Entity entity = entities.create();
	entity.assign<TransformComponent>();
	entity.assign<RigidBodyComponent>();

	//-------------------------------------
	return true;
}

void Application::Terminate()
{
	//-------------------------------------
	// Any objects that requires clean up

	//-------------------------------------
}

void Application::Update(double deltaTime)
{
	if (mRenderWindow->IsKeyPressed(Key::ESCAPE))
		Quit();

	mRenderWindow->Clear(0.2f, 0.3f, 0.3f, 1.0f);

	//-------------------------------------
	// Place code here which will update per frame

	systems.update_all(deltaTime);

	//-------------------------------------

	mRenderWindow->PollEvents();
	mRenderWindow->Display();
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