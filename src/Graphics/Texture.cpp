#include "Texture.h"

#include "Glad/glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>

Texture::Texture() : mData(nullptr)
{

}

Texture::~Texture() 
{

}

Texture::Texture(const char *path, const string &directory, bool gamma) : mData(nullptr)
{
	string filename = string(path);
	mFilename = directory + '/' + filename;

	int width, height, nrComponents;
	mData = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (mData != nullptr)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, mData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(mData);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
}

int Texture::GetTextureID() const {

	return mTextureID;
}

void Texture::Bind() 
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glActiveTexture(GL_TEXTURE0);
}