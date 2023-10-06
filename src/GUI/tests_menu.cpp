#include "tests_menu.h"

#include "../tests/test_clear_color.h"
#include "../tests/test_texture_2D.h"
#include "../tests/test_3D.h"

#include <imgui.h>

TestsMenuUI::TestsMenuUI() {
    // Setup
    m_CurrentTest = nullptr;
    m_TestMenu = new test::TestMenu( m_CurrentTest );
    m_CurrentTest = m_TestMenu;

    m_TestMenu->RegisterTest<test::TestClearColor>( "Clear Color" );
    m_TestMenu->RegisterTest<test::TestTexture2D>( "2D Texture" );
    m_TestMenu->RegisterTest<test::Test3D>( "Test 3D" );
}

TestsMenuUI::~TestsMenuUI() {
    delete m_CurrentTest;
    if ( m_CurrentTest != m_TestMenu ) {
        delete m_TestMenu;
    }
}

void TestsMenuUI::OnImGuiRender() {
    if ( m_CurrentTest ) {
        m_CurrentTest->OnUpdate( 0.0f );
        m_CurrentTest->OnRender();
        ImGui::Begin( "Test" );
        if ( m_CurrentTest != m_TestMenu && ImGui::Button( "<-" ) ) {
            delete m_CurrentTest;
            m_CurrentTest = m_TestMenu;
        }
        m_CurrentTest->OnImGuiRender();
        ImGui::End();
    }
}