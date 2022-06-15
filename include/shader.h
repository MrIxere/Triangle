#pragma once
#include <gl/glew.h>
#include <xstring>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>

namespace gpr5300
{
	class CShader
	{
	public:
		GLuint program = 0;
		void Load();
		void Use() const;
		void SetMatrix4(std::string_view uniform, glm::mat4& matrix4) const;
		void SetVector3(std::string_view uniform, float x, float y, float z) const;
		void SetVector3(const std::string& uniform, const glm::vec3& value) const;
		void SetFloat(const std::string& uniform, float value) const;
		void SetInt(const std::string& uniform, int value) const;
		void ActivateLight() const;

		void Delete() const;
	private:
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;
	};
}