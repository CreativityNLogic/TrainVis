#ifndef WEIGHTEDSUMRENDERING_H
#define WEIGHTEDSUMRENDERING_H

#include "Glad/glad.h"
#include <iostream>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/FogComponent.h"


class WeightedSumRendering
{
public:
	WeightedSumRendering() :
		mMaxDepth(1.0f),
		mMaxPass(5),
		mReflectionShader("../../assets/shaders/cubemap.vs", "../../assets/shaders/cubemap.fs")
	{
		mCube.LoadFromFile("../../assets/models/Cube.fbx");
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
	}

	void Initialise(unsigned int width, unsigned int height, glm::vec3 background)
	{
		mBackgroundColour = background;

		InitialiseAccumulations(width, height);

		InitaliseShaders();
		InitialiseQuad();
	}


	void Terminate()
	{
		TerminateAccumulations();
		TerminateShaders();
	}

	void InitialiseQuad()
	{
		float quadVertices[] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		glGenVertexArrays(1, &mQuadVAO);
		glBindVertexArray(mQuadVAO);

		glGenBuffers(1, &mQuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	void DrawQuad()
	{
		glBindVertexArray(mQuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void InitialiseAccumulations(unsigned int width, unsigned int height)
	{
		glGenTextures(2, mAccumulationTextureID);

		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);

		glGenFramebuffers(1, &mAccumulationFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mAccumulationFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAccumulationTextureID[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mAccumulationTextureID[1], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);  

		glGenFramebuffers(1, &mSceneFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mSceneFBO);

		glGenTextures(1, &mSceneTexture);
		glBindTexture(GL_TEXTURE_2D, mSceneTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSceneTexture, 0);

		glGenRenderbuffers(1, &mSceneRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, mSceneRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080); 
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mSceneRBO); 
																							
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void TerminateAccumulations()
	{
		glDeleteFramebuffers(1, &mAccumulationFBO);
		glDeleteTextures(2, mAccumulationTextureID);
	}

	void InitaliseShaders()
	{
		mWeightedSumInitShader.Initialise("../../assets/shaders/wsumInit.vs", "../../assets/shaders/wsumInit.fs");
		mWeightedSumFinalShader.Initialise("../../assets/shaders/wsumFinal.vs", "../../assets/shaders/wsumFinal.fs");
	}

	void TerminateShaders()
	{
		mWeightedSumInitShader.Terminate();
		mWeightedSumFinalShader.Terminate();
	}

	void ReloadShaders()
	{
		TerminateShaders();
		InitaliseShaders();
	}

	void RenderWeightedSum(entityx::EntityManager &es, Camera *camera)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mSceneFBO);
		glEnable(GL_DEPTH_TEST); 

		glClearColor(mBackgroundColour.r, mBackgroundColour.g, mBackgroundColour.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

			graphic.Model.SetProjection(camera->GetProjectionMatrix());
			graphic.Model.SetView(camera->GetViewMatrix());
			graphic.Model.SetViewPosition(camera->GetPosition());

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

			graphic.Model.SetProjection(camera->GetProjectionMatrix());
			graphic.Model.SetView(camera->GetViewMatrix());
			graphic.Model.SetViewPosition(camera->GetPosition());

			graphic.Model.SetFogParams(mFog);

			graphic.Model.Draw(matComp.Materials);
		});

		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetLights(lightEntities, matComp.Materials);
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);

			graphic.Model.SetProjection(camera->GetProjectionMatrix());
			graphic.Model.SetView(camera->GetViewMatrix());
			graphic.Model.SetViewPosition(camera->GetPosition());

			//graphic.Model.SetFogParams(mFog);

			graphic.Model.Draw(matComp.Materials);
		});

		mReflectionShader.setMat4("MVP", mCube.GetModel() * mCube.GetView() * mCube.GetProjection());
		mReflectionShader.setVec3("cameraPosition", camera->GetPosition());
		mCube.SetView(camera->GetViewMatrix());
		mCube.SetProjection(camera->GetProjectionMatrix());
		mReflectionShader.setInt("skybox", 0);
		mSkybox.Bind(0);
		mCube.Draw();
		glActiveTexture(GL_TEXTURE0);
		glDepthFunc(GL_LESS);





		mSkybox.SetProjection(camera->GetProjectionMatrix());
		mSkybox.SetView(camera->GetViewMatrix());
		mSkybox.SetFogParams(mFog);
		mSkybox.Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, mAccumulationFBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		es.each<TransformComponent, SpriteComponent>([=](entityx::Entity entity, TransformComponent &transform, SpriteComponent &sprite)
		{
			sprite.Sprite.SetPosition(transform.Position);
			sprite.Sprite.SetRotation(transform.Rotation);
			sprite.Sprite.SetScale(transform.Scale);

			sprite.Sprite.SetProjection(camera->GetProjectionMatrix());
			sprite.Sprite.SetView(camera->GetViewMatrix());

			sprite.Sprite.SetFogParams(mFog);

			sprite.Sprite.Draw();
		});

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);

		mWeightedSumFinalShader.Bind();

		mWeightedSumFinalShader.setInt("ColorTex", 0);
		mWeightedSumFinalShader.setInt("SceneTex", 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mSceneTexture);

		mWeightedSumFinalShader.setVec3("BackgroundColor", mBackgroundColour);
		
		DrawQuad();
	}

	void Resize(int width, int height)
	{
		TerminateAccumulations();
		InitialiseAccumulations(width, height);
	}

	void SetFogParams(FogComponent fog)
	{
		mFog = fog;
	}

	void SetBackground(glm::vec3 background)
	{
		mBackgroundColour = background;
	}

private:
	GLuint mQuadVAO;
	GLuint mQuadVBO;

	GLuint mSceneFBO;
	GLuint mSceneRBO;
	GLuint mSceneTexture;

	GLuint mAccumulationTextureID[2];
	GLuint mAccumulationFBO;

	Shader mWeightedSumInitShader;
	Shader mWeightedSumFinalShader;

	GLfloat mMaxDepth;
	GLuint mMaxPass;

	glm::vec3 mBackgroundColour;

	Cubemap mSkybox;
	FogComponent mFog;

	Shader mReflectionShader;
	Model mCube;
};

#endif // WEIGHTEDSUMRENDERING_H