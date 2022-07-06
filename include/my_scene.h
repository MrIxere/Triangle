#pragma once
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "shader.h"
#include "cubemap.h"
#include "camera.h"
#include "framebuffer.h"

namespace gpr5300
{
	class CleanScene final : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;
		void ProcessInput(float dt);
		void mouse_callback(double xPosInput, double yPosInput);
		void OnEvent(const SDL_Event& event) override;

	private:
		glm::mat4* modelMatrices;
		unsigned int amount = 1;
		float tt_ = 0;
		int numberOfModels_ = 0;
		CShader skyShader_;
		CShader sceneShader_;
		CShader simpleColorShader_;
		CShader screenShader_;
		std::vector<Model> models_;
		Model model_;
		Skybox skybox_;
		FrameBuffer frameBuffer_;
		Camera cam_{ glm::vec3(0.0f, 0.0f, 3.0f) };
	};

}