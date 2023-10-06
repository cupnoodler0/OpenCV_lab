#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <thread>

cv::Mat MedianBlur_s_old( cv::Mat &image, int kernelSize ) {
    int rows = image.rows;
    int cols = image.cols;

    cv::Mat output( rows, cols, CV_8UC3 );

    for ( int row = 0; row < rows; row++ ) {
        for ( int col = 0; col < cols; col++ ) {
            std::vector<int> kernelB;
            std::vector<int> kernelG;
            std::vector<int> kernelR;

            for ( int i = -kernelSize / 2; i <= kernelSize / 2; i++ ) {
                for ( int j = -kernelSize / 2; j <= kernelSize / 2; j++ ) {
                    int r = row + i;
                    int c = col + j;

                    // std::printf( "point:(%d, %d), kernel:(%d, %d)\n", col, row,
                    //              c, r );
                    if ( r >= 0 && r < rows && c >= 0 && c < cols ) {
                        kernelB.push_back( image.at<cv::Vec3b>( r, c )[0] );
                        kernelG.push_back( image.at<cv::Vec3b>( r, c )[1] );
                        kernelR.push_back( image.at<cv::Vec3b>( r, c )[2] );
                    }
                }
            }

            std::sort( kernelB.begin(), kernelB.end() );
            std::sort( kernelG.begin(), kernelG.end() );
            std::sort( kernelR.begin(), kernelR.end() );
            int medianIndex = kernelB.size() / 2;
            cv::Vec3b medianValue =
                cv::Vec3b( kernelB[medianIndex], kernelG[medianIndex],
                           kernelR[medianIndex] );

            output.at<cv::Vec3b>( row, col ) = medianValue;
        }
    }
    return output;
}