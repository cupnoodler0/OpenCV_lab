#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "texture.h"
#include "filter/filtering.h"
#include "filter/custom_filtering.h"
#include "texture_cv.h"

#include "GUI/render_UI.h"
#include "GUI/tests_menu.h"

/* Remove this if the VC Image class has implemeted */
#include "../include/stb_image/stb_image.h"

// using namespace cv;

int main( void ) {
    /* Initialize the library */
    if ( !glfwInit() ) return -1;
    GLFWwindow* window;
    /* GL version setting */
    const char* glsl_version = "#version 130";
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow( 1280, 720, "Hello World", NULL, NULL );
    if ( !window ) {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent( window );
    /* Frame rate setting */
    glfwSwapInterval( 1 );

    /* init glad  */
    gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );

    // std::cout << glGetString(GL_VERSION) << std::endl;
    {
        GLCall( glEnable( GL_BLEND ) );
        GLCall( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );

        Renderer renderer;

        /* Init ImGui, setup */
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
        io.ConfigFlags |=
            ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport /
                                               // Platform Windows
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so
        // platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable ) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL( window, true );
        ImGui_ImplOpenGL3_Init( glsl_version );

        /* Tests */
        GUI *gui = new GUI();

        /* IMAGE PROCESSING */
        /* Test texture for the CV image window */
        TextureCV inCVTex( "res/textures/tiger.png" );
        TextureCV outCVTex( "res/textures/tiger.png" );
        TextureCV hsvCVTex( "res/textures/tiger.png" );

        GLuint srcImTexture;
        GLuint prcImTexture;
        GLuint hsvImTexture;
        GLCall( glGenTextures( 1, &srcImTexture ) );
        GLCall( glGenTextures( 1, &prcImTexture ) );
        GLCall( glGenTextures( 1, &hsvImTexture ) );
        /* The sourse image -- original one container */
        struct Images {
            cv::Mat src = cv::imread( "res/textures/tiger.png" );
            cv::Mat processing = src.clone();
            cv::Mat processed = src.clone();
            cv::Mat hsv = src.clone();
        } images;

        /* MOVE THIS TO THE IP MENU */
        struct convKernel {
            float Row1[3]{ 0., 0., 0. };
            float Row2[3]{ 0., 0., 0. };
            float Row3[3]{ 0., 0., 0. };
        };
        struct IdKernel {
            float Row1[3]{ 0., 0., 0. };
            float Row2[3]{ 0., 1., 0. };
            float Row3[3]{ 0., 0., 0. };
        };
        struct blurKernel {
            float Row1[3]{ .25 / 4, .25 / 2, .25 / 4 };
            float Row2[3]{ .25 / 2, .25, .25 / 2 };
            float Row3[3]{ .25 / 4, .25 / 2, .25 / 4 };
        };
        struct bSobelKernel {
            float Row1[3]{ -1., -2., -1. };
            float Row2[3]{ 0., 0., 0. };
            float Row3[3]{ 1., 2., 1. };
        };
        struct tSobelKernel {
            float Row3[3]{ 1., 2., 1. };
            float Row2[3]{ 0., 0., 0. };
            float Row1[3]{ -1., -2., -1. };
        };
        struct rSobelKernel {
            float Row3[3]{ -1., 0., 1. };
            float Row2[3]{ -2., 0., 2. };
            float Row1[3]{ -1., 0., 1. };
        };
        struct lSobelKernel {
            float Row3[3]{ 1., 0., -1. };
            float Row2[3]{ 2., 0., -2. };
            float Row1[3]{ 1., 0., -1. };
        };
        struct embossKernel {
            float Row3[3]{ -2., -1., 0. };
            float Row2[3]{ -1., 1., 1. };
            float Row1[3]{ 0., 1., 2. };
        };
        struct outlineKernel {
            float Row3[3]{ -1., -1., -1. };
            float Row2[3]{ -1., 8., -1. };
            float Row1[3]{ -1., -1., -1. };
        };
        struct sharpenKernel {
            float Row3[3]{ 0., -1., 0. };
            float Row2[3]{ -1., 5., -1. };
            float Row1[3]{ 0., -1., 0. };
        };
        /* Loop until the user closes the window */
        while ( !glfwWindowShouldClose( window ) ) {
            GLCall( glClearColor( 0.f, 0.f, 0.f, 1.f ) );
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /* Test Dockspace */
            gui->RenderUI();
            /* Rendering */
            ImGui::Render();
            // int display_w, display_h;
            // glfwGetFramebufferSize( window, &display_w, &display_h );
            // glViewport( 0, 0, display_w, display_h );
            // glClearColor( clear_color.x * clear_color.w,
            //               clear_color.y * clear_color.w,
            //               clear_color.z * clear_color.w, clear_color.w );
            // glClear( GL_COLOR_BUFFER_BIT );
            ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
            /* Swap front and back buffers */

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we
            // save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call
            //  glfwMakeContextCurrent(window) directly)
            if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable ) {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent( backup_current_context );
            }

            glfwSwapBuffers( window );

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow( window );
    glfwTerminate();
    return 0;
}