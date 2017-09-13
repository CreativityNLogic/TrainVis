#include "Model.h"

#include <iostream>

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

void Model::Draw()
{
	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		if (i < mMaterials.size())
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::scale(modelMatrix, mScale);
			modelMatrix *= glm::mat4_cast(mRotation);
			modelMatrix = glm::translate(modelMatrix, mPosition);

			mMaterials[i].setTransform(modelMatrix);
			mMaterials[i].setView(mView);
			mMaterials[i].setProjection(mProjection);
			mMaterials[i].Bind();
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

void Model::loadModel(const std::string &filename)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	mDirectory = filename.substr(0, filename.find_last_of('/'));

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
		// cout << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")" << endl; //Check if .obj file is being loaded correctly

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

		Material mat("../../assets/shaders/default.vs", "../../assets/shaders/default.fs");
		mat.setDiffuseColour(glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.0));
		mat.setSpecularColour(glm::vec4(specular.r, specular.g, specular.b, 1.0));
		mat.setAmbientColour(glm::vec4(ambient.r, ambient.g, ambient.b, 1.0));

		mMaterials.push_back(mat);

		// GET DIFFUSE/SPECULAR/NORMAL TEXTURES
	}

	return Mesh(vertices, indices);
}