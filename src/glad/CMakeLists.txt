cmake_minimum_required(VERSION 3.25.1)
project(glad)

# set(CMAKE_CXX_STANDARD 17)

# Vars
set(INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

# Project
add_library(glad
    ${INC_DIR}/glad/glad.h
    ${INC_DIR}/KHR/khrplatform.h
    ${SRC_DIR}/glad.c
)
# Additional include directories
target_include_directories(glad PUBLIC
    ${INC_DIR}/
)