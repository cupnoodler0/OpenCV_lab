#pragma once

#include <vector>

#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

cv::Mat ApplyHomographyToImage( cv::Mat input_im,
                                std::vector<cv::Point2f> pts_src ) {
    cv::Mat result;

    result = cv::findHomography( pts_src, pts_dst );

    return result;
}