#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace test {

	class Test
	{

	public:
		
		Test(){}
		virtual ~Test(){}

		virtual void OnUpdate(float deltaTime){}
		virtual void OnRender(GLFWwindow* inCurrentWindow = nullptr){}
		virtual void OnImGuiRender(){}











	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		

		/*void OnUpdate(float deltaTime) override;
		void OnRender() override;*/
		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name) 
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:

		Test*& m_CurrentTest;
		//Pair containg name of test as string and a lambda function which will create a new instance of a test
		std::vector < std::pair<std::string, std::function<Test* ()>>> m_Tests;

	};





}