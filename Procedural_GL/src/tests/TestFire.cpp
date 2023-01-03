#include "TestFire.h"




#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

test::TestFire::TestFire()
    :m_ProjMatrix {
    glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f)
}, m_ViewMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) }
{
   
    static const float positions[] = {
    -1.0f,1.0f, 0.0f, 0.0f,1.0f,   //v0 
    1.0f,1.0f, 0.0f, 1.0f,1.0f,    //v1
    1.0f, -1.0f, 0.0f, 1.0f,0.0f, //v2 
    -1.0f, -1.0f,0.0f, 0.0f,0.0f, //v3  
    };

    unsigned int indices[] = {
        0,2,1,
        2,0,3
    };

    //Blending
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Enable depth test
    GLCall(glEnable(GL_DEPTH_TEST));
    // Accept fragment if it closer to the camera than the former one
    (glDepthFunc(GL_LESS));

   
    VAO = std::make_unique<VertexArray>();

    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 5 * sizeof(float)); //4 * 3,if no texture coords

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2); //texture coordinates

    VAO->AddBuffer(*m_VertexBuffer,layout);
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    

    /*va.AddBuffer(vb, layout);


    IndexBuffer(indices, 6);*/

   
   


    shader = std::make_unique<Shader>("res/shaders/vertexShaderFire.vs", "res/shaders/fragmentShaderFire.fs");
    //Shader shader("res/shaders/vertexShader.vs", "res/shaders/fragmentShader.fs");
    shader->Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


    //m_Texture = std::make_unique<Texture>("res/textures/Profile_picture.png");
    //shader->SetUniform1i("u_Texture", 0); // must match what we call in texture.Bind()

    ////clearing stuff
   
    //m_IndexBuffer->Unbind();
    //shader->Unbind();

}

test::TestFire::~TestFire()
{
    GLCall(glDisable(GL_DEPTH_TEST));
}

void test::TestFire::OnUpdate(float deltaTime)
{
    m_DeltaTime = deltaTime;
    m_ElapsedTime += deltaTime;
}

void test::TestFire::OnRender(GLFWwindow* inCurrentWindow)
{
   

	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	//GLCall(glClear(GL_COLOR_BUFFER_BIT));
    // Clear the screen
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   
    //User mouse input---------------------------------------------------------------------------------
    // 
    int mouseButtonState = glfwGetMouseButton(inCurrentWindow,GLFW_MOUSE_BUTTON_RIGHT);
    if (mouseButtonState == GLFW_PRESS)
    {
        glfwSetInputMode(inCurrentWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        // Get mouse position
        double xpos, ypos{ 0.0 };
        //glfwGetMousePos(&xpos, &ypos);
        glfwGetCursorPos(inCurrentWindow, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(inCurrentWindow, &width, &height);
        glfwSetCursorPos(inCurrentWindow, width / 2, height / 2);

        // Compute new orientation
        horizontalAngle += mouseSpeed * m_DeltaTime * float(width / 2 - xpos);
        verticalAngle += mouseSpeed * m_DeltaTime * float(height / 2 - ypos);
    }
    else
    {
        glfwSetInputMode(inCurrentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
   

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );
    // Up vector : perpendicular to both direction and right
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(inCurrentWindow,GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * m_DeltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(inCurrentWindow,GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * m_DeltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(inCurrentWindow,GLFW_KEY_D) == GLFW_PRESS) {
        position += right * m_DeltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(inCurrentWindow,GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * m_DeltaTime * speed;
    }

    m_ViewMatrix = glm::lookAt(position,position+direction,up);

    m_ModelMatrix = glm::mat4(1);
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * m_ModelMatrix; //reversed order for opengl layout
    //glm::mat4 mvp = model;
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);
    shader->SetUniform1f("elapsedTime", m_ElapsedTime);

    Renderer renderer;
    renderer.Draw(*VAO, *m_IndexBuffer, *shader);

    //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);


    
}

void test::TestFire::OnImGuiRender()
{
    //ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
