#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "../Graphics/Shader.h"
#include "../Graphics/Model.h"

class Cubemap
{
public:
	Cubemap();
	Cubemap(std::vector<std::string> textures_faces, bool gamma);

	void LoadFromFile(std::vector<std::string> textures_faces, bool gamma);
	unsigned int GetCubemapID() const;

	void Draw();
	void Bind(unsigned int cubemapSlot);

	void SetView(glm::mat4 view);
	void SetProjection(glm::mat4 proj);

private:
	std::vector<std::string> mFaces;

	unsigned int mCubemapID;

	Shader mCubemapShader;

	unsigned int mVAO;
	unsigned int mVBO;

	glm::mat4 mView;
	glm::mat4 mProjection;
};

#endif 