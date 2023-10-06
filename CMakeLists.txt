include(FetchContent)
cmake_minimum_required(VERSION 3.25.1)
project(main)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "-O3")

# Vars
set(INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# stb does not have a CMake buil, but is header-only
# find_path(STB_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include/stb_image")

# OpenGL Flags
# set(OpenGL_GL_PREFERENCE GLVND)

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
# option(GLFW_INSTALL OFF)
# find_package(glfw3 REQUIRED)
set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
# add_subdirectory( ${INC_DIR}/glfw )

# OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
include_directories( ${OpenCV_INCLUDE_DIRS} )

# external includes
# include_directories( SYSTEM ${INC_DIR}/glad/include )
# include_directories( SYSTEM ${INC_DIR}/glm )
# include_directories( SYSTEM ${INC_DIR}/imgui )
# include_directories( SYSTEM ${INC_DIR}/stb_image )
# include_directories( SYSTEM ${SRC_DIR} )
# include_directories( SYSTEM ${SRC_DIR}/tests )

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.3.7
)
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_Declare(imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         docking
)
FetchContent_MakeAvailable(glfw glm imgui)

# Project
add_executable(main
  ${SRC_DIR}/main.cpp

  ${SRC_DIR}/renderer.cpp
  ${SRC_DIR}/index_buffer.cpp
  ${SRC_DIR}/shader.cpp
  ${SRC_DIR}/texture.cpp
  ${SRC_DIR}/vertex_array.cpp
  ${SRC_DIR}/vertex_buffer.cpp
  ${SRC_DIR}/filter/filtering.cpp
  ${SRC_DIR}/filter/custom_filtering.cpp
  ${SRC_DIR}/texture_cv.cpp

  ${SRC_DIR}/GUI/render_UI.cpp
  ${SRC_DIR}/GUI/tests_menu.cpp
  ${SRC_DIR}/GUI/IP_menu.cpp

  ${INC_DIR}/stb_image/stb_image.cpp

  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp

  ${SRC_DIR}/tests/test.cpp
  ${SRC_DIR}/tests/test_clear_color.cpp
  ${SRC_DIR}/tests/test_texture_2D.cpp
  ${SRC_DIR}/tests/test_3D.cpp

  ${SRC_DIR}/renderer/framebuffer.cpp
  ${SRC_DIR}/renderer/OpenGL_framebuffer.cpp
)

# add_library(main
#   ${imgui_SOURCE_DIR}/imgui.cpp
#   ${imgui_SOURCE_DIR}/imgui_draw.cpp
#   ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
#   ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
#   ${imgui_SOURCE_DIR}/imgui_widgets.cpp
#   ${imgui_SOURCE_DIR}/imgui_tables.cpp
#   ${imgui_SOURCE_DIR}/imgui_demo.cpp
# )

target_include_directories(main PRIVATE
  ${glfw_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
  # ${SRC_DIR}/core
  # ${SRC_DIR}/GUI
  # ${SRC_DIR}/renderer
  # ${SRC_DIR}/tests
)

add_subdirectory(${SRC_DIR}/glad/)

target_link_libraries(main
  ${OpenCV_LIBS}
  glfw
  glad
  # stb_image
  # imgui
  glm
  ${CMAKE_DL_LIBS}
)

set_target_properties(main
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)