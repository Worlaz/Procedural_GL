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


	class TestTexture2D : public Test 
	{

	public:

		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender(GLFWwindow* inCurrentWindow = nullptr) override;
		void OnImGuiRender() override;


	private:
		
		glm::vec3 translationA, translationB; 

		glm::mat4 m_Proj, m_View;

		std::unique_ptr<VertexArray> VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr < IndexBuffer> m_IndexBuffer;
		std::unique_ptr < Shader> shader;
		std::unique_ptr<Texture> m_Texture;


		
	

	};






}