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
#include "../Graphics/Camera.h"
#include "../Graphics/Sprite.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
public:
	RenderSystem() : mCamera(nullptr) 
	{
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
		for (auto entity : lights)
		{
			if (lightCount > MAXLIGHTS)
				break;

			lightEntities.push_back(entity);

			lightCount++;
		}

		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetLights(lightEntities, matComp.Materials);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);
			
			if (mCamera != nullptr)
			{
				graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
				graphic.Model.SetView(mCamera->GetViewMatrix());
				graphic.Model.SetViewPosition(mCamera->GetPosition());
			}

			graphic.Model.Draw(matComp.Materials);
		});

		// NEED TO STOP THIS DOUBLE RENDER!
		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetLights(lightEntities, matComp.Materials);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);

			if (mCamera != nullptr)
			{
				graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
				graphic.Model.SetView(mCamera->GetViewMatrix());
				graphic.Model.SetViewPosition(mCamera->GetPosition());
			}

			graphic.Model.Draw(matComp.Materials);
		});

		if (mCamera != nullptr)
		{
			auto &sprites = es.entities_with_components<TransformComponent, SpriteComponent>();
			std::vector<entityx::Entity> spriteList(sprites.begin(), sprites.end());
			std::sort(spriteList.begin(), spriteList.end(), [=](entityx::Entity a, entityx::Entity b)
			{
				glm::vec3 camPos = mCamera->GetPosition();
				auto &transA = a.component<TransformComponent>();
				auto &transB = b.component<TransformComponent>();

				glm::vec3 aVector = transA->Position - camPos;
				glm::vec3 bVector = transB->Position - camPos;

				return glm::length(aVector) < glm::length(bVector);
			});

			std::vector<entityx::Entity>::reverse_iterator iter = spriteList.rbegin();
			for (; iter != spriteList.rend(); iter++)
			{
				auto &sprite = iter->component<SpriteComponent>();
				auto &transform = iter->component<TransformComponent>();

				sprite->Sprite.SetPosition(transform->Position);
				sprite->Sprite.SetRotation(transform->Rotation);
				sprite->Sprite.SetScale(transform->Scale);

				sprite->Sprite.SetProjection(mCamera->GetProjectionMatrix());
				sprite->Sprite.SetView(mCamera->GetViewMatrix());

				sprite->Sprite.Draw();
			}
		}
	}

private:
	Camera *mCamera;
};

#endif // RENDERSYSTEM_H