#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Glad/glad.h"
#include "Texture.h"
#include "Shader.h"

#include "../Components/FogComponent.h"

class Sprite
{
public:
	Sprite() :
		mModel(glm::mat4(1.0f)),
		mView(glm::mat4(1.0f)),
		mProjection(glm::mat4(1.0f)),
		mPosition(glm::vec3(0.0f)),
		mRotation(glm::quat()),
		mScale(glm::vec3(1.0f)),
		mOpacity(1.0f),
		mIsBillboard(false)
	{
	}

	Sprite(const std::string &filename) :
		mModel(glm::mat4(1.0f)),
		mView(glm::mat4(1.0f)),
		mProjection(glm::mat4(1.0f)),
		mPosition(glm::vec3(0.0f)),
		mRotation(glm::quat()),
		mScale(glm::vec3(1.0f)),
		mOpacity(1.0f),
		mIsBillboard(false)
	{
		LoadFromFile(filename);
	}

	void LoadFromFile(const std::string &filename)
	{
		mTexture.LoadFromFile(filename, false);
		Initialise();
	}

	void LoadFromTexture(const Texture &texture)
	{
		mTexture = texture;
		Initialise();
	}

	void Initialise()
	{
		mShader.Initialise("../../assets/shaders/sprite.vs", "../../assets/shaders/sprite.fs");

		float quadVertices[] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	void Draw()
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mShader.Bind();

		mTexture.Bind(0);
		mShader.setInt("Sprite", 0);

		glm::vec3 cameraRightWorldSpace = glm::vec3(mView[0][0], mView[1][0], mView[2][0]);
		glm::vec3 cameraUpWorldSpace = glm::vec3(mView[0][1], mView[1][1], mView[2][1]);
		
		mShader.setInt("fogParameters.type", mFog.Type);
		mShader.setVec4("fogParameters.colour", mFog.Colour);
		mShader.setFloat("fogParameters.linearStart", mFog.LinearStart);
		mShader.setFloat("fogParameters.linearEnd", mFog.LinearEnd);
		mShader.setFloat("fogParameters.expDensity", mFog.ExpDensity);
		mShader.setFloat("fogParameters.skyMix", mFog.SkyMix);
		mShader.setBool("UseFog", mFog.Enabled);

		mShader.setVec3("SpriteCenter", mPosition);
		mShader.setVec3("CameraRightWorldSpace", cameraRightWorldSpace);
		mShader.setVec3("CameraUpWorldSpace", cameraUpWorldSpace);
		mShader.setVec2("BillboardSize", glm::vec2(mScale));
		mShader.setMat4("MVP", mProjection * mView * GetModel());
		mShader.setMat4("VP", mProjection * mView);
		mShader.setFloat("Opacity", mOpacity);
		mShader.setBool("IsBillboard", mIsBillboard);

		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}

	void SetFogParams(FogComponent &fog)
	{
		mFog = fog;
	}

	void SetPosition(glm::vec3 position)
	{
		mPosition = position;
	}

	void SetRotation(glm::quat rotation)
	{
		mRotation = rotation;
	}

	void SetScale(glm::vec3 scale)
	{
		mScale = scale;
	}

	void SetView(glm::mat4 view)
	{
		mView = view;
	}

	void SetProjection(glm::mat4 projection)
	{
		mProjection = projection;
	}

	glm::mat4 GetModel() const
	{
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), mPosition);
		glm::mat4 rot = glm::mat4_cast(mRotation);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);

		return (trans * rot * scale);
	}

	glm::mat4 GetView() const
	{
		return mView;
	}

	glm::mat4 GetProjection() const
	{
		return mProjection;
	}

	void SetOpacity(float opacity)
	{
		mOpacity = opacity;
	}

	float GetOpacity() const
	{
		return mOpacity;
	}

	void SetBillboard(bool billboard)
	{
		mIsBillboard = billboard;
	}

	bool GetBillboard() const
	{
		return mIsBillboard;
	}

private:
	FogComponent mFog;

	Texture mTexture;
	Shader mShader;

	glm::mat4 mModel;
	glm::mat4 mView;
	glm::mat4 mProjection;

	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	float mOpacity;

	bool mIsBillboard;

	GLuint mVAO;
	GLuint mVBO;
};

#endif // SPRITE_H