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

class Material {

public:
	
	Material(string shaderVS, string shaderFS);

	void Bind();

	void setDiffuseTexture(Texture tex);

	void setDiffuseColor(glm::vec4);

private:

	Shader shader;
	Texture diffuseTexture;
	glm::vec4 diffuseColor;

};
