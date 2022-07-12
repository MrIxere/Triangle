#pragma once
#include "shader.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>
#include <assimp/types.h>

namespace gpr5300
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 tangent;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class CMesh
	{
	public:
		void InitMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures);
		void Draw(const CShader& shader) const;
		void	BindTexture(const CShader& shader) const;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int vao_{}, vbo_{}, ebo_{};

	private:
		void SetupMesh();
	};


}