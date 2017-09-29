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

using namespace std;

class Material 
{
public:
	Material(string shaderVS, string shaderFS);
	void Bind();

	void setDiffuseTexture(const std::string &filename);
	void setSpecularTexture(const std::string &filename);
	void setNormalTexture(const std::string &filename);

	void setDiffuseColour(glm::vec4 colour);
	void setSpecularColour(glm::vec4 colour);
	void setAmbientColour(glm::vec4 colour);

	// Thinking of putting the shader and the projection/view/model outside of materials. Probably need
	// to create a render target class
	void setTransform(glm::mat4 transform);
	void setView(glm::mat4 view);
	void setProjection(glm::mat4 proj);

private:
	Shader mShader;

	Texture mDiffuseTexture;
	Texture mSpecularTexture;
	Texture mNormalTexture;
	
	glm::vec4 mDiffuseColour;
	glm::vec4 mSpecularColour;
	glm::vec4 mAmbientColour;

	glm::mat4 mModel;
	glm::mat4 mView;
	glm::mat4 mProjection;
};
