#pragma once
#include "mesh.h"

#include <gl/glew.h>
#include <glm/ext/matrix_float4x4.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "shader.h"


namespace gpr5300
{

	void CMesh::Generate()
	{
		// VAO
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		//VBO
		glGenBuffers(1, &vbo_[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &vbo_[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals_), normals_, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &vbo_[2]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		//EBO
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

		SetViewMatrix();
	}

	void CMesh::Delete() const
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void CMesh::SetViewMatrix()
	{
		view_ = translate(view_, glm::vec3(0.0f, 0.0f, -2.f));
	}

	void CMesh::Draw(float t, const CShader& pipeline)
	{
		pipeline.Use();
		pipeline.SetVector3("viewPos", 0.0f, 0.0f, 2.0f);
		pipeline.SetVector3("lightColor", 1.0f, 1.0f, 1.0f);
		pipeline.SetInt("material.diffuse", 0);
		pipeline.SetInt("material.specular", 1);
		pipeline.SetFloat("material.shininess", 64);

		pipeline.ActivateLight();
		model_ = rotate(model_, glm::radians(-0.1f), glm::vec3(0.5f, 1.0f, 0.1f));
		projection_ = glm::perspective(glm::radians(45.f), 1920.0f / 1080.0f, 0.1f, 100.0f);


		// retrieve the matrix uniform locations
		pipeline.SetMatrix4("model", model_);
		pipeline.SetMatrix4("view", view_);
		pipeline.SetMatrix4("projection", projection_);

		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}
}