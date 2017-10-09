#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <entityx/System.h>
#include "../Components/TransformComponent.h"
#include "../Components/GraphicsComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/LightComponent.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Cubemap.h"
#include "../Graphics/CubemapShader.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
public:
	RenderSystem() : mCamera(nullptr) {
		mCubeModel.LoadFromFile("../../assets/model/Cube.fbx");

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
		mCubemapShader.use();
		mCubemap.Bind(0);
		mCubeModel.Draw();


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
	};

	
private:
	Camera *mCamera;
	Model mCubeModel;
	Cubemap mCubemap;
	CubemapShader mCubemapShader;
};

#endif // RENDERSYSTEM_H