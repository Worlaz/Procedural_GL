#include "TestTexture2D.h"




#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

test::TestTexture2D::TestTexture2D()
    : translationA{ 200.0f, 200.0f, 0.0f }, translationB{ 400.0f, 200.0f, 0.0f },
    m_Proj{ glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f) }, m_View{ glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) }
{
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

   
    VAO = std::make_unique<VertexArray>();

    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); //4.4 since we now have 2 float for texture coords

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2); //texture coordinates

    VAO->AddBuffer(*m_VertexBuffer,layout);
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    

    /*va.AddBuffer(vb, layout);


    IndexBuffer(indices, 6);*/

   
   


    shader = std::make_unique<Shader>("res/shaders/vertexShader.vs", "res/shaders/fragmentShader.fs");
    //Shader shader("res/shaders/vertexShader.vs", "res/shaders/fragmentShader.fs");
    shader->Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


    m_Texture = std::make_unique<Texture>("res/textures/Profile_picture.png");
    shader->SetUniform1i("u_Texture", 0); // must match what we call in texture.Bind()

    ////clearing stuff
   
    //m_IndexBuffer->Unbind();
    //shader->Unbind();

}

test::TestTexture2D::~TestTexture2D()
{
}

void test::TestTexture2D::OnUpdate(float deltaTime)
{
}

void test::TestTexture2D::OnRender(GLFWwindow* inCurrentWindow)
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    m_Texture->Bind();

    

    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
    glm::mat4 mvp =  m_Proj* m_View* model; //reversed order for opengl layout
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*VAO, *m_IndexBuffer, *shader);

    //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);


    //translate the other object and then render
    model = glm::translate(glm::mat4(1.0f), translationB);
    mvp = m_Proj * m_View * model; //reversed order for opengl layout
    shader->Bind();
    shader->SetUniformMat4f("u_MVP", mvp);
    renderer.Draw(*VAO, *m_IndexBuffer, *shader);
}

void test::TestTexture2D::OnImGuiRender()
{
    ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
