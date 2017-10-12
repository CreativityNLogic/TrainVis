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

Texture::Texture()
{

}

Texture::Texture(const Texture &texture)
{
	SetTextureID(texture.GetTextureID());
	SetFilename(texture.GetFilename());
}

Texture::Texture(const std::string &filename, bool gamma)
{
	LoadFromFile(filename, gamma);
}

void Texture::LoadFromFile(const std::string &filename, bool gamma)
{
	mFilename = filename;

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
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
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load: " << filename << std::endl;
	}
}

void Texture::Bind(unsigned int textureSlot)
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}	

void Texture::SetTextureID(unsigned int id)
{
	mTextureID = id;
}

void Texture::SetFilename(const std::string &filename)
{
	mFilename = filename;
}

int Texture::GetTextureID() const
{
	return mTextureID;
}

std::string Texture::GetFilename() const
{
	return mFilename;
}