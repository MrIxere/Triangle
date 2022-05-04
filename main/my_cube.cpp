#include <fstream>
#include <iostream>
#include <sstream>
#include "gl/glew.h"

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace gpr5300
{
	struct Texture
	{
		float texCords[8] =
		{
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
			data = stbi_load("data/textures/Jeff.png", &texWidth_, &texHeight_, &nrChannels_, 0);
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

	struct Pipeline
	{
		float vertices[12] =
		{
			-1.0f, -1.0f,0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};

		unsigned int indices[6] =
		{
			0, 1, 3,
			1, 2, 3
		};
		GLuint program_ = 0;
		GLuint vao_ = 0;
		GLuint ebo_ = 0;
		GLuint vbo_[2] = {};

		void BindPipeline(Texture& texture)
		{

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

	struct Shader
	{
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;

		void BindShader(Pipeline& pipeline)
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
			//Load program/pipeline
			pipeline.program_ = glCreateProgram();
			glAttachShader(pipeline.program_, vertexShader_);
			glAttachShader(pipeline.program_, fragmentShader_);
			glLinkProgram(pipeline.program_);
			//Check if shader program was linked correctly

			glGetProgramiv(pipeline.program_, GL_LINK_STATUS, &success);
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
		Shader shader_;
		Pipeline pipeline_;
	};

	void Cube::Begin()
	{
		glEnable(GL_DEPTH_TEST);

		texture_.BindTexture();
		pipeline_.BindPipeline(texture_);
		shader_.BindShader(pipeline_);
		
		
		////Textures
		//if (!data)
		//{
		//	std::cerr << "Failed to load texture!";
		//}

		
	}

	void Cube::End()
	{
		//Unload program/pipeline
		glDeleteProgram(pipeline_.program_);

		glDeleteShader(shader_.vertexShader_);
		glDeleteShader(shader_.fragmentShader_);
		glDeleteTextures(1, &texture_.texture_);

		glDeleteVertexArrays(1, &pipeline_.vao_);

	}

	void Cube::Update(float dt)
	{
		//t += dt;

		//Draw program

		glUseProgram(pipeline_.program_);
		glUniform1i(glGetUniformLocation(pipeline_.program_, "ourTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_.texture_);
		glBindVertexArray(pipeline_.vao_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}


}

int main(int argc, char** argv)
{
	gpr5300::Cube scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}
