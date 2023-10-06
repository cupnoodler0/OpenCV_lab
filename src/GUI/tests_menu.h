#pragma once

#include "../tests/test_clear_color.h"
#include "../tests/test_texture_2D.h"
#include "../tests/test_3D.h"

class TestsMenuUI {
   public:
    test::Test* m_CurrentTest;
    test::TestMenu* m_TestMenu;

    TestsMenuUI();
    ~TestsMenuUI();
    void OnImGuiRender();

   // private:
};