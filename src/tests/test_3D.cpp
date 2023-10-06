#include "test_3D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "../renderer.h"

#include <imgui.h>

namespace test {
Test3D::Test3D()
    : m_Proj( glm::perspective( glm::radians( 60.0f ), 960.f / 540.f, .1f,
                                1000.f ) ),
      m_View( glm::translate( glm::mat4( 1.f ), glm::vec3( 0, 0, -3 ) ) ),
      m_Translation( 0, 0, 0 ),
      m_Rotation( .5f, 1.f, 0.f ) {
    float cube_positions[] = {
        // front
        -.5f, -.5f, .5f, 0.f, 0.f,  // 0
        .5f, -.5f, .5f, 1.f, 0.f,   // 1
        .5f, .5f, .5f, 1.f, 1.f,    // 2
        -.5f, .5f, .5f, 0.f, 1.f,   // 3
        // back
        -.5f, -.5f, -.5f, 0.f, 0.f,  // 4
        .5f, -.5f, -.5f, 1.f, 0.f,   // 5
        .5f, .5f, -.5f, 1.f, 1.f,    // 6
        -.5f, .5f, -.5f, 0.f, 1.f    // 7
    };
    unsigned cube_indices[] = {
        0, 1, 2,  // front
        2, 3, 0,  //
        1, 5, 6,  // right
        6, 2, 1,  //
        7, 6, 5,  // back
        5, 4, 7,  //
        4, 0, 3,  // left
        3, 7, 4,  //
        4, 5, 1,  // bottom
        1, 0, 4,  //
        3, 2, 6,  // top
        6, 7, 3   //
    };

    GLCall( glEnable( GL_DEPTH_TEST ) );
    GLCall( glEnable( GL_BLEND ) );
    GLCall( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );

    m_VAO = std::make_unique<VertexArray>();

    m_VertexBuffer =
        std::make_unique<VertexBuffer>( cube_positions, 8 * 5 * sizeof( float ) );
    VertexBufferLayout layout;
    layout.Push<float>( 3 );
    layout.Push<float>( 2 );
    m_VAO->AddBuffer( *m_VertexBuffer, layout );

    m_IndexBuffer = std::make_unique<IndexBuffer>( cube_indices, 3 * 12 );

    m_Shader = std::make_unique<Shader>( "res/shaders/Basic.shader" );
    m_Shader->Bind();
    m_Shader->SetUniform4f( "u_Color", .8f, .3f, .8f, 1.f );

    m_Texture = std::make_unique<Texture>( "res/textures/somepic.png" );
    m_Shader->SetUniform1i( "u_Texture", 0 );
}
Test3D::~Test3D() {}
void Test3D::OnUpdate( float deltaTime ) {}
void Test3D::OnRender() {
    GLCall( glClearColor( 0.f, .1f, .2f, 1.f ) );
    GLCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
    Renderer renderer;

    m_Texture->Bind();

    {
        glm::mat4 model = glm::translate( glm::mat4( 1.f ), m_Translation );
        // Was it learnopengl
        // glm::mat4 model = glm::rotate( glm::mat4( 1.f ), glm::radians( -55.f
        // ),
        //                                glm::vec3( 1.f, 0.f, 0.f ) );
        model = glm::rotate(
            model, (float)glfwGetTime() * glm::radians( 50.f ),
            glm::vec3( m_Rotation.x, m_Rotation.y, m_Rotation.z ) );
        glm::mat4 mvp = m_Proj * m_View * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f( "u_MVP", mvp );
        m_Shader->SetUniform1f( "u_Time", (float)glfwGetTime() );
        renderer.Draw( *m_VAO, *m_IndexBuffer, *m_Shader );
    }
}
void Test3D::OnImGuiRender() {
    ImGui::SliderFloat3( "3D test", &m_Translation.x, -10.f, 10.f );
    ImGui::SliderFloat3( "Rotation vector", &m_Rotation.x, -1, 1 );
}

}  // namespace test