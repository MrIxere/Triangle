#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "file_utility.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

namespace gpr5300
{
	void CShader::Load(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}


	void CShader::Use() const
	{
		glUseProgram(ID);
	}

	void CShader::checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

	void CShader::SetMatrix4(const std::string_view uniform, glm::mat4& matrix4) const
	{
		const int uniformLocation = glGetUniformLocation(ID, uniform.data());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value_ptr(matrix4));
	}

	void CShader::SetVector3(const std::string_view uniform, float x, float y, float z) const
	{
		const int uniformLocation = glGetUniformLocation(ID, uniform.data());
		glUniform3f(uniformLocation, x, y, z);
	}
	void CShader::SetVector3(const std::string& uniform, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, uniform.c_str()), 1, &value[0]);
	}

	void CShader::SetFloat(const std::string& uniform, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, uniform.c_str()), value);
	}

	void CShader::SetInt(const std::string& uniform, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, uniform.c_str()), value);
	}
}