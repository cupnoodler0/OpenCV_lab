#include "custom_filtering.h"
#include "filtering.h"

cv::Mat CustomFiltering::Sepia( cv::Mat *prcingImg ) {
    cv::Mat prcedImg;
    /* Gray scale */
    // cv::Mat img_gray;
    // cv::cvtColor( *prcingImg, img_gray, cv::COLOR_BGR2GRAY );
    /* Normalize */
    // cv::Mat normalized;
    // cv::normalize( img_gray, normalized, 255, cv::NORM_MINMAX );
    /* Sepia color setting */
    cv::Mat kernel = ( cv::Mat_<double>( 3, 3 ) << 0.272, 0.534, 0.131,  //
                       0.349, 0.686, 0.168,                              //
                       0.349, 0.769, 0.189 );
    cv::transform( *prcingImg, prcedImg, kernel );
    cv::imshow( "Sepia filter", prcedImg );

    return prcedImg;
}

cv::Mat CustomFiltering::Watercolor( cv::Mat *prcingImg ) {
    cv::Mat prcedImg;
    int blurVal = 5;
    prcedImg = Filtering::GaussianBlur( prcingImg, &blurVal );
    prcedImg = Filtering::MedianBlur( prcingImg, &blurVal );
    prcedImg.convertTo( prcedImg, -1, 0.8, 10 );
    cv::Mat maskImg = cv::imread( "res/textures/wc.jpg" );
    cv::Mat maskImg_resized;
    cv::resize( maskImg, maskImg_resized,
                cv::Size( prcedImg.cols, prcedImg.rows ), cv::INTER_AREA );
    cv::GaussianBlur( maskImg_resized, maskImg_resized, cv::Size( 19, 19 ), 0.,
                      0. );
    // cv::addWeighted( prcedImg, 0.7, maskImg_resized, 0.3, 0, prcedImg );
    cv::multiply( prcedImg, maskImg_resized, prcedImg, 0.005 );
    // cv::transform( *prcingImg, prcedImg, kernel );
    // prcedImg = Filtering::GaussianBlur(*prcedImg, 5);
    cv::imshow( "Watercolor filter", prcedImg );

    return prcedImg;
}

cv::Mat CustomFiltering::Cartoon( cv::Mat *prcingImg ) {
    cv::Mat srcIm = *prcingImg;
    cv::Mat prcedImg;
    cv::Mat maskImg;
    cv::Mat gray_scale;
    cv::cvtColor( srcIm, gray_scale, cv::COLOR_BGR2GRAY );
    cv::Mat blurIm;
    cv::GaussianBlur(gray_scale, blurIm, cv::Size( 5, 5 ),
                      0., 0. );
    cv::threshold( blurIm, maskImg, 100, 255,
                   cv::ThresholdTypes::THRESH_BINARY );
    cv::cvtColor( maskImg, maskImg, cv::COLOR_GRAY2BGR);
    cv::bitwise_and( srcIm, maskImg, prcedImg );
    cv::imshow( "Cartoon filter", prcedImg );

    return prcedImg;
}
