#pragma once

#include "Shader.h"
#include "Texture.h"

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
#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>
#include <iostream>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
	public:
		/*  Mesh Data  */
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		/*  Functions  */
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		~Mesh();

		void Draw(Shader &shader);

	private:
		/*  Render data  */
		unsigned int VAO, VBO, EBO;

		/* All loaded textures */
		vector<Texture> textures_loaded;

		/*  Functions    */
		void setupMesh();
};