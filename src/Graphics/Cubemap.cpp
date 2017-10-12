#include "Cubemap.h"

#include "Glad/glad.h"
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>


Cubemap::Cubemap() : 
	mData(nullptr),
	mCubemapShader("../../assets/shaders/skyboxVert.vs", "../../assets/shaders/skyboxFrag.fs")
{
	float cubeMapVertes[] = 
	{       
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertes), &cubeMapVertes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

Cubemap::Cubemap(std::vector<std::string> textures_faces, bool gamma) :
	mData(nullptr)
{
	LoadFromFile(textures_faces, gamma);
}

void Cubemap::LoadFromFile(std::vector<std::string> faces, bool gamma)
{
	glGenTextures(1, &mCubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemapID);

	mFaces = faces;

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data != nullptr)
		{
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::SetView(glm::mat4 view)
{
	mView = view;
}

void Cubemap::SetProjection(glm::mat4 proj)
{
	mProjection = proj;
}

unsigned int Cubemap::GetCubemapID() const
{
	return mCubemapID;
}

void Cubemap::Bind(unsigned int cubemapSlot)
{
	glActiveTexture(GL_TEXTURE0 + cubemapSlot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemapID);
}

void Cubemap::Draw()
{
	glDepthFunc(GL_LEQUAL);
	mCubemapShader.use();
	mCubemapShader.setInt("CubeMap", 0);
	Bind(0);

	mCubemapShader.setMat4("View", glm::mat4(glm::mat3(mView)));
	mCubemapShader.setMat4("Projection", mProjection);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}