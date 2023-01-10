#pragma once

#include "Test.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {


	class TestFireParticles : public Test
	{

	public:

		TestFireParticles();
		~TestFireParticles();
		void Init(GLFWwindow* inCurrentWindow);
		void UpdatePosition(GLFWwindow* inCurrentWindow = nullptr);
		void UpdateVelocity(GLFWwindow* inCurrentWindow = nullptr);
		void DisplayTextures(GLFWwindow* inCurrentWindow = nullptr);
		void OnUpdate(float deltaTime) override;
		void OnRender(GLFWwindow* inCurrentWindow) override;
		void OnImGuiRender() override;

		
	private:
		bool mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
				return true;
			}
			else
			{
				return false;
			}

		}
		Renderer renderer;
		bool IsInitial{ true };

		unsigned int fbo_Switch{ 0 };
		unsigned int m_FBOPosition0;
		unsigned int m_FBOPosition1;
		unsigned int m_FBOVelocity0;
		unsigned int m_FBOVelocity1;

		unsigned int readingFrom { 0 };
		unsigned int m_TexturePosition0;
		unsigned int m_TexturePosition1;
		unsigned int m_TextureVelocity0;
		unsigned int m_TextureVelocity1;
		
		unsigned int m_SqrtNmbrOfParticles{ 1000 };
		unsigned int m_nmbrPfParticles{ m_SqrtNmbrOfParticles * m_SqrtNmbrOfParticles };

		float m_DeltaTime{0.0f};
		float m_ElapsedTime{ 0.0f };
		float m_GradRotSpeed{ 1.0f };
		float m_VerticalSpeed{ 1.0f };
		glm::mat4 m_ProjMatrix, m_ViewMatrix, m_ModelMatrix;

		std::unique_ptr<VertexArray> VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr < IndexBuffer> m_IndexBuffer;
		std::unique_ptr < Shader> shader;
		std::unique_ptr < Shader> m_shaderParticlePosInit;
		std::unique_ptr < Shader> m_shaderParticleVelInit;
		std::unique_ptr < Shader> m_shaderParticlePosUpdate;
		std::unique_ptr < Shader> m_shaderParticleVelUpdate;
		std::unique_ptr < Shader> m_shaderDisplayTexture; //shader for displaying the position texture
		std::unique_ptr<Texture> m_Texture;


		//User controls
		// position
		glm::vec3 position = glm::vec3(0, 0,5);
		// horizontal angle : toward -Z
		float horizontalAngle = 3.14f;
		// vertical angle : 0, look at the horizon
		float verticalAngle = 0.0f;
		// Initial Field of View
		float initialFoV = 45.0f;

		float speed = 3.0f; // 3 units / second
		float mouseSpeed = 0.04f;
		
		glm::vec3 m_Acceleration{0.0f,0.0f,0.0f};

	};






}