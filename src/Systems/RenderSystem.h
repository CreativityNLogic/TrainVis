#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <entityx/System.h>
#include <algorithm>
#include <map>
#include "../Components/TransformComponent.h"
#include "../Components/GraphicsComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/LightComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/FogComponent.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/Cubemap.h"
#include "../Graphics/WeightedSumRendering.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
public:
	RenderSystem() : mCamera(nullptr) 
	{
		// load textures
		std::vector<std::string> faces
		{
			("../../assets/textures/skybox/right.jpg"),
			("../../assets/textures/skybox/left.jpg"),
			("../../assets/textures/skybox/up.jpg"),
			("../../assets/textures/skybox/down.jpg"),
			("../../assets/textures/skybox/back.jpg"),
			("../../assets/textures/skybox/front.jpg")
		};

		mSkybox.LoadFromFile(faces, false);

		mWeightSumRenderer.Initialise(1920, 1080);
	}

	~RenderSystem() 
	{
		mWeightSumRenderer.TerminateAccumulations();
	}
	
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

	void setFogParams(FogComponent &fog)
	{
		mFog = fog;
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		if (mCamera == nullptr)
			return;

		unsigned int lightCount = 0;
		const unsigned MAXLIGHTS = 3;

		std::vector<entityx::Entity> lightEntities;

		auto lights = es.entities_with_components<TransformComponent, LightComponent>();
		for (auto entity : lights)
		{
			if (lightCount > MAXLIGHTS)
				break;

			lightEntities.push_back(entity);

			lightCount++;
		}

		mWeightSumRenderer.RenderWeightedSum(es, mCamera);

		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetLights(lightEntities, matComp.Materials);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);

			graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
			graphic.Model.SetView(mCamera->GetViewMatrix());
			graphic.Model.SetViewPosition(mCamera->GetPosition());

			graphic.Model.SetFogParams(mFog);

			graphic.Model.Draw(matComp.Materials);
		});
		
		// NEED TO STOP THIS DOUBLE RENDER!
		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetLights(lightEntities, matComp.Materials);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);

			graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
			graphic.Model.SetView(mCamera->GetViewMatrix());
			graphic.Model.SetViewPosition(mCamera->GetPosition());

			graphic.Model.SetFogParams(mFog);

			graphic.Model.Draw(matComp.Materials);
		});

		mSkybox.SetProjection(mCamera->GetProjectionMatrix());
		mSkybox.SetView(mCamera->GetViewMatrix());
		mSkybox.SetFogParams(mFog);
		mSkybox.Draw();
	};	
	
private:
	Camera *mCamera;
	Cubemap mSkybox;
	FogComponent mFog;
	WeightedSumRendering mWeightSumRenderer;
};

#endif // RENDERSYSTEM_H