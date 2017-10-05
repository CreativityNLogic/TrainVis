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

	mEmissiveTexture.Bind(3);
	mShader.setInt("EmissiveTexture", 3);

	mShader.setVec4("AmbientColour", mAmbientColour);

	glm::mat4 mvp = mProjection * mView * mModel;
	mShader.setMat4("Model", mModel);
	mShader.setMat4("TransInvModel", glm::transpose(glm::inverse(mModel)));
	mShader.setMat4("View", mView);
	mShader.setMat4("Projection", mProjection);
	mShader.setMat4("MVP", mvp);
	mShader.setVec3("ViewPosition", mViewPosition);
	mShader.setFloat("Shininess", mShininess);
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

void Material::setEmissiveTexture(const std::string &filename)
{
	mEmissiveTexture.LoadFromFile(filename, false);
}

void Material::setShininess(float shininess)
{
	mShininess = shininess;
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

void Material::setViewPosition(glm::vec3 pos)
{
	mViewPosition = pos;
}

void Material::setLight(unsigned int index, glm::vec3 position, glm::vec3 direction, LightComponent light)
{
	if (index >= 3)
	{
		std::cout << "ERROR (Material): The light index cannot be greater than 3." << std::endl;
		return;
	}

	mShader.setInt("lights[" + std::to_string(index) + "].type", light.LightType);

	mShader.setVec3("lights[" + std::to_string(index) + "].position", position);
	mShader.setVec3("lights[" + std::to_string(index) + "].direction", direction);

	mShader.setVec3("lights[" + std::to_string(index) + "].diffuse", light.Diffuse);
	mShader.setVec3("lights[" + std::to_string(index) + "].specular", light.Specular);
	mShader.setVec3("lights[" + std::to_string(index) + "].ambient", light.Ambient);

	mShader.setFloat("lights[" + std::to_string(index) + "].ambientStrength", light.AmbientStrength);
	mShader.setFloat("lights[" + std::to_string(index) + "].specularStrength", light.SpecularStrength);

	mShader.setFloat("lights[" + std::to_string(index) + "].constant", light.Constant);
	mShader.setFloat("lights[" + std::to_string(index) + "].linear", light.Linear);
	mShader.setFloat("lights[" + std::to_string(index) + "].quadratic", light.Quadratic);

	mShader.setFloat("lights[" + std::to_string(index) + "].innerCutoff", light.InnerCutoff);
	mShader.setFloat("lights[" + std::to_string(index) + "].outerCutoff", light.OuterCutoff);
}