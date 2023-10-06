#include "median_blur.h"
#include "filtering.h"

cv::Mat Filtering::IdentityMul( cv::Mat *prcingImage, float *ctrlVal ) {
    cv::Mat prcedImage;

    /* Identity kernel */
    cv::Mat idKernel =
        ( cv::Mat_<double>( 3, 3 ) << 0, 0, 0, 0, *ctrlVal, 0, 0, 0, 0 );
    /* Apply this one instantly */
    cv::filter2D( *prcingImage, prcedImage, -1, idKernel, cv::Point( -1, -1 ),
                  4 );
    return prcedImage;
}

cv::Mat Filtering::ContrastBrightness( cv::Mat *prcingImage, float *conVal, float *briVal){
    cv::Mat prcedImg;

    // prcedImg = *prcingImage * ( *ctrlVal / 127 + 1 ) - *ctrlVal;
    (*prcingImage).convertTo( prcedImg, -1, *conVal, *briVal );

    return prcedImg;
}

cv::Mat Filtering::GaussianBlur( cv::Mat *prcingImage, int *ctrlVal ) {
    cv::Mat prcedImage;

    /* Built-in Gaussian Blur */
    if ( *ctrlVal % 2 == 0 ) *ctrlVal += 1;
    /* Apply this one instatnly */
    cv::GaussianBlur( *prcingImage, prcedImage, cv::Size( *ctrlVal, *ctrlVal ),
                      0., 0. );
    return prcedImage;
}

cv::Mat Filtering::MedianBlur( cv::Mat *prcingImage, int *ctrlVal ) {
    cv::Mat prcedImage;

    /* Built-in Gaussian Blur */
    if ( *ctrlVal % 2 == 0 ) *ctrlVal += 1;
    /* Apply this one instatnly */
    // cv::medianBlur(*prcingImage, prcedImage, *ctrlVal);
    // prcedImage = MedianBlur_s(*prcingImage, *ctrlVal);
    cv::Mat im_cvt;
    cv::cvtColor( *prcingImage, im_cvt, cv::COLOR_BGR2GRAY );
    medianBlurMT(im_cvt, prcedImage, *ctrlVal, 4);
    cv::cvtColor( prcedImage, im_cvt, cv::COLOR_GRAY2BGR );

    return im_cvt;
}

cv::Mat Filtering::Thresholding(
    cv::Mat *prcingImage, int *minVal, int *maxVal,
    cv::ThresholdTypes thrType) {
    cv::Mat prcedImage;
    cv::threshold( *prcingImage, prcedImage, *minVal, *maxVal, thrType );
    return prcedImage;
}

cv::Mat Filtering::EdgeDetPrep( cv::Mat prcingImg ) {
    cv::Mat img_gray;
    cv::cvtColor( prcingImg, img_gray, cv::COLOR_BGR2GRAY );
    cv::Mat img_blur;
    cv::GaussianBlur( img_gray, img_blur, cv::Size( 3, 3 ), 0. );
    return img_blur;
}

cv::Mat Filtering::SobelEdgeDet( cv::Mat prcingImg, int x, int y ) {
    prcingImg = EdgeDetPrep( prcingImg );
    cv::Mat prcedImg;
    cv::Sobel( prcingImg, prcedImg, CV_64F, x, y, 5 );
    cv::imshow( "Sobel edge detection", prcedImg );
    return prcedImg;
}

cv::Mat Filtering::CannyEdgeDet( cv::Mat prcingImg, int thrMin, int thrMax ) {
    cv::Mat img_blur = EdgeDetPrep( prcingImg );
    cv::Mat prcedImg;
    cv::Canny( img_blur, prcedImg, thrMin, thrMax );
    cv::imshow( "Canny edge detection", prcedImg );
    return prcedImg;
}