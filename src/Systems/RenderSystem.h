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
#include "../Graphics/Model.h"
class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
public:
	RenderSystem() : mCamera(nullptr) 
	{
		mWeightSumRenderer.Initialise(1920, 1080, glm::vec3(0.2f, 0.3f, 0.3f));
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
		mWeightSumRenderer.SetFogParams(fog);
	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override
	{
		if (mCamera == nullptr)
			return;

		mWeightSumRenderer.RenderWeightedSum(es, mCamera);
	};

private:
	Camera *mCamera;
	WeightedSumRendering mWeightSumRenderer;
};

#endif // RENDERSYSTEM_H