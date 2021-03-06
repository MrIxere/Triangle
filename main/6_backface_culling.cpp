#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/glew.h"
#include "glm/mat4x4.hpp"
#include "engine.h"
#include "file_utility.h"
#include "scene.h"
#include <windows.h>

	
#include "stb_image.h"

//TODO
//refactor all this shit

namespace gpr5300
{
	class CullTexture
	{
	public:
		void CreateTexture(const std::string& file_path, auto textureNumber)
		{
			data_ = stbi_load(file_path.c_str(),
				&texWidth_, &texHeight_, &nrChannels_, 0);
			if (data_ == nullptr)
			{
				//TODO
			}
			glActiveTexture(textureNumber);
			glGenTextures(1, &texture_);
			glBindTexture(GL_TEXTURE_2D, texture_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth_, texHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		void Delete() const
		{
			glDeleteTextures(1, &texture_);
		}
	private:
		unsigned char* data_{};
		unsigned int texture_ = 0;
		unsigned int border_ = 0;
		int texWidth_ = 0;
		int texHeight_ = 0;
		int nrChannels_ = 0;
	};

	class CullMesh
	{
	public:
		glm::mat4 view_ = glm::mat4(1.0f);
		void Generate()
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

		void Delete() const
		{
			glDeleteProgram(program);
			glDeleteVertexArrays(1, &vao_);
		}

		void SetViewMatrix()
		{
			view_ = translate(view_, glm::vec3(0.0f, 0.0f, -3.f));
		}

		void Draw(float t)
		{
			glUseProgram(program);
			/*const int objectColor = glGetUniformLocation(program, "objectColor");*/
			const int lightColor = glGetUniformLocation(program, "lightColor");
			const int lightPos = glGetUniformLocation(program, "lightPos");
			const int viewPos = glGetUniformLocation(program, "viewPos");
			const int diffuse = glGetUniformLocation(program, "material.diffuse");
			const int specular = glGetUniformLocation(program, "material.specular");
			const int shininess = glGetUniformLocation(program, "material.shininess");
			const int ambientL = glGetUniformLocation(program, "light.ambientL");
			const int diffuseL = glGetUniformLocation(program, "light.diffuseL");
			const int specularL = glGetUniformLocation(program, "light.specularL");
			glUniform3f(lightPos, 0.0f, 0.0f, 1.0f);
			/*glUniform3f(objectColor, abs(cos(t)), abs(sin(t)), abs(tan(t)));*/
			glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
			glUniform3f(viewPos, 0.0f, 0.0f, 2.0f);
			glUniform1i(diffuse, 0);
			glUniform1i(specular, 1);
			glUniform1f(shininess, 10);
			glUniform3f(ambientL, 0.2f, 0.2f, 0.2f);
			glUniform3f(diffuseL, 0.5f, 0.5f, 0.5f);
			glUniform3f(specularL, 1.5f, 1.5f, 1.5f);

			model_ = rotate(model_, glm::radians(0.1f), glm::vec3(1.0f, 0.1f, 1.0f));
			projection_ = glm::perspective(glm::radians(45.f), (float)1920 / (float)1080, 0.1f, 100.0f);
			view_ = translate(view_, glm::vec3(0.0f, 0.0f, 0.0f));
			// retrieve the matrix uniform locations
			const unsigned int modelLoc = glGetUniformLocation(program, "model");
			const unsigned int viewLoc = glGetUniformLocation(program, "view");
			// pass them to the shaders (3 different ways)
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view_[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &projection_[0][0]);

			glBindVertexArray(vao_);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		}
		GLuint program = 0;
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

		float normals_[72] =
		{
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
		GLuint vbo_[3] = {};
		GLuint ebo_ = 0;


		glm::mat4 model_ = glm::mat4(1.0f);
		
		glm::mat4 projection_ = glm::mat4(1.0f);
	};


	class CullShader
	{
	public:
		void Load(CullMesh& mesh)
		{
			//Load shaders
			const auto vertexContent = gpr5300::LoadFile("data/shaders/culling/culling.vert");
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
			const auto fragmentContent = gpr5300::LoadFile("data/shaders/culling/culling.frag");
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
			mesh.program = glCreateProgram();
			glAttachShader(mesh.program, vertexShader_);
			glAttachShader(mesh.program, fragmentShader_);
			glLinkProgram(mesh.program);
			//Check if shader program was linked correctly

			glGetProgramiv(mesh.program, GL_LINK_STATUS, &success);
			if (!success)
			{
				std::cerr << "Error while linking shader program\n";
			}

		}

		void Delete() const
		{
			glDeleteShader(vertexShader_);
			glDeleteShader(fragmentShader_);
		}
	private:
		GLuint vertexShader_ = 0;
		GLuint fragmentShader_ = 0;
	};

	class CullScene final : public gpr5300::Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;


		void processInput(float dt)
		{
			const float cameraSpeed = 1.00f * dt; // adjust accordingly
			if (GetKeyState('W') & 0x8000)
				cameraPos += cameraSpeed * cameraFront;
			if (GetKeyState('S') & 0x8000)
				cameraPos -= cameraSpeed * cameraFront;
			if (GetKeyState('A') & 0x8000)
				cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
			if (GetKeyState('D') & 0x8000)
				cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;

			SDL_GetRelativeMouseState(&mouseX_, &mouseY_);
			float xOffset = mouseX_ * sensitivity_/5;
			float yOffset = mouseY_ * sensitivity_/5;
			direction.x += xOffset;
			direction.y += yOffset;

			cameraFront.x = direction.x;
			cameraFront.y = -direction.y;
		}

	private:

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		CullShader pipeline_;
		CullTexture mainTexture_;
		CullTexture specTexture_;
		CullMesh mesh_;
		//	Light light_;
		float tt_ = 0.0f;

		int mouseX_, mouseY_;
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
		const float sensitivity_ = 0.01f;
	};

	void CullScene::Begin()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);	
		mainTexture_.CreateTexture("data/textures/Jeff.png", GL_TEXTURE0);
		specTexture_.CreateTexture("data/textures/Jeff.png", GL_TEXTURE1);
		mesh_.Generate();
		pipeline_.Load(mesh_);
	}

	void CullScene::End()
	{
		//Unload program/pipeline
		mesh_.Delete();
		pipeline_.Delete();
		mainTexture_.Delete();
		specTexture_.Delete();
	}

	void CullScene::Update(float dt)
	{
		//Draw program
		tt_ += dt;
		processInput(dt);
		mesh_.view_ = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		mesh_.Draw(tt_);
	}

	

}

int main(int argc, char** argv)
{
	gpr5300::CullScene scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}
