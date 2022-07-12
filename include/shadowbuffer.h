#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <string>
#include <vector>
#include <assimp/types.h>

#include "shader.h"


namespace gpr5300
{

	class ShadowBuffer
	{
	public:
		void InitSB();
		void bindShadowBuffer() const;
		static void bindDefaultBuffer();

		unsigned int texture_;


		unsigned int textureShadowbuffer_;
		unsigned int svao_{}, svbo_{}, sfbo_{}, srbo_{};
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	private:


	};
} 