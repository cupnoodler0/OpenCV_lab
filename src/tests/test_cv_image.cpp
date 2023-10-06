#include "test_cv_image.h"

#include "../renderer.h"
#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

test::TestCVImage::TestCVImage()
    : m_Proj( glm::ortho( 0.f, 960.f, 0.f, 540.f, -1.f, 1.f ) ),
      m_View( glm::translate( glm::mat4( 1.f ), glm::vec3( 0, 0, 0 ) ) ),
      m_TranslationA( 200, 200, 0 ),
      m_TranslationB( 400, 200, 0 ),
      m_SrcImWinIsShowing( false ),
      m_PrcImWinIsShowing( false ) {
    GLCall( glEnable( GL_BLEND ) );
    GLCall( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );

    m_Shader = std::make_unique<Shader>( "res/shaders/Basic.shader" );
    m_Shader->Bind();
    // m_Shader->SetUniform4f( "u_Color", .8f, .3f, .8f, 1.f );

    m_Texture = std::make_unique<Texture>( "res/textures/tiger.jpg" );
    m_Shader->SetUniform1i( "u_Texture", 0 );
}

test::TestCVImage::~TestCVImage() {}

void test::TestCVImage::OnUpdate( float deltaTime ) {}

void test::TestCVImage::OnRender() {
    Renderer renderer;
    m_Texture->Bind();
}

void test::TestCVImage::OnImGuiRender() {
    ImGui::Image(
        reinterpret_cast<void*>( static_cast<intptr_t>( textureIntLoc ) ),
        ImVec2( srcImage.cols * .5f, srcImage.rows * .5f ), ImVec2( 0.f, 0.f ),
        ImVec2( 1.f, 1.f ) );
}
