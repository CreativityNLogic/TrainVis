#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Material.h"

class Model 
{
public:
	/*  Functions   */
	Model();
	Model(const std::string &filename);
	~Model();

	void LoadFromFile(const std::string &filename);

	void Draw();
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	void SetView(glm::mat4 view);
	void SetProjection(glm::mat4 projection);

private:
	/*  Functions   */
	void loadModel(const std::string &filename);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	glm::mat4 mView;
	glm::mat4 mProjection;

	/*  Model Data  */
	std::vector<Mesh> mMeshes;
	std::vector<Material> mMaterials;
	std::string mDirectory;
};

#endif // MODEL_H