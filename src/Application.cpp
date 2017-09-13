#include <iostream>

#include "Application.h"

#include "Graphics/RenderWindow.h"
#include "Graphics/Camera.h"

#include "Systems/PhysicSystem.h"
#include "Systems/RenderSystem.h"

#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/GraphicsComponent.h"

#include "Physics/RigidBody.h"

#include <iostream>

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 25.0f));
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 25.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// settings used for camera
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool firstMouseInApp = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec2 oldMousePos = glm::vec2();

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

	mPhysicWorld.reset(new RigidWorld());

	systems.add<PhysicSystem>();
	systems.add<RenderSystem>()->setCamera(&camera);
	systems.configure();

	btCollisionShape *shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	float mass = 1.0f;
	btVector3 fallInertia(0, 0, 0);
	if(mass > 0.0f)
		shape->calculateLocalInertia(mass, fallInertia);

	btTransform transform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 25.0f, 0.0f));

	entityx::Entity entity = entities.create();
	entityx::ComponentHandle<TransformComponent> tranform = entity.assign<TransformComponent>();
	tranform->Position = glm::vec3(0.0f, 25.0f, 0.0f);

	entity.assign<GraphicsComponent>("../../assets/models/Cube.fbx");
	RigidBody *body = new RigidBody(mass, new btDefaultMotionState(transform), shape, glm::vec3(fallInertia.x(), fallInertia.y(), fallInertia.z()));
	entity.assign<RigidBodyComponent>()->Body = body;
	mPhysicWorld->addRigidBody(body);

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
	mPhysicWorld->stepSimulation(deltaTime, 10);

	//-----------------------------------
	// Camera 

	float cameraSpeed = 6.5f * (float)deltaTime; // adjust accordingly
	if (mRenderWindow->IsKeyPressed(Key::W))
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::S))
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::A))
		camera.ProcessKeyboard(LEFT_CAM, (float)deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::D))
		camera.ProcessKeyboard(RIGHT_CAM, (float)deltaTime);

	glm::vec2 newMousePos = mRenderWindow->GetMousePosition();
	glm::vec2 mouseDelta(newMousePos.x - oldMousePos.x, newMousePos.y - oldMousePos.y);
	camera.ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
	oldMousePos = newMousePos;
	
	//-----------------------------------
	// Update systems
	systems.update_all(deltaTime);

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
