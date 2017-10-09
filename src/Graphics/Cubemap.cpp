#include "Cubemap.h"

#include "Glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>

Cubemap::Cubemap() : mData(nullptr)
{

}

Cubemap::Cubemap(std::vector<std::string> textures_faces, bool gamma) :
	mData(nullptr)
{
	LoadFromFile(textures_faces, gamma);
}

unsigned int Cubemap::LoadFromFile(std::vector<std::string> textures_faces, bool gamma)
{

	int width, height, nrChannels;
	for (unsigned int i = 0; i < textures_faces.size(); i++)
	{
		unsigned char *data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}

	}

	if (mData != nullptr)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, &mCubemapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemapID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, mData);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		stbi_image_free(mData);
	}
	return mCubemapID;
}

void loadSkyboxFaces() {
	// load textures
	std::vector<std::string> faces
	{
		("bin/assets/textures/skybox/right.jpg"),
		("bin/assets/textures/skybox/left.jpg"),
		("bin/assets/textures/skybox/top.jpg"),
		("bin/assets/textures/skybox/bottom.jpg"),
		("bin/assets/textures/skybox/back.jpg"),
		("bin/assets/textures/skybox/front.jpg")
	};
}
int Cubemap::GetCubemapID() const
{
	return mCubemapID;
}

void Cubemap::Bind(unsigned int cubemapSlot)
{
	glActiveTexture(GL_TEXTURE0 + cubemapSlot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemapID);
}