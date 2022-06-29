#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>

#include "mesh.h"
#include "shader.h"

namespace gpr5300
{
	class Model
	{
	public:
		std::vector<Texture> textures_loaded;
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection;
		void InitModel(const char* path);
		void Draw(const CShader& shader) const;

	private:
		std::vector<CMesh> meshes_;
		std::string directory_;

		void loadModel(const std::string& path);
		void processNode(const aiNode* node, const aiScene* scene);
		CMesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}
