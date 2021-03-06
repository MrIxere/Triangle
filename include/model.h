#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>

#include "mesh.h"
#include "shader.h"
#include "engine.h"

namespace gpr5300
{
	class Model
	{
	public:
		std::vector<Texture> textures_loaded;
		glm::mat4 model = glm::mat4(1.0);
		void InitModel(const char* path, bool flip);
		void Draw(const CShader& shader) const;

		glm::mat4* modelAmount;
		void MultipleDraw(const CShader& shader, int amount) const;
		std::vector<CMesh> meshes_;
		CMesh mesh;
	private:
		std::string directory_;

		void loadModel(const std::string& path);
		void processNode(const aiNode* node, const aiScene* scene);
		CMesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}
