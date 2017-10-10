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
	RenderSystem() : mCamera(nullptr) {
		mCubeModel.LoadFromFile("../../assets/model/Cube.fbx");
		Shader mCubemapShader("skyboxVert.vs", "skyboxFrag.fs");
	

		// load textures
		std::vector<std::string> faces
		{
			("bin/assets/textures/skybox/right.jpg"),
			("bin/assets/textures/skybox/left.jpg"),
			("bin/assets/textures/skybox/top.jpg"),
			("bin/assets/textures/skybox/bottom.jpg"),
			("bin/assets/textures/skybox/back.jpg"),
			("bin/assets/textures/skybox/front.jpg")
		};

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
	
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		mCubemapShader.use();
		mSkybox.Bind(0);
		mCubemapShader.setInt("skybox", 0);

		if (mCamera != nullptr) {
			glm::mat4 view = glm::mat4(glm::mat3(mCamera->GetViewMatrix())); // remove translation from the view matrix
			mCubemapShader.setMat4("view", view);
			glm::mat4 projection = glm::mat4(mCamera->GetProjectionMatrix());
			mCubemapShader.setMat4("projection", projection);
		}
		glDepthFunc(GL_LESS);
	};	

private:
	Camera *mCamera;
	Model mCubeModel;
	Cubemap mSkybox;
	Shader mCubemapShader;


};

#endif // RENDERSYSTEM_H