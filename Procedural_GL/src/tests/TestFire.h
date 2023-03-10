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


	class TestFire : public Test 
	{

	public:

		TestFire();
		~TestFire();

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
		
		float m_DeltaTime{0.0f};
		float m_ElapsedTime{ 0.0f };
		float m_GradRotSpeed{ 1.0f };
		float m_VerticalSpeed{ 1.0f };
		glm::mat4 m_ProjMatrix, m_ViewMatrix, m_ModelMatrix;

		std::unique_ptr<VertexArray> VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr < IndexBuffer> m_IndexBuffer;
		std::unique_ptr < Shader> shader;
		std::unique_ptr<Texture> m_Texture;


		//User controls
		// position
		glm::vec3 position = glm::vec3(0, 0, 2.5);
		// horizontal angle : toward -Z
		float horizontalAngle = 3.14f;
		// vertical angle : 0, look at the horizon
		float verticalAngle = 0.0f;
		// Initial Field of View
		float initialFoV = 45.0f;

		float speed = 3.0f; // 3 units / second
		float mouseSpeed = 0.005f;
		
	

	};






}