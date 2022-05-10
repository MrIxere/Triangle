#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace gl {

	// Defines several possible options for camera movement. Used as
	// abstraction to stay away from window-system specific input methods
	enum class CameraMovementEnum {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 50.f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;


	// An abstract camera class that processes input and calculates the
	// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
	class Camera
	{
	public:
		// camera Attributes
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 vertical = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 world_up;
		// euler Angles
		float yaw;
		float pitch;
		// camera options
		float movement_speed_;
		float mouse_sensitivity_;
		float zoom_;

		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH);
		Camera(
			float posX, float posY, float posZ,
			float upX, float upY, float upZ,
			float yaw, float pitch);
		glm::mat4 GetViewMatrix();
		void ProcessKeyboard(CameraMovementEnum direction, float deltaTime);
		void ProcessMouseMovement(
			float xoffset,
			float yoffset,
			GLboolean constrainpitch = true);
		void ProcessMouseScroll(float yoffset);
		void SetState(glm::vec3 position, float pitch, float yaw);
		void UpdateCameraVectors();

	};

} // End namespace gl.
