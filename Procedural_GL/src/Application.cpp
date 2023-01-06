#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
//#include "imgui/imgui_impl_opengl3_loader.h"

#include "tests/TestCLearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestFire.h"
#include "tests/TestFireParticles.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

   
    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Kill me", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }




    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) { std::cout << "Error!" << std::endl; }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
    
        -50.0f, -50.0f, 0.0f, 0.0f,   //0
         50.0f, -50.0f, 1.0f, 0.0f,  //1
         50.0f,  50.0f, 1.0f, 1.0f,    //2
        -50.0f,  50.0f, 0.0f, 1.0f    //3

    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

   //Blending
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    


    VertexArray va;
    VertexBuffer vb(positions,4 * 4 * sizeof(float)); //4.4 since we now have 2 float for texture coords

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2); //texture coordinates

    va.AddBuffer(vb,layout);


    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
    
    


    Shader shader("res/shaders/vertexShader.vs", "res/shaders/fragmentShader.fs");
    shader.Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    
   
    Texture texture("res/textures/Profile_picture.png");
    texture.Bind(0);
    shader.SetUniform1i("u_Texture",0); // must match what we call in texture.Bind()

    //clearing stuff
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");


    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::TestFire>("Fire GPU Texture");
    testMenu->RegisterTest<test::TestFireParticles>("Fire Particle System");

    //test::TestClearColor test;
   

    double lastTime = glfwGetTime();
    
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            /* test.OnUpdate(0.0f);
             test.OnRender();*/

             //glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
             //glm::mat4 mvp = proj * view * model; //reversed order for opengl layout
             //shader.Bind();
             //shader.SetUniformMat4f("u_MVP", mvp);
             //
             //renderer.Draw(va, ib, shader);

             //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);


             //translate the other object and then render
            //model = glm::translate(glm::mat4(1.0f), translationB);
            // mvp = proj * view * model; //reversed order for opengl layout
            // shader.SetUniformMat4f("u_MVP", mvp);
            // renderer.Draw(va, ib, shader);






             //ImGUI
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                //Calculate deltaTime and update lastTime

                double currentTime = glfwGetTime();
                float deltaTime = float(currentTime - lastTime);
                lastTime = currentTime;

                currentTest->OnUpdate(deltaTime);
                currentTest->OnRender(window);

                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                /*ImGui::Begin("Test");
                currentTest->OnImGuiRender();
                ImGui::End();*/
            }

            //test.OnImGuiRender();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);


            /* Poll for and process events */
            glfwPollEvents();
        }
    
        delete currentTest;
        if (currentTest != testMenu)
        {
            delete testMenu;
        }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}