#ifndef WEIGHTEDSUMRENDERING_H
#define WEIGHTEDSUMRENDERING_H

#include "Glad/glad.h"
#include <iostream>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/FogComponent.h"


class WeightedSumRendering
{
public:
	WeightedSumRendering() :
		mDrawBuffers
	{	
		GL_COLOR_ATTACHMENT0_EXT,
		GL_COLOR_ATTACHMENT1_EXT,
		GL_COLOR_ATTACHMENT2_EXT,
		GL_COLOR_ATTACHMENT3_EXT,
		GL_COLOR_ATTACHMENT4_EXT,
		GL_COLOR_ATTACHMENT5_EXT,
		GL_COLOR_ATTACHMENT6_EXT
	},
		mMaxDepth(1.0f),
		mMaxPass(4)
	{
		mBackgroundColour = glm::vec3(0.2f, 0.3f, 0.3f);
	}

	void Initialise(unsigned int width, unsigned int height)
	{
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
	}

	void TerminateAccumulations()
	{
		glDeleteFramebuffers(1, &mAccumulationFBO);
		glDeleteTextures(2, mAccumulationTextureID);
	}

	void InitaliseShaders()
	{
		std::string path = "../../assets/shaders/";

		mWeightedSumInitShader.Initialise(path + "wsumInit.vs", path + "wsumInit.fs");
		mWeightedSumFinalShader.Initialise(path + "wsumFinal.vs", path + "wsumFinal.fs");
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
		glDisable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, mAccumulationFBO);
		glDrawBuffer(mDrawBuffers[0]);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);

		mWeightedSumInitShader.Bind();

		es.each<TransformComponent, SpriteComponent>([=](entityx::Entity entity, TransformComponent &transform, SpriteComponent &sprite)
		{
			sprite.Sprite.SetPosition(transform.Position);
			sprite.Sprite.SetRotation(transform.Rotation);
			sprite.Sprite.SetScale(transform.Scale);

			sprite.Sprite.SetProjection(camera->GetProjectionMatrix());
			sprite.Sprite.SetView(camera->GetViewMatrix());

			sprite.Sprite.Draw();
		});

		glDisable(GL_BLEND);

		GLuint err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);

		mWeightedSumFinalShader.Bind();

		mWeightedSumFinalShader.setInt("ColorTex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[0]);

		mWeightedSumFinalShader.setVec3("BackgroundColor", mBackgroundColour);
		
		DrawQuad();

		err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
	}

	void Resize(int width, int height)
	{
		TerminateAccumulations();
		InitialiseAccumulations(width, height);
	}

private:
	GLuint mQuadVAO;
	GLuint mQuadVBO;

	GLuint mAccumulationTextureID[2];
	GLuint mAccumulationFBO;

	Shader mWeightedSumInitShader;
	Shader mWeightedSumFinalShader;

	const GLenum mDrawBuffers[7];

	GLfloat mMaxDepth;
	GLuint mMaxPass;

	glm::vec3 mBackgroundColour;
};

#endif // WEIGHTEDSUMRENDERING_H