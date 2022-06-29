#pragma once
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "shader.h"

namespace gpr5300
{
	class CleanScene final : public Scene
	{
	public:

		void Begin() override;
		void End() override;
		void Update(float dt) override;
		void ProcessInput(float dt);
	private:
		int modelNumber_ = 0;
		CShader sceneShader_;
		CShader simpleColorShader_;
		Model model_;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};

}