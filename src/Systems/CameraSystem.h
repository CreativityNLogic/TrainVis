#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <entityx/System.h>
#include "../Components/CameraComponent.h"
#include "../Graphics/RenderWindow.h"

class CameraSystem : public entityx::System<CameraSystem>
{
public:
	CameraSystem() : mRenderWindow(nullptr) {}
	CameraSystem(RenderWindow *renderWindow) : mRenderWindow(renderWindow) {}
	~CameraSystem() {}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		if (mRenderWindow != nullptr)
		{
			es.each<CameraComponent>([=](entityx::Entity entity, CameraComponent &camera)
			{
				if (mRenderWindow->IsKeyPressed(Key::W))
					camera.Camera.ProcessKeyboard(FORWARD, (float)dt);

				if (mRenderWindow->IsKeyPressed(Key::S))
					camera.Camera.ProcessKeyboard(BACKWARD, (float)dt);

				if (mRenderWindow->IsKeyPressed(Key::A))
					camera.Camera.ProcessKeyboard(LEFT_CAM, (float)dt);

				if (mRenderWindow->IsKeyPressed(Key::D))
					camera.Camera.ProcessKeyboard(RIGHT_CAM, (float)dt);

				glm::vec2 newMousePos = mRenderWindow->GetMousePosition();
				glm::vec2 mouseDelta(newMousePos.x - mOldMousePos.x, newMousePos.y - mOldMousePos.y);
				camera.Camera.ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
				mOldMousePos = newMousePos;
			});
		}
	};

private:
	glm::vec2 mOldMousePos;
	RenderWindow *mRenderWindow;
};

#endif // CAMERASYSTEM_H