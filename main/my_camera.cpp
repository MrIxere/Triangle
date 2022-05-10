#include <fstream>
#include <iostream>
#include <sstream>
#include "gl/glew.h"

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//new includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>





namespace gpr5300
{
	struct Texture
	{
		float texCords[48] =
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
			1.0f, 1.0f

		};
		unsigned char* data;
		unsigned int texture_ = 0;
		int texWidth_ = 0;
		int texHeight_ = 0;
		int nrChannels_ = 0;

		void BindTexture()
		{
			data = stbi_load("data/textures/amouJeff.png", &texWidth_, &texHeight_, &nrChannels_, 0);
			glGenTextures(1, &texture_);
			glBindTexture(GL_TEXTURE_2D, texture_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth_, texHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			//std::free(data);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glGenerateMipmap(GL_TEXTURE_2D);

			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		

	};

	struct Mesh
	{
		float vertices[108] =
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
			  -0.5f, -0.5f, 0.5f,
		};

		unsigned int indices[36] =
		{
			//Front
			0, 1, 2,
			0, 2, 3,

			//Back
			4, 5, 7,
			5, 6, 7,

			//Right
			8, 9, 10,
			8, 10, 11,

			//Left
			12, 13, 14,
			12, 14, 15,

			//Top
			16, 17, 18,
			16, 18, 19,

			//Bottom
			20, 21, 22,
			20, 22, 23
			
		};
		GLuint program_ = 0;
		GLuint vao_ = 0;
		GLuint ebo_ = 0;
		GLuint vbo_[2] = {};

		void BindMesh(Texture& texture)
		{
			//TODO créé classe différente
			// VAO 
			glGenVertexArrays(1, &vao_);
			glBindVertexArray(vao_);

			//VBO
			glGenBuffers(1, &vbo_[0]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glGenBuffers(1, &vbo_[1]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texture.texCords), texture.texCords, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);

			//EBO
			glGenBuffers(1, &ebo_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			
		}
	};

	struct Pipeline
	{
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;

		void BindPipeline(Mesh& mesh)
		{
			//Load shaders
			const auto vertexContent = LoadFile("data/shaders/my_cube/cube.vert");
			const auto* ptr = vertexContent.data();
			vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader_, 1, &ptr, nullptr);
			glCompileShader(vertexShader_);
			//Check success status of shader compilation
			GLint success;
			glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				std::cerr << "Error while loading vertex shader\n";
			}
			const auto fragmentContent = LoadFile("data/shaders/my_cube/cube.frag");
			ptr = fragmentContent.data();
			fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader_, 1, &ptr, nullptr);
			glCompileShader(fragmentShader_);
			//Check success status of shader compilation

			glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				std::cerr << "Error while loading fragment shader\n";
			}
			//Load program/mesh
			mesh.program_ = glCreateProgram();
			glAttachShader(mesh.program_, vertexShader_);
			glAttachShader(mesh.program_, fragmentShader_);
			glLinkProgram(mesh.program_);
			//Check if shader program was linked correctly

			glGetProgramiv(mesh.program_, GL_LINK_STATUS, &success);
			if (!success)
			{
				std::cerr << "Error while linking shader program\n";
			}

		}
	};
	
	class Cube : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;

		

	private:
		//float t = 0.0f;
		Texture texture_;
		Pipeline shader_;
		Mesh mesh_;
		float t = 0.0f;
	};

	void Cube::Begin()
	{
		glEnable(GL_DEPTH_TEST);

		texture_.BindTexture();
		mesh_.BindMesh(texture_);
		shader_.BindPipeline(mesh_);
		
		
		////Textures
		//if (!data)
		//{
		//	std::cerr << "Failed to load texture!";
		//}

		
	}

	void Cube::End()
	{
		//Unload program/pipeline
		glDeleteProgram(mesh_.program_);

		glDeleteShader(shader_.vertexShader_);
		glDeleteShader(shader_.fragmentShader_);
		glDeleteTextures(1, &texture_.texture_);

		glDeleteVertexArrays(1, &mesh_.vao_);

	}

	void Cube::Update(float dt)
	{
		
		t += dt;

		glUseProgram(mesh_.program_);

		glUniform1i(glGetUniformLocation(mesh_.program_, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_.texture_);



		glm::mat4 model = glm::mat4(1.0f);
		model = rotate(model, t * glm::radians(50.0f), glm::vec3(65.0f, 42.0f, 12.0f));

		glm::mat4 view = glm::mat4(1.0f);
		view = translate(view,  glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection/* = glm::mat4(1.0f)*/;
		projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

		

		//Draw program

		int modelLoc = glGetUniformLocation(mesh_.program_, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(mesh_.program_, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(mesh_.program_, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		
		glBindVertexArray(mesh_.vao_);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
	}


}

int main(int argc, char** argv)
{
	gpr5300::Cube scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}
