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
class Filtering {
   private:
    static cv::Mat EdgeDetPrep( cv::Mat prcingImg );

   public:
    static cv::Mat IdentityMul( cv::Mat *prcingImage, float *ctrlVal ); /* Convolutional Identity */
    static cv::Mat ContrastBrightness( cv::Mat *prcingImage, float *conVal, float *briVal );
    static cv::Mat GaussianBlur( cv::Mat *prcingImage, int *ctrlVal );
    static cv::Mat MedianBlur( cv::Mat *prcingImage, int *ctrlVal );
    static cv::Mat Thresholding(
        cv::Mat *prcingImage, int *minVal, int *maxVal,
        cv::ThresholdTypes thrType = cv::THRESH_BINARY );
    static cv::Mat SobelEdgeDet( cv::Mat prcingImg, int x, int y );
    static cv::Mat CannyEdgeDet( cv::Mat prcingImg, int thrMin, int thrMax );
};