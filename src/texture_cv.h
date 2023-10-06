#pragma once

#include <opencv2/opencv.hpp>
#include "renderer.h"

class TextureCV {
   private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char *m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    cv::Mat m_Image;
    cv::Mat m_EditedImage;

   public:
    TextureCV( const std::string &path );
    ~TextureCV();
    /* OpenGL state */
    void Bind( unsigned int slot = 0 ) const;
    void Unbind() const;
    /* Render / Show the windows with the image */
    void OnImGuiRender();
    void Resize( int width, int height );
    /* Set new image */
    void Update( cv::Mat img );
    /* Filters */
    void ContrastBrightness( float *conVal, float *briVal );
    void GaussianBlur( int *val );
    void MedianBlur( int *val );
    void Threshold( int *minval, int *maxval,
                    cv::ThresholdTypes ttype = cv::THRESH_BINARY );
    void SobelEdgeDet( int x, int y );
    void CannyEdgeDet( int minval, int maxval );
    /* Size */
    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};