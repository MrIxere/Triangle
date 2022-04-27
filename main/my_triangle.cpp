#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"

namespace gpr5300
{
	class MyTriangle : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;

		float vertices[12] =
		{
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f
		};

		float color[12] =
		{
			1.0f, 1.0f, 1.0f,
			1.0f, 0.5f, 1.0f,
			1.0f, 0.5f, 1.0f,
			1.0f, 0.5f, 0.0f
		};

		unsigned int indices[6] = {
			0,1,3,
			1,2,3
		};

	private:
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;
		GLuint program_ = 0;
		GLuint vao_ = 0;
		GLuint vbo_[2] = {};
		GLuint ebo_ = 0;
		float t = 0.0f;
	};

	void MyTriangle::Begin()
	{
		//EBOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		//VBO
		glGenBuffers(1, &vbo_[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glGenBuffers(1, &vbo_[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, ebo_);


		//VAO
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);


		//Load shaders
		const auto vertexContent = LoadFile("data/shaders/my_triangle/triangle.vert");
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
		const auto fragmentContent = LoadFile("data/shaders/my_triangle/triangle.frag");
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
		program_ = glCreateProgram();
		glAttachShader(program_, vertexShader_);
		glAttachShader(program_, fragmentShader_);
		glLinkProgram(program_);
		//Check if shader program was linked correctly

		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cerr << "Error while linking shader program\n";
		}

	}

	void MyTriangle::End()
	{
		//Unload program/pipeline
		glDeleteProgram(program_);

		glDeleteShader(vertexShader_);
		glDeleteShader(fragmentShader_);

		glDeleteVertexArrays(1, &vao_);
	}

	void MyTriangle::Update(float dt)
	{
		t += dt;

		//Draw program
		glUseProgram(program_);
		const float colorValue = (std::cos(t + 4.0f));
		glUniform1f(glGetUniformLocation(program_, "colorCoeff"), colorValue);
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

	}


}

int main(int argc, char** argv)
{
	gpr5300::MyTriangle scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}