#include <sstream>
#include <SDL.h>
#include <Windows.h>

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

		sceneShader.Load("data/shaders/framebuffer/model.vert", "data/shaders/framebuffer/model.frag");
		simpleColorShader.Load("data/shaders/framebuffer/model_single_colour.vert", "data/shaders/framebuffer/model_single_colour.frag");
		skyShader.Load("data/shaders/framebuffer/cubemap.vert", "data/shaders/framebuffer/cubemap.frag");

		skybox.skyboxTexture = skybox.loadSkybox(skybox.skyboxFaces);
		skybox.BindSkybox();

		model_.InitModel("data/models/backpack.obj");
		model_.view = translate(model_.view, glm::vec3(0.0f, 0.0f, -2.0f));
		/*
		model_.InitModel("data/textures/amogus/among us.obj");
		
		model_.view = translate(model_.view, glm::vec3(0.0f, 0.0f, -20.0f));*/
	}
	
	void CleanScene::End()
	{

	}


	void CleanScene::Update(float dt)
	{
		//Draw program
		ProcessInput(dt);


		model_.model = glm::rotate(model_.model, dt * glm::radians(0.0f), glm::vec3(10.0f, 0.0f, 0.0f)); //locale coordonate
		model_.projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		//Set uniforms
		sceneShader.Use();

		sceneShader.SetInt("skybox", 0);

		sceneShader.SetVector3("viewPos", 0.0f, 0.0f, 2.0f);
		sceneShader.SetVector3("dirLight.direction", 0.0f, 0.0f, 1.0f);
		sceneShader.SetVector3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		sceneShader.SetVector3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		sceneShader.SetVector3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		/*sceneShader_.SetInt("material.ambient", 0);
		sceneShader_.SetInt("material.specular", 1);*/
		sceneShader.SetFloat("material.shininess", 64.0f);
		sceneShader.SetMatrix4("model", model_.model);//
		sceneShader.SetMatrix4("view", model_.view);
		sceneShader.SetMatrix4("projection", model_.projection);
		glm::mat4 model = scale(model_.model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		sceneShader.SetMatrix4("model", model);



		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		model_.Draw(sceneShader);

		glDepthFunc(GL_LEQUAL);
		skyShader.Use();
		skyShader.SetInt("skybox", 0);
		skyShader.SetMatrix4("view", model_.view);
		skyShader.SetMatrix4("projection", model_.projection);
		glBindVertexArray(skybox.skyvao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//simpleColorShader.Use();
		//simpleColorShader.SetMatrix4("view", model_.view);
		//simpleColorShader.SetMatrix4("projection", model_.projection);
		//model = scale(model, glm::vec3(1.1f, 1.1f, 1.1f));	// it's a bit too big for our scene, so scale it down
		//simpleColorShader.SetMatrix4("model", model);
		//model_.Draw(simpleColorShader);
		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glEnable(GL_DEPTH_TEST);
	}

	void CleanScene::ProcessInput(float dt)
	{
		const float cameraSpeed = 1.00f * dt; // adjust accordingly
		const Uint8* state = SDL_GetKeyboardState(nullptr);

		if (state[SDL_SCANCODE_W]) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (state[SDL_SCANCODE_A]) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (state[SDL_SCANCODE_S]) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (state[SDL_SCANCODE_D]) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
