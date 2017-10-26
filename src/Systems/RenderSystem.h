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
#include "../Graphics/Cubemap.h"

class RenderSystem : public entityx::System<RenderSystem>, public entityx::Receiver<RenderSystem>
{
public:
	RenderSystem() : mCamera(nullptr) 
	{
		// Load textures
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
		
		// Build and compile shaders
		Shader shader("lighting.vs", "lighting.fs");
		Shader hdrShader("hdr.vs", "hdr.fs");

		// Configure floating point framebuffer
		glGenFramebuffers(1, &hdrFBO);

		// Create floating point color buffer
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Create depth buffer
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);

		// Attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetLights(lightEntities, matComp.Materials);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);
			
			graphic.Model.SetProjection(mCamera->GetProjectionMatrix());
			graphic.Model.SetView(mCamera->GetViewMatrix());
			graphic.Model.SetViewPosition(mCamera->GetPosition());

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

			graphic.Model.Draw(matComp.Materials);
		});

		// Render transparent sprites
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

		mSkybox.SetProjection(mCamera->GetProjectionMatrix());
		mSkybox.SetView(mCamera->GetViewMatrix());
		mSkybox.Draw();

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

		// Render scene into floating point framebuffer

		// ...

		// Render floating point color buffer and tonemap HDR colors

		// ...

	};	
	
private:
	Camera *mCamera;
	Cubemap mSkybox;
	unsigned int hdrFBO, colorBuffer, rboDepth;
	Shader shader, hdrShader;
};

#endif // RENDERSYSTEM_H