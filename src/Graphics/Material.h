#pragma once

#include "Glad/glad.h"
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "Shader.h"

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>

#include "../Components/LightComponent.h"

class Material
{
public:
	Material();
	Material(const Material &mat);
	Material(const std::string shaderVS, const std::string shaderFS);

	void loadShader(const std::string shaderVS, const std::string shaderFS);

	void Bind();
	void UnBind();

	void loadDiffuseTexture(const std::string &filename);
	void loadSpecularTexture(const std::string &filename);
	void loadNormalTexture(const std::string &filename);
	void loadEmissiveTexture(const std::string &filename);

	void setDiffuseTexture(Texture diffuse);
	void setSpecularTexture(Texture specular);
	void setNormalTexture(Texture normal);
	void setEmissiveTexture(Texture emissive);

	void setDiffuseColour(glm::vec4 colour);
	void setSpecularColour(glm::vec4 colour);
	void setAmbientColour(glm::vec4 colour);
	void setEmissiveColour(glm::vec4 colour);

	void setShininess(float shininess);

	void setTransform(glm::mat4 transform);
	void setView(glm::mat4 view);
	void setProjection(glm::mat4 proj);
	void setViewPosition(glm::vec3 pos);
	void setLight(unsigned int index, glm::vec3 position, glm::vec3 direction, LightComponent light);

	Texture getDiffuseTexture() const;
	Texture getSpecularTexture() const;
	Texture getNormalTexture() const;
	Texture getEmissiveTexture() const;

	glm::vec4 getDiffuseColour() const;
	glm::vec4 getSpecularColour() const;
	glm::vec4 getAmbientColour() const;
	glm::vec4 getEmissiveColour() const;

	float getShininess() const;

	glm::mat4 getTransform() const;
	glm::mat4 getView() const;
	glm::mat4 getProjection() const;
	glm::vec3 getViewPosition() const;

	void setShader(Shader shader);
	Shader getShader() const;

	void setOpacity(float opacity);
	bool getOpacity() const;

	void setIsTransparent(bool transparent);
	bool IsTransparent() const;

private:
	Shader mShader;

	Texture mDiffuseTexture;
	Texture mSpecularTexture;
	Texture mNormalTexture;
	Texture mEmissiveTexture;

	glm::vec4 mDiffuseColour;
	glm::vec4 mSpecularColour;
	glm::vec4 mAmbientColour;
	glm::vec4 mEmissiveColour;

	float mShininess;

	glm::mat4 mModel;
	glm::mat4 mView;
	glm::mat4 mProjection;

	glm::vec3 mViewPosition;

	bool mIsTransparent;
	float mOpacity;
};
