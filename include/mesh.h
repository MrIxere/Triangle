#pragma once
#include "shader.h"
#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>

namespace gpr5300
{
	class CMesh
	{
	public:
		glm::mat4 view_ = glm::mat4(1.0f);
		GLuint vbo_[3] = {};
		void Generate();
		void Delete() const;
		void SetViewMatrix();
		void Draw(float t, const CShader& pipeline);
	private:
		float vertices_[72] =
		{
			//Front
			-0.5f, -0.5f, -0.5f, //0
			-0.5f, 0.5f, -0.5f,
			 0.5f, 0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,

			 //Back
			 -0.5f, -0.5f, 0.5f, //4
			 -0.5f, 0.5f, 0.5f,
			  0.5f, 0.5f, 0.5f,
			  0.5f, -0.5f, 0.5f,

			  //Right
			  0.5f, -0.5f, -0.5f, //8
			  0.5f, 0.5f, -0.5f,
			  0.5f, 0.5f, 0.5f,
			  0.5f, -0.5f, 0.5f,

			  //Left
			  -0.5f, -0.5f, -0.5f, //12
			  -0.5f, 0.5f, -0.5f,
			  -0.5f, 0.5f, 0.5f,
			  -0.5f, -0.5f, 0.5f,

			  //Top
			  -0.5f, 0.5f, -0.5f, //16
			   0.5f, 0.5f, -0.5f,
			   0.5f, 0.5f, 0.5f,
			  -0.5f, 0.5f, 0.5f,

			  //Bottom
			  -0.5f, -0.5f, -0.5f, //20
			   0.5f, -0.5f, -0.5f,
			   0.5f, -0.5f, 0.5f,
			  -0.5f, -0.5f, 0.5f

		};

		unsigned int indices_[36] =
		{
			//Front
			1, 2, 0,
			0, 2, 3,

			//Back
			7, 5, 4,
			7, 6, 5,

			//Right
			8, 9, 10,
			8, 10, 11,

			//Left
			14, 13, 12,
			15, 14, 12,

			//Top
			18, 17, 16,
			19, 18, 16,

			//Bottom
			20, 21, 22,
			20, 22, 23
		};

		float texCoords[48] =
		{
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
		};

		float normals_[72] = {
			//Front
			 0.0f, 0.0f, -1.0f,
			 0.0f, 0.0f, -1.0f,
			 0.0f, 0.0f, -1.0f,
			 0.0f, 0.0f, -1.0f,

			 //Back
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,
			 0.0f, 0.0f, 1.0f,

			 //Right
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			//Left
			-1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,

		   //Top
		   0.0f, 1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,

		  //Bottom
		  0.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		};

		GLuint vao_ = 0;
		GLuint ebo_ = 0;


		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 projection_ = glm::mat4(1.0f);
	};
}