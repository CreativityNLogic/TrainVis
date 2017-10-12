#ifndef DUALDEPTHPEELING_H
#define DUALDEPTHPEELING_H

#include "Glad/glad.h"
#include <iostream>

#include <entityx/entityx.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Sprite.h"
#include "../Components/GraphicsComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/MaterialComponent.h"


class DualDepthPeeling
{
public:
	DualDepthPeeling() :
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
		mSprite.LoadFromFile("../../assets/textures/smoke.png");
		//InitialiseDualPeeling(width, height);
		//InitialiseFrontPeeling(width, height);
		InitialiseAccumulations(width, height);

		InitaliseShaders();
		InitialiseQuad();
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

	void InitialiseDualPeeling(unsigned int width, unsigned int height)
	{
		glGenTextures(2, mDualDepthTextureID);
		glGenTextures(2, mDualFrontBlenderTextureID);
		glGenTextures(2, mDualBackTempTextureID);
		glGenFramebuffers(1, &mDualPeelingSingleFBO);

		for (int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mDualDepthTextureID[i]);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_FLOAT_RG32_NV, width, height, 0, GL_RGB, GL_FLOAT, 0);

			glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mDualFrontBlenderTextureID[i]);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);

			glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mDualBackTempTextureID[i]);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);
		}

		glGenTextures(1, &mDualBackBlenderTextureID);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mDualBackBlenderTextureID);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, width, height,	0, GL_RGB, GL_FLOAT, 0);

		glGenFramebuffersEXT(1, &mDualBackBlenderFBO);
		glBindFramebufferEXT(GL_FRAMEBUFFER, mDualBackBlenderFBO);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,	GL_TEXTURE_RECTANGLE_ARB, mDualBackBlenderTextureID, 0);

		glGenFramebuffers(GL_FRAMEBUFFER, &mDualPeelingSingleFBO);

		int j = 0;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, mDualDepthTextureID[j], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_RECTANGLE_ARB, mDualFrontBlenderTextureID[j], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_RECTANGLE_ARB, mDualBackTempTextureID[j], 0);

		j = 1;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_RECTANGLE_ARB, mDualDepthTextureID[j], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_RECTANGLE_ARB, mDualFrontBlenderTextureID[j], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_RECTANGLE_ARB, mDualBackTempTextureID[j], 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_RECTANGLE_ARB, mDualBackBlenderTextureID, 0);

		GLenum err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
	}

	void TerminateDualPeeling()
	{
		glDeleteFramebuffers(1, &mDualBackBlenderFBO);
		glDeleteFramebuffers(1, &mDualPeelingSingleFBO);
		glDeleteTextures(2, mDualDepthTextureID);
		glDeleteTextures(2, mDualFrontBlenderTextureID);
		glDeleteTextures(2, mDualBackTempTextureID);
		glDeleteTextures(1, &mDualBackBlenderTextureID);
	}

	void InitialiseFrontPeeling(unsigned int width, unsigned int height)
	{
		glGenTextures(2, mFrontDepthTextureID);
		glGenTextures(2, mFrontColourTextureID);
		glGenFramebuffersEXT(2, mFrontFBO);

		for (int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mFrontDepthTextureID[i]);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_DEPTH_COMPONENT32F_NV, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mFrontColourTextureID[i]);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height,	0, GL_RGBA, GL_FLOAT, 0);

			glGenFramebuffers(GL_FRAMEBUFFER, &mFrontFBO[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE_ARB, mFrontDepthTextureID[i], 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, mFrontColourTextureID[i], 0);
		}

		glGenTextures(1, &mFrontColourBlenderTextureID);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, mFrontColourBlenderTextureID);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height,	0, GL_RGBA, GL_FLOAT, 0);

		glGenFramebuffers(1, &mFrontColourBlenderFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrontColourBlenderFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_RECTANGLE_ARB, mFrontDepthTextureID[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, mFrontColourBlenderTextureID, 0);
		
		GLenum err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
	}

	void TerminateFrontPeeling()
	{
		glDeleteFramebuffersEXT(2, mFrontFBO);
		glDeleteFramebuffersEXT(1, &mFrontColourBlenderFBO);
		glDeleteTextures(2, mFrontDepthTextureID);
		glDeleteTextures(2, mFrontColourTextureID);
		glDeleteTextures(1, &mFrontColourBlenderTextureID);
	}

	void InitialiseAccumulations(unsigned int width, unsigned int height)
	{
		glGenTextures(2, mAccumulationTextureID);

		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

		glGenFramebuffers(1, &mAccumulationFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mAccumulationFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mAccumulationTextureID[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mAccumulationTextureID[1], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);  

		GLenum err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
	}

	void TerminateAccumulations()
	{
		glDeleteFramebuffers(1, &mAccumulationFBO);
		glDeleteTextures(2, mAccumulationTextureID);
	}

	void InitaliseShaders()
	{
		std::string path = "../../assets/shaders/";
		/*mDualInitShader.Initialise(path + "dualinit.vs", path + "dualinit.fs");
		mDualPeelShader.Initialise(path + "dualpeel.vs", path + "dualpeel.fs");
		mDualBlendShader.Initialise(path + "dualblend.vs", path + "dualblend.fs");
		mDualFinalShader.Initialise(path + "dualfinal.vs", path + "dualfinal.fs");

		mFrontInitShader.Initialise(path + "frontinit.vs", path + "frontinit.fs");
		mFrontPeelShader.Initialise(path + "frontpeel.vs", path + "frontpeel.fs");
		mFrontBlendShader.Initialise(path + "frontblend.vs", path + "frontblend.fs");
		mFrontFinalShader.Initialise(path + "frontfinal.vs", path + "frontfinal.fs");

		mAverageInitShader.Initialise(path + "avginit.vs", path + "avginit.fs");
		mAverageFinalShader.Initialise(path + "avgfinal.vs", path + "avgfinal.fs");*/

		mWeightedSumInitShader.Initialise(path + "wsumInit.vs", path + "wsumInit.fs");
		mWeightedSumFinalShader.Initialise(path + "wsumFinal.vs", path + "wsumFinal.fs");
	}

	void TerminateShaders()
	{
		/*mDualInitShader.Terminate();
		mDualPeelShader.Terminate();
		mDualBlendShader.Terminate();
		mDualFinalShader.Terminate();

		mFrontInitShader.Terminate();
		mFrontPeelShader.Terminate();
		mFrontBlendShader.Terminate();
		mFrontFinalShader.Terminate();

		mAverageInitShader.Terminate();
		mAverageFinalShader.Terminate();*/

		mWeightedSumInitShader.Terminate();
		mWeightedSumFinalShader.Terminate();
	}

	void ReloadShaders()
	{
		TerminateShaders();
		InitaliseShaders();
	}

	void RenderDualPeeling(entityx::EntityManager &es, Camera *camera)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glBindFramebuffer(GL_FRAMEBUFFER, mDualPeelingSingleFBO);
		glDrawBuffers(2, &mDrawBuffers[1]);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(mDrawBuffers[0]);
		glClearColor(-mMaxDepth, -mMaxDepth, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlendEquation(GL_MAX);

		mDualInitShader.Bind();

		es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);

			if (camera != nullptr)
			{
				graphic.Model.SetProjection(camera->GetProjectionMatrix());
				graphic.Model.SetView(camera->GetViewMatrix());
				graphic.Model.SetViewPosition(camera->GetPosition());
			}

			mDualInitShader.setMat4("MVP", graphic.Model.GetProjection() * graphic.Model.GetView() * graphic.Model.GetModel());

			mDualInitShader.setInt("DiffuseTexture", 0);
			if (matComp.Materials.size() > 0)
				matComp.Materials[0].getDiffuseTexture().Bind(0);

			graphic.Model.Draw();
		});

		GLenum err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;

		glDrawBuffer(mDrawBuffers[6]);
		glClearColor(mBackgroundColour.x, mBackgroundColour.y, mBackgroundColour.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		unsigned int currId = 0;
		for (unsigned int pass = 1; pass < mMaxPass; pass++)
		{
			currId = pass % 2;
			unsigned int prevId = 1 - currId;
			unsigned int bufId = currId * 3;

			glDrawBuffers(2, &mDrawBuffers[bufId + 1]);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawBuffer(mDrawBuffers[bufId + 0]);
			glClearColor(-mMaxDepth, -mMaxDepth, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawBuffers(3, &mDrawBuffers[bufId + 0]);
			glBlendEquation(GL_MAX);

			mDualPeelShader.Bind();

			mDualPeelShader.setInt("DepthBlenderTex", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mFrontDepthTextureID[prevId]);

			mDualPeelShader.setInt("FrontBlenderTex", 1);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, mDualFrontBlenderTextureID[prevId]);

			es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
			{
				graphic.Model.SetPosition(transform.Position);
				graphic.Model.SetRotation(transform.Rotation);
				graphic.Model.SetScale(transform.Scale);

				if (camera != nullptr)
				{
					graphic.Model.SetProjection(camera->GetProjectionMatrix());
					graphic.Model.SetView(camera->GetViewMatrix());
					graphic.Model.SetViewPosition(camera->GetPosition());
				}

				mWeightedSumInitShader.setMat4("MVP", graphic.Model.GetProjection() * graphic.Model.GetView() * graphic.Model.GetModel());

				mWeightedSumInitShader.setInt("DiffuseTexture", 0);
				if (matComp.Materials.size() > 0)
					matComp.Materials[0].getDiffuseTexture().Bind(0);

				graphic.Model.Draw();
			});

			err = glGetError();
			if (err)
				std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;

			glDrawBuffer(mDrawBuffers[6]);

			glBlendEquationEXT(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			mDualBlendShader.Bind();
			mDualBlendShader.setInt("TempTex", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mDualBackTempTextureID[currId]);

			DrawQuad();

			err = glGetError();
			if (err)
				std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
		}

		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);

		mDualFinalShader.Bind();

		mDualFinalShader.setInt("DepthBlenderTex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mFrontDepthTextureID[currId]);

		mDualFinalShader.setInt("FrontBlenderTex", 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, mDualFrontBlenderTextureID[currId]);

		mDualFinalShader.setInt("BackBlenderTex", 2);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, mDualBackBlenderTextureID);

		DrawQuad();

		err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
	}

	void RenderFrontToBackPeeling()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER, mFrontColourBlenderFBO);
		glDrawBuffer(mDrawBuffers[0]);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		mFrontInitShader.Bind();

		// DRAW MODELS

		mFrontInitShader.UnBind();

		GLenum err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;

		int numLayers = (mMaxPass - 1) * 2;
		for (int layer = 1; layer < numLayers; layer++) 
		{
			int currId = layer % 2;
			int prevId = 1 - currId;

			glBindFramebufferEXT(GL_FRAMEBUFFER, mFrontFBO[currId]);
			glDrawBuffer(mDrawBuffers[0]);

			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			mFrontPeelShader.Bind();

			mFrontPeelShader.setInt("DepthTex", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mFrontDepthTextureID[prevId]);

			// DRAW MODELS

			mFrontPeelShader.UnBind();

			err = glGetError();
			if (err)
				std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFrontColourBlenderFBO);
			glDrawBuffer(mDrawBuffers[0]);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			glBlendEquation(GL_FUNC_ADD);
			glBlendFuncSeparate(GL_DST_ALPHA, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

			mFrontBlendShader.Bind();

			mFrontBlendShader.setInt("TempTex", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mFrontColourTextureID[currId]);

			DrawQuad();

			mFrontBlendShader.UnBind();
			
			glDisable(GL_BLEND);

			err = glGetError();
			if (err)
				std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
		}

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glDrawBuffer(GL_BACK);
		glDisable(GL_DEPTH_TEST);

		mFrontFinalShader.Bind();

		mFrontFinalShader.setInt("ColorTex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mFrontColourBlenderTextureID);

		mFrontFinalShader.setVec3("BackgroundColor", mBackgroundColour);
		DrawQuad();

		mFrontFinalShader.UnBind();

		err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
	}

	void RenderAverageColors()
	{
		glDisable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, mAccumulationFBO);
		glDrawBuffers(2, mDrawBuffers);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);

		mAverageInitShader.Bind();

		DrawQuad();

		mAverageInitShader.UnBind();

		glDisable(GL_BLEND);

		GLuint err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);

		mAverageFinalShader.Bind();

		mAverageFinalShader.setInt("ColorTex0", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[0]);

		mAverageFinalShader.setInt("ColorTex1", 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, mAccumulationTextureID[1]);

		mAverageFinalShader.setVec3("BackgroundColor", mBackgroundColour);
		DrawQuad();

		mAverageFinalShader.UnBind();

		err = glGetError();
		if (err)
			std::cout << "Error: " << err << " at line " << __LINE__ << std::endl;
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

		if (camera != nullptr)
		{
			mSprite.SetProjection(camera->GetProjectionMatrix());
			mSprite.SetView(camera->GetViewMatrix());
		}

		mWeightedSumInitShader.setMat4("MVP", mSprite.GetProjection() * mSprite.GetView() * mSprite.GetModel());

		mSprite.Draw();

		/*es.each<TransformComponent, GraphicsComponent, MaterialComponent>([=](entityx::Entity entity, TransformComponent &transform, GraphicsComponent &graphic, MaterialComponent &matComp)
		{
			graphic.Model.SetPosition(transform.Position);
			graphic.Model.SetRotation(transform.Rotation);
			graphic.Model.SetScale(transform.Scale);
			
			if (camera != nullptr)
			{
				graphic.Model.SetProjection(camera->GetProjectionMatrix());
				graphic.Model.SetView(camera->GetViewMatrix());
				graphic.Model.SetViewPosition(camera->GetPosition());
			}

			mWeightedSumInitShader.setMat4("MVP", graphic.Model.GetProjection() * graphic.Model.GetView() * graphic.Model.GetModel());
			
			mWeightedSumInitShader.setInt("DiffuseTexture", 0);
			if (matComp.Materials.size() > 0)
				matComp.Materials[0].getDiffuseTexture().Bind(0);

			graphic.Model.Draw();
		});*/

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
		//TerminateDualPeeling();
		//InitialiseDualPeeling(width, height);

		//TerminateFrontPeeling();
		//InitialiseFrontPeeling(width, height);

		TerminateAccumulations();
		InitialiseAccumulations(width, height);
	}

private:
	Sprite mSprite;

	GLuint mQuadVAO;
	GLuint mQuadVBO;

	GLuint mDualBackBlenderFBO;
	GLuint mDualPeelingSingleFBO;
	GLuint mDualDepthTextureID[2];
	GLuint mDualFrontBlenderTextureID[2];
	GLuint mDualBackTempTextureID[2];
	GLuint mDualBackBlenderTextureID;

	GLuint mFrontFBO[2];
	GLuint mFrontDepthTextureID[2];
	GLuint mFrontColourTextureID[2];
	GLuint mFrontColourBlenderTextureID;
	GLuint mFrontColourBlenderFBO;

	GLuint mAccumulationTextureID[2];
	GLuint mAccumulationFBO;
	GLuint mAccumulationRBO;

	Shader mDualInitShader;
	Shader mDualPeelShader;
	Shader mDualBlendShader;
	Shader mDualFinalShader;

	Shader mFrontInitShader;
	Shader mFrontPeelShader;
	Shader mFrontBlendShader;
	Shader mFrontFinalShader;

	Shader mAverageInitShader;
	Shader mAverageFinalShader;

	Shader mWeightedSumInitShader;
	Shader mWeightedSumFinalShader;

	const GLenum mDrawBuffers[7];

	GLfloat mMaxDepth;
	GLuint mMaxPass;

	glm::vec3 mBackgroundColour;
};

#endif // DUALDEPTHPEELING_H