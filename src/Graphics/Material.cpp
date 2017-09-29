#include "Material.h"

Material::Material(string shaderVS, string shaderFS) :
	mShader(shaderVS.c_str(), shaderFS.c_str())
{
}

void Material::Bind() 
{
	mShader.use();

	mDiffuseTexture.Bind(0);
	mShader.setInt("DiffuseTexture", 0);
	mShader.setVec4("DiffuseColour", mDiffuseColour);
	
	mSpecularTexture.Bind(1);
	mShader.setInt("SpecularTexture", 1);
	mShader.setVec4("SpecularColour", mSpecularColour);

	mNormalTexture.Bind(2);
	mShader.setInt("NormalTexture", 2);

	mShader.setVec4("AmbientColour", mAmbientColour);

	glm::mat4 mvp = mProjection * mView * mModel;
	mShader.setMat4("MVP", mvp);
}

void Material::setDiffuseTexture(const std::string &filename)
{
	mDiffuseTexture.LoadFromFile(filename, false);
}

void Material::setDiffuseColour(glm::vec4 col)
{
	mDiffuseColour = col;
}

void Material::setSpecularTexture(const std::string &filename)
{
	mSpecularTexture.LoadFromFile(filename, false);
}

void Material::setSpecularColour(glm::vec4 col)
{
	mSpecularColour = col;
}

void Material::setNormalTexture(const std::string &filename)
{
	mNormalTexture.LoadFromFile(filename, false);
}

void Material::setAmbientColour(glm::vec4 col)
{
	mAmbientColour = col;
}

void Material::setTransform(glm::mat4 transform)
{
	mModel = transform;
}

void Material::setView(glm::mat4 view)
{
	mView = view;
}

void Material::setProjection(glm::mat4 proj)
{
	mProjection = proj;
}