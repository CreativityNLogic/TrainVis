#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <entityx/Entity.h>

#include "AnimatedMesh.h"
#include "../Material.h"

#include "../../Components/FogComponent.h"

static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }

class AnimatedModel
{
public:
	/*  Functions   */
	AnimatedModel();
	AnimatedModel(const std::string &filename);
	~AnimatedModel();

	void LoadFromFile(const std::string &filename);

	void Draw();
	void Draw(std::vector<Material> &materials);

	void AddMesh(AnimatedMesh mesh);
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

	void SetFogParams(FogComponent &fog);

private:
	/*  Functions   */
	void loadModel(const std::string &filename);
	void processNode(aiNode *node, const aiScene *scene);
	AnimatedMesh processMesh(aiMesh *mesh, const aiScene *scene);

private:
	FogComponent mFog;

	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	glm::vec3 mViewPosition;

	glm::mat4 mModel;
	glm::mat4 mView;
	glm::mat4 mProjection;

	/*  Model Data  */
	std::vector<AnimatedMesh> mMeshes;
	std::vector<Material> mMaterials;

	std::map<std::string, unsigned int> mBoneMapping;
	std::vector<BoneInfo> mBoneInfoList;
};

#endif // ANIMATEDMODEL_H