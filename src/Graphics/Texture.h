#pragma once

#include <stb/stb_image.h>

#include "Glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>

using namespace std;

class Texture {

public:

	string filename;
	unsigned int textureID;
	unsigned int id;
	string type;
	aiString path;

	Texture();
	~Texture();
	Texture(const char *path, const string &directory, bool gamma);
	

	int getTextureID();

	int getID();

	void Bind();


private:
	


};
