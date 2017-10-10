#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <entityx/System.h>
#include "../Components/TransformComponent.h"
#include "../Components/GraphicsComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Cubemap.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
public:
	RenderSystem() : 
		mCamera(nullptr)
	{
		// load textures
		std::vector<std::string> faces;
		faces.push_back("../../assets/textures/skybox/right.jpg");
		faces.push_back("../../assets/textures/skybox/left.jpg");
		faces.push_back("../../assets/textures/skybox/up.jpg");
		faces.push_back("../../assets/textures/skybox/down.jpg");
		faces.push_back("../../assets/textures/skybox/back.jpg");
		faces.push_back("../../assets/textures/skybox/front.jpg");

		mSkybox.LoadFromFile(faces, false);
	}

	~RenderSystem() {}
	
	void configure(entityx::EventManager &event_manager) 
	{
		event_manager.subscribe<entityx::ComponentAddedEvent<CameraComponent>>(*this);
	}

	void receive(const entityx::ComponentAddedEvent<CameraComponent> &comp)
	{
		entityx::ComponentHandle<CameraComponent> camera = comp.component;
		setCamera(&camera->Camera);
	}

	void setCamera(Camera *camera) { mCamera = camera; }

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		unsigned int lightCount = 0;
		const unsigned MAXLIGHTS = 3;
		
		std::vector<entityx::Entity> lightEntities;

		auto lights = es.entities_with_components<TransformComponent, LightComponent>();
		for(auto entity : lights)
		{
			if (lightCount > MAXLIGHTS)
				break;

			lightEntities.push_back(entity);

			lightCount++;
		}

		es.each<TransformComponent, GraphicsComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic)
		{
			graphic.Model.SetLights(lightEntities);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);
			
			if (mCamera != nullptr)
			{
				graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
				graphic.Model.SetView(mCamera->GetViewMatrix());
				graphic.Model.SetViewPosition(mCamera->GetPosition());
			}

			graphic.Model.Draw();
		});

		if (mCamera != nullptr)
		{
			mSkybox.SetView(mCamera->GetViewMatrix());
			mSkybox.SetProjection(mCamera->GetProjectionMatrix());
		}

		mSkybox.Draw();
	};	

private:
	Camera *mCamera;
	Cubemap mSkybox;
};

#endif // RENDERSYSTEM_H