#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

// 1. Global Thresholding
// 2. Binary Thresholding
// 3. Inverse-Binary Thresholding
// 4. Truncate Thresholding
// 5. Threshold to Zero
// 6. Inverted Threshold to Zero

// MAKE THIS CLASS OR ITS METHODS STATIC (CHECK HOW TO DO THIS LATER)
class CustomFiltering {
   public:
    static cv::Mat Sepia( cv::Mat *prcingImg );
    static cv::Mat Watercolor( cv::Mat *prcingImg );
    static cv::Mat Cartoon( cv::Mat *prcingImg );
};