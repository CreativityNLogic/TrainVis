#include "Material.h"

Material::Material()
{

}

Material::Material(const Material &mat)
{
	setShader(mat.getShader());
	setDiffuseTexture(mat.getDiffuseTexture());
	setSpecularTexture(mat.getSpecularTexture());
	setNormalTexture(mat.getNormalTexture());
	setEmissiveTexture(mat.getEmissiveTexture());

	setDiffuseColour(mat.getDiffuseColour());
	setSpecularColour(mat.getSpecularColour());
	setAmbientColour(mat.getAmbientColour());
	setEmissiveColour(mat.getEmissiveColour());

	setShininess(mat.getShininess());

	setTransform(mat.getTransform());
	setView(mat.getView());
	setProjection(mat.getProjection());
	setViewPosition(mat.getViewPosition());
}

Material::Material(const std::string shaderVS, const std::string shaderFS) :
	mShader(shaderVS.c_str(), shaderFS.c_str())
{
}

void Material::loadShader(const std::string shaderVS, const std::string shaderFS)
{
	mShader = Shader(shaderVS.c_str(), shaderFS.c_str());
}

void Material::Bind()
{
	mShader.use();

	mDiffuseTexture.Bind(0);
	mShader.setInt("DiffuseTexture", 0);

	mSpecularTexture.Bind(1);
	mShader.setInt("SpecularTexture", 1);

	mNormalTexture.Bind(2);
	mShader.setInt("NormalTexture", 2);

	mEmissiveTexture.Bind(3);
	mShader.setInt("EmissiveTexture", 3);

	mShader.setVec4("DiffuseColour", mDiffuseColour);
	mShader.setVec4("SpecularColour", mSpecularColour);
	mShader.setVec4("AmbientColour", mAmbientColour);
	mShader.setVec4("EmissiveColour", mEmissiveColour);

	mShader.setFloat("Shininess", mShininess);

	glm::mat4 mvp = mProjection * mView * mModel;
	mShader.setMat4("Model", mModel);
	mShader.setMat4("TransInvModel", glm::transpose(glm::inverse(mModel)));
	mShader.setMat4("View", mView);
	mShader.setMat4("Projection", mProjection);
	mShader.setMat4("MVP", mvp);

	mShader.setVec3("ViewPosition", mViewPosition);
}

void Material::loadDiffuseTexture(const std::string &filename)
{
	mDiffuseTexture.LoadFromFile(filename, false);
}

void Material::loadSpecularTexture(const std::string &filename)
{
	mSpecularTexture.LoadFromFile(filename, false);
}

void Material::loadNormalTexture(const std::string &filename)
{
	mNormalTexture.LoadFromFile(filename, false);
}

void Material::loadEmissiveTexture(const std::string &filename)
{
	mEmissiveTexture.LoadFromFile(filename, false);
}

void Material::setDiffuseTexture(Texture diffuse)
{
	mDiffuseTexture = diffuse;
}

void Material::setSpecularTexture(Texture specular)
{
	mSpecularTexture = specular;
}

void Material::setNormalTexture(Texture normal)
{
	mNormalTexture = normal;
}

void Material::setEmissiveTexture(Texture normal)
{
	mEmissiveTexture = normal;
}

void Material::setDiffuseColour(glm::vec4 col)
{
	mDiffuseColour = col;
}

void Material::setSpecularColour(glm::vec4 col)
{
	mSpecularColour = col;
}

void Material::setAmbientColour(glm::vec4 col)
{
	mAmbientColour = col;
}

void Material::setEmissiveColour(glm::vec4 col)
{
	mEmissiveColour = col;
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

Texture Material::getDiffuseTexture() const
{
	return mDiffuseTexture;
}

Texture Material::getSpecularTexture() const
{
	return mSpecularTexture;
}

Texture Material::getNormalTexture() const
{
	return mNormalTexture;
}

Texture Material::getEmissiveTexture() const
{
	return mEmissiveTexture;
}

glm::vec4 Material::getDiffuseColour() const
{
	return mDiffuseColour;
}

glm::vec4 Material::getSpecularColour() const
{
	return mSpecularColour;
}

glm::vec4 Material::getAmbientColour() const
{
	return mAmbientColour;
}

glm::vec4 Material::getEmissiveColour() const
{
	return mEmissiveColour;
}

float Material::getShininess() const
{
	return mShininess;
}

glm::mat4 Material::getTransform() const
{
	return mModel;
}

glm::mat4 Material::getView() const
{
	return mView;
}

glm::mat4 Material::getProjection() const
{
	return mProjection;
}

glm::vec3 Material::getViewPosition() const
{
	return mViewPosition;
}

void Material::setShader(Shader shader)
{
	mShader = shader;
}

Shader Material::getShader() const
{
	return mShader;
}