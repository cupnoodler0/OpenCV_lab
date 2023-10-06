#include "texture_cv.h"

#include <imgui.h>
#include "../include/stb_image/stb_image.h"
#include "filter/filtering.h"

TextureCV::TextureCV( const std::string &path )
    : m_RendererID( 0 ),
      m_FilePath( path ),
      m_LocalBuffer( nullptr ),
      m_Width( 0 ),
      m_Height( 0 ),
      m_BPP( 0 ) {
    stbi_set_flip_vertically_on_load( 1 );
    /* Not used here, replaced with the Mat image */
    m_LocalBuffer = stbi_load( path.c_str(), &m_Width, &m_Height, &m_BPP, 4 );
    /* Shoud be a class member */
    m_Image = cv::imread( path.c_str() );

    m_Width = m_Image.cols;
    m_Height = m_Image.rows;

    m_EditedImage = m_Image.clone();
    // cv::resize( img, img, cv::Size( m_Width, m_Height ) );

    GLCall( glGenTextures( 1, &m_RendererID ) );
    GLCall( glBindTexture( GL_TEXTURE_2D, m_RendererID ) );

    GLCall(
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
    GLCall(
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
    GLCall(
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
    GLCall(
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );

    GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_Image.cols,
                          m_Image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE,
                          m_Image.ptr() ) );
    GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );

    if ( m_LocalBuffer ) stbi_image_free( m_LocalBuffer );
}

TextureCV::~TextureCV() { GLCall( glDeleteTextures( 1, &m_RendererID ) ); }

void TextureCV::Bind( unsigned int slot ) const {
    GLCall( glActiveTexture( GL_TEXTURE0 + slot ) );
    GLCall( glBindTexture( GL_TEXTURE_2D, m_RendererID ) );
}

void TextureCV::Unbind() const { GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) ); }

void TextureCV::OnImGuiRender() {
    ImGui::Image(
        reinterpret_cast<void *>( static_cast<intptr_t>( m_RendererID ) ),
        ImVec2( m_Width * .5f, m_Height * .5f ), ImVec2( 0.f, 0.f ),
        ImVec2( 1.f, 1.f ) );
    // ImGui::Image(
    //     &m_RendererID ,
    //     ImVec2( m_Image.cols * .5f, m_Image.rows * .5f ), ImVec2( 0.f, 0.f ),
    //     ImVec2( 1.f, 1.f ) );
}

void TextureCV::Resize(int width, int height){
    m_Width = width;
    m_Height = height;
}

void TextureCV::Update(cv::Mat img){
    // m_Image = img;
    Resize(img.cols, img.rows);
    m_EditedImage = img;

    GLCall( glBindTexture( GL_TEXTURE_2D, m_RendererID ) );
    GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height,
                          0, GL_RGB, GL_UNSIGNED_BYTE, m_EditedImage.ptr() ) );
    GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

void TextureCV::ContrastBrightness( float *conVal, float *briVal ) {
    cv::Mat m_EditImage = Filtering::ContrastBrightness( &m_Image, conVal, briVal );
    Update(m_EditImage);
}
void TextureCV::GaussianBlur( int *val ) {
    cv::Mat m_EditImage = Filtering::GaussianBlur( &m_Image, val );
    Update(m_EditImage);
}
void TextureCV::MedianBlur( int *val ) {
    cv::Mat m_EditImage = Filtering::MedianBlur( &m_Image, val );
    Update(m_EditImage);
}
void TextureCV::Threshold( int *minval, int *maxval,
                           cv::ThresholdTypes ttype) {
    cv::Mat m_EditImage = Filtering::Thresholding( &m_Image, minval, maxval, ttype );
    Update(m_EditImage);
}
void TextureCV::SobelEdgeDet( int x, int y) {
    cv::Mat m_EditImage = Filtering::SobelEdgeDet( m_Image, x, y );
    Update(m_EditImage);
}
void TextureCV::CannyEdgeDet( int minval, int maxval) {
    cv::Mat m_EditImage = Filtering::SobelEdgeDet( m_Image, minval, maxval );
    Update(m_EditImage);
}
