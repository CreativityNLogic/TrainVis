#include "Model.h"

#include <iostream>

#include "../Components/TransformComponent.h"

/*  Functions   */
Model::Model() :
	mPosition(glm::vec3(0.0f)),
	mRotation(glm::quat()),
	mScale(glm::vec3(1.0f)),
	mView(glm::mat4(1.0f)),
	mProjection(glm::mat4(1.0f))
{

}

Model::Model(const std::string &filename) :
	mView(glm::mat4(1.0f)),
	mProjection(glm::mat4(1.0f))
{
	loadModel(filename);
}

Model::~Model() 
{

}

void Model::LoadFromFile(const std::string &filename)
{
	loadModel(filename);
}

void Model::AddMesh(Mesh mesh)
{
	mMeshes.push_back(mesh);
}

void Model::ClearMeshes()
{
	mMeshes.clear();
}

void Model::Draw()
{
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		if (i < mMaterials.size())
		{
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), mPosition);
			glm::mat4 rot = glm::mat4_cast(mRotation);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);

			glm::mat4 modelMatrix = trans * rot * scale;

			mMaterials[i].setTransform(modelMatrix);
			mMaterials[i].setView(mView);
			mMaterials[i].setProjection(mProjection);
			mMaterials[i].setViewPosition(mViewPosition);
			mMaterials[i].Bind();
		}

		mMeshes[i].Draw();
	}
		
}

void Model::Draw(std::vector<Material> &materials)
{
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		if (materials.size() != 0)
		{
			glm::mat4 trans = glm::translate(glm::mat4(1.0f), mPosition);
			glm::mat4 rot = glm::mat4_cast(mRotation);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), mScale);

			glm::mat4 modelMatrix = trans * rot * scale;

			materials[i % materials.size()].setTransform(modelMatrix);
			materials[i % materials.size()].setView(mView);
			materials[i % materials.size()].setProjection(mProjection);
			materials[i % materials.size()].setViewPosition(mViewPosition);
			materials[i % materials.size()].Bind();
		}

		mMeshes[i].Draw();
	}

}

void Model::SetPosition(glm::vec3 position)
{
	mPosition = position;
}

void Model::SetRotation(glm::quat rotation)
{
	mRotation = rotation;
}

void Model::SetScale(glm::vec3 scale)
{
	mScale = scale;
}

void Model::SetView(glm::mat4 view)
{
	mView = view;
}

void Model::SetProjection(glm::mat4 projection)
{
	mProjection = projection;
}

void Model::SetViewPosition(glm::vec3 position)
{
	mViewPosition = position;
}

void Model::SetLights(std::vector<entityx::Entity> lights)
{
	for (unsigned int i = 0; i < mMaterials.size(); i++)
	{
		for (unsigned int j = 0; j < lights.size(); j++)
		{
			auto &transform = lights[j].component<TransformComponent>();
			auto &light = lights[j].component<LightComponent>();

			mMaterials[i].setLight(j, transform->Position, glm::eulerAngles(transform->Rotation), *light.get());
		}
	}
}

void Model::loadModel(const std::string &filename)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, scene));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vector;
		// vertex positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// texture co-ordinates
		if (mesh->mTextureCoords[0])	// does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		if (mesh->HasTangentsAndBitangents())
		{
			glm::vec3 tangent;
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			vertex.Tangent = tangent;

			glm::vec3 bitangent;
			bitangent.x = mesh->mBitangents[i].x;
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;
			vertex.BiTangent = bitangent;
		}
		else
		{
			vertex.BiTangent = glm::vec3();
			vertex.Tangent = glm::vec3();
		}

		vertex.Colour = glm::vec3();

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
		aiColor3D diffuse;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

		aiColor3D specular;
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

		aiColor3D ambient;
		material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);

		float shininess = 0;
		material->Get(AI_MATKEY_SHININESS, shininess);

		aiString diffuseTex;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTex);

		aiString specTex;
		material->GetTexture(aiTextureType_SPECULAR, 0, &specTex);

		aiString normalTex;
		material->GetTexture(aiTextureType_NORMALS, 0, &normalTex);

		Material mat("../../assets/shaders/lighting.vs", "../../assets/shaders/lighting.fs");
		mat.setDiffuseColour(glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.0));
		mat.setSpecularColour(glm::vec4(specular.r, specular.g, specular.b, 1.0));
		mat.setAmbientColour(glm::vec4(ambient.r, ambient.g, ambient.b, 1.0));

		mat.setDiffuseTexture(diffuseTex.C_Str());
		mat.setSpecularTexture(specTex.C_Str());
		mat.setNormalTexture(normalTex.C_Str());

		mat.setShininess(shininess);

		mMaterials.push_back(mat);
	}

	return Mesh(vertices, indices);
}