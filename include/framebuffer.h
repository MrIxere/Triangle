#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>
#include <assimp/types.h>
#include "shader.h"

namespace gpr5300
{
	class FrameBuffer
	{
	public:
		void InitFrameBuffer();
		void Draw(const CShader& shader) const;
		void bindFrameBuffer();
		void bindDefaultFrameBuffer();
		void createScreenVao();
		unsigned int texture;
		unsigned int textureColorBuffer = 0;
		unsigned int vao{}, vbo{}, fbo{}, rbo{};
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	private:
		float quadVertices[4 * 6] =
		{
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
	};
}
