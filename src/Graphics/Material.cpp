#include "Material.h"

Material::Material(string shaderVS, string shaderFS) {

	Shader shader(shaderVS.c_str(), shaderFS.c_str());
}


void Material::Bind() {
	// Pass texture in + diffuse colour

	glBindTexture(GL_TEXTURE_2D, diffuseTexture.GetTextureID());
}


void Material::setDiffuseTexture(Texture tex) {

	diffuseTexture = tex;
}


void Material::setDiffuseColor(glm::vec4 col) {

	diffuseColor = col;
}