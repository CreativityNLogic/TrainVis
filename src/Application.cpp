#include "Application.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/Camera.h"
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
	mRenderWindow->Clear(0.2f, 0.3f, 0.3f, 1.0f);
	
	//----Camera 
	//glfwSetCursorPosCallback(mRenderWindow, mouse_callback);

	float cameraSpeed = 6.5f * deltaTime; // adjust accordingly
	if (mRenderWindow->IsKeyPressed(Key::W))
		camera.ProcessKeyboard(FORWARD, deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::S))
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::A))
		camera.ProcessKeyboard(LEFT_CAM, deltaTime);

	if (mRenderWindow->IsKeyPressed(Key::D))
		camera.ProcessKeyboard(RIGHT_CAM, deltaTime);

	//glfwMakeContextCurrent(mRenderWindow);
	//glfwSetCursorPosCallback(mRenderWindow, mouse_callback);
	//camera.ProcessMouseMovement(xoffset, yoffset);
	
	mRenderWindow->GetMousePosition().x;
	mRenderWindow->GetMousePosition().y;
	
	
	glm::vec2 newMousePos = mRenderWindow->GetMousePosition();

	glm::vec2 mouseDelta(newMousePos.x - oldMousePos.x, 
		newMousePos.y - oldMousePos.y);

	//mouse_callback(mouseDelta.x, mouseDelta.y);
	camera.ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
	oldMousePos = newMousePos;
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
	//mTrainModel->SetView(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	mTrainModel->SetView(camera.GetViewMatrix());
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

void Application::mouse_callback(double xpos, double ypos)
{
	if (firstMouseInApp)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseInApp = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



void mouse_callback(double xpos, double ypos) {
	if (firstMouseInApp)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseInApp = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
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
