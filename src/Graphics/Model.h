#pragma once

#include "Mesh.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
	public:
		vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		bool gammaCorrection;

		/*  Functions   */
		Model(char *path);

		void Draw(Shader &shader);

	private:
		/*  Model Data  */
		vector<Mesh> meshes;
		string directory;

		/*  Functions   */
		void loadModel(string path);

		void processNode(aiNode *node, const aiScene *scene);

		Mesh processMesh(aiMesh *mesh, const aiScene *scene);

		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};