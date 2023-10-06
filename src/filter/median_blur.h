#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// Define function to apply median blur
void medianBlur(cv::Mat& image, cv::Mat& output, int kernelSize, int startRow, int endRow) {
    cv::Mat out;

    for (int row = startRow; row < endRow; row++) {
        for (int col = 0; col < image.cols; col++) {

            cv::Rect roi(col - kernelSize / 2, row - kernelSize / 2, kernelSize, kernelSize);
            // Ensure ROI is within image bounds
            roi &= cv::Rect(0, startRow, image.cols, endRow); 

            cv::Mat kernel = image(roi);

            
            // Flatten kernel into a row vector
            kernel = kernel.clone().reshape(0, 1); 
            // Sort each row in ascending order
            cv::sort(kernel, kernel, cv::SORT_ASCENDING + cv::SORT_EVERY_ROW); 
            std::printf("kernel size:(%d, %d)\n", kernel.cols, kernel.rows);
            std::printf("out:%d\n", kernel.size() / 2);
             // Take median value
            // out.at<uchar>(col, row) = kernel.at<uchar>(kernel.size() / 2);
            // output.at<uchar>(row, col) = kernel.at<uchar>(kernel.size() / 2);
        }
    }
}

// Define multi-threaded function to apply median blur
void medianBlurMT(cv::Mat& image, cv::Mat& output, int kernelSize, int numThreads) {
    std::vector<std::thread> threads(numThreads);
    int rowsPerThread = ceil(float(image.rows) / numThreads);
    for (int i = 0; i < numThreads; i++) {
        int startRow = i * rowsPerThread;
        int endRow = std::min((i + 1) * rowsPerThread, image.rows);
        threads[i] = std::thread(medianBlur, std::ref(image), std::ref(output), kernelSize, startRow, endRow);
    }
    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }
}
