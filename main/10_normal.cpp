#include <sstream>
#include <SDL.h>
#include <Windows.h>
#include <stb_image.h>

#include "engine.h"
#include "my_scene.h"


#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>


namespace gpr5300
{
	void CleanScene::Begin()
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);


		frameBuffer_.InitFrameBuffer();
		skybox_.skyboxTexture = skybox_.loadSkybox(skybox_.skyboxFaces);
		skybox_.BindSkybox();
		skyShader_.Load("data/shaders/normal/cubemap.vert", "data/shaders/normal/cubemap.frag");

		sceneShader_.Load("data/shaders/normal/model.vert", "data/shaders/normal/model.frag");
		screenShader_.Load("data/shaders/normal/framebuffer.vert", "data/shaders/normal/framebuffer.frag");


		//model_.InitModel("data/models/golf/scene_mesh_decimated_textured.obj", false);
		//model_.InitModel("data/textures/nanosuit/nanosuit.obj", false);
		model_.InitModel("data/models/backpack.obj", true);
		//model_.InitModel("data/models/cat/12221_Cat_v1_l3.obj", false);
		//model_.InitModel("data/textures/planet/planet.obj", true);
		
		//This one doesn't work with culling dunno why
		//model_.InitModel("data/models/summoner rift/summoner_rift.obj", false);
		sceneShader_.Use();
		modelMatrices = new glm::mat4[amount];
		for (unsigned int i = 0; i < amount; i++)
		{
			constexpr float offset = 10.0f;
			glm::mat4 model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(offset * i, 0.0, 0.0));
			modelMatrices[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < model_.meshes_.size(); i++)
		{
			unsigned int VAO = model_.meshes_[i].vao_;
			glBindVertexArray(VAO);
			// vertex attributes
			std::size_t vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindVertexArray(0);
		}


	}
	
	void CleanScene::End()
	{

	}


	void CleanScene::Update(float dt)
	{
		//Draw program
		tt_ += dt;
		ProcessInput(dt);

		frameBuffer_.bindFrameBuffer();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = cam_.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(cam_.Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);

		//set uniforms
		sceneShader_.Use();
		sceneShader_.SetInt("skybox", 0);
		sceneShader_.SetVector3("viewPos", cam_.Position);
		sceneShader_.SetVector3("dirLight.direction", cam_.Front);
		sceneShader_.SetVector3("dirLight.ambient", 0.3f, 0.3f,0.3f);
		sceneShader_.SetVector3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		sceneShader_.SetVector3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		sceneShader_.SetInt("material.ambient", 0);
		sceneShader_.SetInt("material.specular", 1);
		sceneShader_.SetFloat("material.shininess", 64.0f);
		sceneShader_.SetMatrix4("view", view);
		sceneShader_.SetMatrix4("projection", projection);

		model = scale(model_.model, glm::vec3(0.25f, 0.25f, 0.25f));

		sceneShader_.SetMatrix4("model", model);

		//model_.Draw(sceneShader_);
		model_.MultipleDraw(sceneShader_, amount);

		//draw skybox
		glDepthFunc(GL_LEQUAL);
		skyShader_.Use();
		skyShader_.SetInt("skybox", 0);
		view = glm::mat4(glm::mat3(cam_.GetViewMatrix()));
		skyShader_.SetMatrix4("view", view);
		skyShader_.SetMatrix4("projection", projection);

		glBindVertexArray(skybox_.skyvao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_.skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		//SecondPass FrameBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		frameBuffer_.bindDefaultFrameBuffer();
		screenShader_.Use();
		glBindVertexArray(frameBuffer_.vao);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D,frameBuffer_.textureColorBuffer );
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


	void CleanScene::OnEvent(const SDL_Event& event)
	{
		cam_.onEvent(event);
	}

	void CleanScene::ProcessInput(float dt)
	{

		SDL_Event event;
		SDL_PollEvent(&event);
		const Uint8* state = SDL_GetKeyboardState(nullptr);


		if (state[SDL_SCANCODE_W]) {
			cam_.ProcessKeyboard(FORWARD, dt);
		}
		if (state[SDL_SCANCODE_A]) {
			cam_.ProcessKeyboard(LEFT, dt);
		}
		if (state[SDL_SCANCODE_S]) {
			cam_.ProcessKeyboard(BACKWARD, dt);
		}
		if (state[SDL_SCANCODE_D]) {
			cam_.ProcessKeyboard(RIGHT, dt);
		}
	}

};

int main(int argc, char** argv)
{
	gpr5300::CleanScene scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	return EXIT_SUCCESS;
}
