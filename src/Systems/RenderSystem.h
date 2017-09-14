#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <entityx/System.h>
#include "../Components/TransformComponent.h"
#include "../Components/GraphicsComponent.h"

#include "../Graphics/Camera.h"

class RenderSystem : public entityx::System<RenderSystem>
{
public:
	RenderSystem() : mCamera(nullptr) {}
	~RenderSystem() {}

	void setCamera(Camera *camera) { mCamera = camera; }

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		es.each<TransformComponent, GraphicsComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic)
		{
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);
			
			if (mCamera != nullptr)
			{
				graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
				graphic.Model.SetView(mCamera->GetViewMatrix());
			}

			graphic.Model.Draw();
		});
	};

private:
	Camera *mCamera;
};

#endif // RENDERSYSTEM_H