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


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

   
    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
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



    glm::vec3 translationA(200.0f, 200.0f, 0.0f);
    glm::vec3 translationB(400.0f, 200.0f, 0.0f);

    float r = 0.0f;
    float increment = 0.05f;



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
        glm::mat4 mvp = proj * view * model; //reversed order for opengl layout
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        
        renderer.Draw(va, ib, shader);

        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        
        
        //translate the other object and then render
       model = glm::translate(glm::mat4(1.0f), translationB);
        mvp = proj * view * model; //reversed order for opengl layout
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);


       
        
        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        //ImGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        

        /* Poll for and process events */
        glfwPollEvents();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    //glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}