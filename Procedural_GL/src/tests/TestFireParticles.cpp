#include "TestFireParticles.h"




#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

test::TestFireParticles::TestFireParticles()
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
    m_shaderDisplayTexture = std::make_unique<Shader>("res/shaders/vertexShader.vs", "res/shaders/fragmentShader.fs");
    m_shaderDisplayTexture->Bind();
    
    m_shaderParticlePos = std::make_unique<Shader>("res/shaders/vertexShaderParticlePosUpdate.vs", "res/shaders/fragmentShaderParticlePosUpdate.fs");
    m_shaderParticlePos->Bind();

    //shader = std::make_unique<Shader>("res/shaders/vertexShaderFireParticles.vs", "res/shaders/fragmentShaderFireParticles.fs");
    shader = std::make_unique<Shader>("res/shaders/vertexShaderParticle.vs", "res/shaders/fragmentShaderParticle.fs");
    //Shader shader("res/shaders/vertexShader.vs", "res/shaders/fragmentShader.fs");
    shader->Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


    //m_Texture = std::make_unique<Texture>("res/textures/Profile_picture.png");
    //shader->SetUniform1i("u_Texture", 0); // must match what we call in texture.Bind()

    ////clearing stuff
   
    //m_IndexBuffer->Unbind();
    //shader->Unbind();

    
   /* glGenTextures(1, &m_TexturePosition0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TexturePosition0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    GLCall(glGenFramebuffers(1, &m_FBOPosition0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOPosition0));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexturePosition0, 0));*/

    //--------- SECOND TEXTURE AND FBO

    /*glGenTextures(1, &m_TexturePosition1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TexturePosition1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    GLCall(glGenFramebuffers(1, &m_FBOPosition1));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOPosition1));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexturePosition1, 0));*/


}

test::TestFireParticles::~TestFireParticles()
{
    GLCall(glDisable(GL_DEPTH_TEST));
}

void test::TestFireParticles::OnUpdate(float deltaTime)
{
    m_DeltaTime = deltaTime;
    m_ElapsedTime += deltaTime;
}

void test::TestFireParticles::OnRender(GLFWwindow* inCurrentWindow)
{
   

	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	//GLCall(glClear(GL_COLOR_BUFFER_BIT));
    // Clear the screen
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   
    //User mouse input---------------------------------------------------------------------------------
    // 
    int mouseButtonState = glfwGetMouseButton(inCurrentWindow,GLFW_MOUSE_BUTTON_RIGHT);

    int width, height;
    if (mouseButtonState == GLFW_PRESS)
    {
        glfwSetInputMode(inCurrentWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        // Get mouse position
        double xpos, ypos{ 0.0 };
        //glfwGetMousePos(&xpos, &ypos);
        glfwGetCursorPos(inCurrentWindow, &xpos, &ypos);
        
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
    // Switch fbo and texture
    if (glfwGetKey(inCurrentWindow, GLFW_KEY_T) == GLFW_PRESS) {

        fbo_Switch = fbo_Switch == 0 ? 1 : 0;
        texture_Switch = texture_Switch == 0 ? 1 : 0;
    }

    m_ViewMatrix = glm::lookAt(position,position+direction,up);

    m_ModelMatrix = glm::mat4(1);
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * m_ModelMatrix; //reversed order for opengl layout
    //glm::mat4 mvp = model;

    //RENDERING------------------------------------------------------------------------------------------------------------
    Renderer renderer;
    //fbo stuffs
    
    

   

    //save old viewport size and change to match texture size corresponding nmbr of particles
    
   /* if (texture_Switch == 0)
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TexturePosition0));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOPosition0));
    }
    else
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TexturePosition1));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOPosition1));
    }*/

    


    //bind texture to fbo
    /*glActiveTexture(GL_TEXTURE0);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TexturePosition0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOPosition0));*/
    
   

    GLCall(glGenFramebuffers(1, &m_FBOPosition0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOPosition0));

    // generate texture
    glGenTextures(1, &m_TexturePosition0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TexturePosition0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 100, 100, 0, GL_RGBA, GL_FLOAT, NULL);


    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexturePosition0, 0));
   
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
       // std::cout << "FBO success!" << std::endl;
    }
    else
    {
        std::cout << "FBO BAD" << std::endl;
    }
    //m_shaderParticlePos->Bind();


    glfwGetWindowSize(inCurrentWindow, &width, &height);
    GLCall(glViewport(0, 0, 100, 100));







    renderer.Draw(*VAO, *m_IndexBuffer, *m_shaderParticlePos);

    

    //glDeleteFramebuffers(1, &m_FBOPosition1); DO WE NEED TO DO THIS?
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    //GLCall(glClear(GL_COLOR_BUFFER_BIT));
    // Clear the screen
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    //Change viewport back again
    GLCall(glViewport(0, 0, width, height));

    
    m_shaderDisplayTexture->Bind();
    m_shaderDisplayTexture->SetUniformMat4f("u_MVP", mvp);
    m_shaderDisplayTexture->SetUniform1i("u_Texture", 0);
   

    //display the position texture
    renderer.Draw(*VAO, *m_IndexBuffer, *m_shaderDisplayTexture);

   /* GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TexturePosition0));*/
    shader->Bind();

    shader->SetUniformMat4f("u_MVP", mvp);
    shader->SetUniform1i("u_TexturePos", 0);


    renderer.DrawInstances(*VAO, *m_IndexBuffer, *shader, 4, 4);
    
    /*shader->SetUniform1f("elapsedTime", m_ElapsedTime);

    shader->SetUniform1f("gradRotSpeed", m_GradRotSpeed);
    shader->SetUniform1f("verticalSpeed", m_VerticalSpeed);*/

    

    //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);


    
}

void test::TestFireParticles::OnImGuiRender()
{
    //ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
   
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
