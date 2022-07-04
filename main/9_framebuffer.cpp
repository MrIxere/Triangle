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
		skyShader_.Load("data/shaders/framebuffer/cubemap.vert", "data/shaders/framebuffer/cubemap.frag");

		sceneShader_.Load("data/shaders/framebuffer/model.vert", "data/shaders/framebuffer/model.frag");
		screenShader_.Load("data/shaders/framebuffer/framebuffer.vert", "data/shaders/framebuffer/framebuffer.frag");


		model_.InitModel("data/models/backpack.obj");
		/*
		model_.InitModel("data/textures/amogus/among us.obj");
		
		model_.view = translate(model_.view, glm::vec3(0.0f, 0.0f, -2.0f));*/
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

		model_.Draw(sceneShader_);

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
