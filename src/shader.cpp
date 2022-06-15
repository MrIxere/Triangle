#include "shader.h"
#include <iostream>
#include "file_utility.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

namespace gpr5300 
{
	void CShader::Load()
	{
		//Load shaders
		const auto vertexContent = LoadFile("data/shaders/clean/clean.vert");
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
		const auto fragmentContent = LoadFile("data/shaders/clean/clean.frag");
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
		program = glCreateProgram();
		glAttachShader(program, vertexShader_);
		glAttachShader(program, fragmentShader_);
		glLinkProgram(program);
		//Check if shader program was linked correctly
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cerr << "Error while linking shader program\n";
		}

	}

	void CShader::Use() const
	{
		glUseProgram(program);
	}

	void CShader::Delete() const
	{
		glDeleteProgram(program);
		glDeleteShader(vertexShader_);
		glDeleteShader(fragmentShader_);
	}

	void CShader::SetMatrix4(const std::string_view uniform, glm::mat4& matrix4) const
	{
		const int uniformLocation = glGetUniformLocation(program, uniform.data());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value_ptr(matrix4));
	}

	void CShader::SetVector3(const std::string_view uniform, float x, float y, float z) const
	{
		int uniformLocation = glGetUniformLocation(program, uniform.data());
		glUniform3f(uniformLocation, x, y, z);
	}

	void CShader::SetVector3(const std::string& uniform, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(program, uniform.c_str()), 1, &value[0]);
	}

	void CShader::SetFloat(const std::string& uniform, float value) const
	{
		glUniform1f(glGetUniformLocation(program, uniform.c_str()), value);
	}

	void CShader::SetInt(const std::string& uniform, int value) const
	{
		glUniform1i(glGetUniformLocation(program, uniform.c_str()), value);
	}

	void CShader::ActivateLight() const
	{
		SetVector3("light.position", 0.0f, 0.0f, 2.0f);
		SetVector3("light.ambientL", 0.3f, 0.3f, 0.3f);
		SetVector3("light.diffuseL", 1.0f, 1.0f, 1.0f);
		SetVector3("light.specularL", 1.0f, 1.0f, 1.0f);
	}
}