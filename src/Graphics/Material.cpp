#include "Material.h"

Material::Material(string shaderVS, string shaderFS) :
	mShader(shaderVS.c_str(), shaderFS.c_str())
{
}

void Material::Bind() 
{
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDiffuseTexture.GetTextureID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mSpecularTexture.GetTextureID());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mNormalTexture.GetTextureID());

	glActiveTexture(GL_TEXTURE0);*/

	mShader.use();

	//mShader.setInt("DiffuseTexture", 0);
	mShader.setVec4("DiffuseColour", mDiffuseColour);

	//mShader.setInt("SpecularTexture", 1);
	mShader.setVec4("SpecularColour", mSpecularColour);

	//mShader.setInt("NormalTexture", 2);
	mShader.setVec4("AmbientColour", mAmbientColour);

	//-----------------------------------
	// Model-View-Projection matrix. (MAKE SURE IT'S IN THIS ORDER!)
	glm::mat4 mvp = mProjection * mView * mModel;
	mShader.setMat4("MVP", mvp);
}

void Material::setDiffuseTexture(Texture tex) 
{
	mDiffuseTexture = tex;
}

void Material::setDiffuseColour(glm::vec4 col)
{
	mDiffuseColour = col;
}

void Material::setSpecularTexture(Texture tex)
{
	mSpecularTexture = tex;
}

void Material::setSpecularColour(glm::vec4 col)
{
	mSpecularColour = col;
}

void Material::setNormalTexture(Texture tex)
{
	mNormalTexture = tex;
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