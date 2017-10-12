#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <entityx/Entity.h>

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
	void Draw(std::vector<Material> &materials);

	void AddMesh(Mesh mesh);
	void ClearMeshes();

	void SetPosition(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);
	void SetView(glm::mat4 view);
	void SetProjection(glm::mat4 projection);
	void SetViewPosition(glm::vec3 position);
	void SetLights(std::vector<entityx::Entity> lights);
	void SetLights(std::vector<entityx::Entity> lights, std::vector<Material> &materials);

	glm::mat4 GetModel() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;

private:
	/*  Functions   */
	void loadModel(const std::string &filename);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	glm::vec3 mViewPosition;

	glm::mat4 mModel;
	glm::mat4 mView;
	glm::mat4 mProjection;

	/*  Model Data  */
	std::vector<Mesh> mMeshes;
	std::vector<Material> mMaterials;
};

#endif // MODEL_H