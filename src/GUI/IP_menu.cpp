#include "IP_menu.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <opencv2/highgui/highgui_c.h>

#include "../filter/filtering.h"
#include "../filter/custom_filtering.h"

ImageProcessingMenu::ImageProcessingMenu()
    : m_InputTexture( "res/textures/tiger.jpg" ),
      m_OutputTexture( "res/textures/tiger.jpg" ),
      m_HSVTexture( "res/textures/tiger.jpg" ) {
    GLCall( glGenTextures( 1, &m_InImageID ) );
    GLCall( glGenTextures( 1, &m_OutImID ) );
    GLCall( glGenTextures( 1, &m_HSVImID ) );
    /*  */
    m_Images.src = cv::imread( "res/textures/tiger.jpg" );
    m_Images.processing = m_Images.src.clone();
    m_Images.out = m_Images.src.clone();
    m_Images.hsv = m_Images.src.clone();
    /* Source paths */
    static char frstImPath[255] = "res/textures/tiger.jpg";
    static char scndImPath[255] = "res/textures/wc.jpg";
    char buf[255]{};
    /* Preset Kernels */
    const char* kernels[] = { "blur",     "bottom sobel", "custom",
                              "emboss",   "identity",     "left sobel",
                              "outline",  "right sobel",  "sharpen",
                              "top sobel" };
    static const char* current_kernel = NULL;
    struct convKernel {
        float Row1[3]{ 0., 0., 0. };
        float Row2[3]{ 0., 0., 0. };
        float Row3[3]{ 0., 0., 0. };
    };
    struct IdKernel {
        float Row1[3]{ 0., 0., 0. };
        float Row2[3]{ 0., 1., 0. };
        float Row3[3]{ 0., 0., 0. };
    };
    struct blurKernel {
        float Row1[3]{ .25 / 4, .25 / 2, .25 / 4 };
        float Row2[3]{ .25 / 2, .25, .25 / 2 };
        float Row3[3]{ .25 / 4, .25 / 2, .25 / 4 };
    };
    struct bSobelKernel {
        float Row1[3]{ -1., -2., -1. };
        float Row2[3]{ 0., 0., 0. };
        float Row3[3]{ 1., 2., 1. };
    };
    struct tSobelKernel {
        float Row3[3]{ 1., 2., 1. };
        float Row2[3]{ 0., 0., 0. };
        float Row1[3]{ -1., -2., -1. };
    };
    struct rSobelKernel {
        float Row3[3]{ -1., 0., 1. };
        float Row2[3]{ -2., 0., 2. };
        float Row1[3]{ -1., 0., 1. };
    };
    struct lSobelKernel {
        float Row3[3]{ 1., 0., -1. };
        float Row2[3]{ 2., 0., -2. };
        float Row1[3]{ 1., 0., -1. };
    };
    struct embossKernel {
        float Row3[3]{ -2., -1., 0. };
        float Row2[3]{ -1., 1., 1. };
        float Row1[3]{ 0., 1., 2. };
    };
    struct outlineKernel {
        float Row3[3]{ -1., -1., -1. };
        float Row2[3]{ -1., 8., -1. };
        float Row1[3]{ -1., -1., -1. };
    };
    struct sharpenKernel {
        float Row3[3]{ 0., -1., 0. };
        float Row2[3]{ -1., 5., -1. };
        float Row1[3]{ 0., -1., 0. };
    };
    // struct convKernel {
    //     float Row1[3]{ 0., 0., 0. };
    //     float Row2[3]{ 0., 0., 0. };
    //     float Row3[3]{ 0., 0., 0. };
    // };
    convKernel ck;
}

ImageProcessingMenu::~ImageProcessingMenu() {}

void ImageProcessingMenu::OnImGuiRender() {
    /* Image Processing Window */
    ImGui::Begin( "Image Processing", NULL, ImGuiWindowFlags_MenuBar );
    /* Options */
    if ( ImGui::BeginMenuBar() ) {
        if ( ImGui::BeginMenu( "File" ) ) {
            if ( ImGui::MenuItem( "Open", "CTRL+O" ) ) {
                /**/
            }
            if ( ImGui::MenuItem( "Save", "CTRL+S" ) ) {
                /**/
            }
            ImGui::Separator();
            if ( ImGui::MenuItem( "Save As...", "SHIFT+CTRL+S" ) ) {
                /**/
            }
            if ( ImGui::MenuItem( "Exit", "CTRL+Q" ) ) {
                printf( "The program could be terminated.\n" );
            }
            ImGui::EndMenu();
        }
        if ( ImGui::BeginMenu( "Edit" ) ) {
            if ( ImGui::MenuItem( "Undo", "CTRL+Z" ) ) {
                /**/
            }
            if ( ImGui::MenuItem( "Redo", "CTRL+Y" ) ) {
                /**/
            }
            ImGui::Separator();
            if ( ImGui::MenuItem( "Cut", "CTRL+X" ) ) {
                /**/
            }
            if ( ImGui::MenuItem( "Copy", "CTRL+C" ) ) {
                /**/
            }
            if ( ImGui::MenuItem( "Paste", "CTRL+V" ) ) {
                /**/
            }
            ImGui::EndMenu();
        }
        /* Operations with the image */
        if ( ImGui::BeginMenu( "Image" ) ) {
            if ( ImGui::MenuItem( "Resize..." ) ) {
                m_MenuFlags.resize_IsShown = !m_MenuFlags.resize_IsShown;
            }
            if ( ImGui::MenuItem( "Shift..." ) ) {
                m_MenuFlags.shear_IsShown = !m_MenuFlags.shear_IsShown;
            }
            if ( ImGui::MenuItem( "Rotate..." ) ) {
                m_MenuFlags.rotate_IsShown = !m_MenuFlags.rotate_IsShown;
            }
            if ( ImGui::MenuItem( "Reflect..." ) ) {
                m_MenuFlags.reflect_IsShown = !m_MenuFlags.reflect_IsShown;
            }
            if ( ImGui::MenuItem( "Homography" ) ) {
                m_MenuFlags.homography_IsShown =
                    !m_MenuFlags.homography_IsShown;
            }
            ImGui::EndMenu();
        }
        /* IMAGE ADJUSTMENTS */
        if ( ImGui::BeginMenu( "Adj" ) ) {
            if ( ImGui::MenuItem( "Brightness/Contrast" ) ) {
                m_MenuFlags.bc_IsShown = !m_MenuFlags.bc_IsShown;
            }
            ImGui::EndMenu();
        }
        /* FILTERS */
        if ( ImGui::BeginMenu( "Filter" ) ) {
            if ( ImGui::MenuItem( "B&W" ) ) {
                cv::Mat img_gray;
                cv::cvtColor( m_Images.src, img_gray, cv::COLOR_BGR2GRAY );
                cv::Mat resized_down;
                double s = .5;
                cv::resize( img_gray, resized_down, cv::Size(), s, s,
                            cv::INTER_LINEAR );
                cv::imshow( "B&W", resized_down );
            }
            if ( ImGui::BeginMenu( "Blur" ) ) {
                if ( ImGui::MenuItem( "Gaussian Blur" ) ) {
                    m_MenuFlags.gb_IsShown = !m_MenuFlags.gb_IsShown;
                }
                if ( ImGui::MenuItem( "Median Blur" ) ) {
                    m_MenuFlags.mb_IsShown = !m_MenuFlags.mb_IsShown;
                }
                ImGui::EndMenu();
            }
            if ( ImGui::BeginMenu( "Custom Filters" ) ) {
                if ( ImGui::MenuItem( "Sepia filter" ) ) {
                    cv::Mat resized_down;
                    double s = .5;
                    cv::resize( m_Images.src, resized_down, cv::Size(), s, s,
                                cv::INTER_LINEAR );
                    CustomFiltering::Sepia( &resized_down );
                }
                if ( ImGui::MenuItem( "Watercolor filter" ) ) {
                    cv::Mat resized_down;
                    double s = .5;
                    cv::resize( m_Images.src, resized_down, cv::Size(), s, s,
                                cv::INTER_LINEAR );
                    CustomFiltering::Watercolor( &resized_down );
                }
                if ( ImGui::MenuItem( "Cartoon filter" ) ) {
                    cv::Mat resized_down;
                    double s = .5;
                    cv::resize( m_Images.src, resized_down, cv::Size(), s, s,
                                cv::INTER_LINEAR );
                    CustomFiltering::Cartoon( &resized_down );
                }
                ImGui::EndMenu();
            }
            if ( ImGui::MenuItem( "Custom Kernel" ) ) {
                m_MenuFlags.ck_IsShown = !m_MenuFlags.ck_IsShown;
            }
            ImGui::EndMenu();
        }
        /* COLORS */
        if ( ImGui::BeginMenu( "Color" ) ) {
            if ( ImGui::BeginMenu( "Get color values" ) ) {
                if ( ImGui::MenuItem( "Get Red" ) ) {
                    std::vector<cv::Mat> chans( 3 );
                    cv::split( m_Images.src, chans );
                    cv::Mat resized_down;
                    cv::resize( chans[2], resized_down, cv::Size(), .5, .5,
                                cv::INTER_LINEAR );
                    cv::imshow( "Green ch only", resized_down );
                }
                if ( ImGui::MenuItem( "Get Green" ) ) {
                    std::vector<cv::Mat> chans( 3 );
                    cv::split( m_Images.src, chans );
                    cv::Mat resized_down;
                    cv::resize( chans[1], resized_down, cv::Size(), .5, .5,
                                cv::INTER_LINEAR );
                    cv::imshow( "Green ch only", resized_down );
                }
                if ( ImGui::MenuItem( "Get Blue" ) ) {
                    std::vector<cv::Mat> chans( 3 );
                    cv::split( m_Images.src, chans );
                    cv::Mat resized_down;
                    cv::resize( chans[0], resized_down, cv::Size(), .5, .5,
                                cv::INTER_LINEAR );
                    cv::imshow( "Green ch only", resized_down );
                }
                ImGui::EndMenu();
            }
            if ( ImGui::BeginMenu( "Bitwise operations" ) ) {
                if ( ImGui::MenuItem( "Bitwise and" ) ) {
                    cv::Mat im1 = cv::imread( m_ImagePaths.path0 );
                    cv::Mat im2 = cv::imread( m_ImagePaths.path1 );
                    cv::Mat im2_resized;
                    cv::resize( im2, im2_resized,
                                cv::Size( im1.cols, im1.rows ) );
                    if ( im1.empty() && im2.empty() ) {
                        printf( "Something wrong with your pics." );
                    } else {
                        cv::Mat result;
                        cv::bitwise_and( im1, im2_resized, result );
                        cv::imshow( "Addition func", result );
                    }
                }
                if ( ImGui::MenuItem( "Bitwise or" ) ) {
                    cv::Mat im1 = cv::imread( m_ImagePaths.path0 );
                    cv::Mat im2 = cv::imread( m_ImagePaths.path1 );
                    cv::Mat im2_resized;
                    cv::resize( im2, im2_resized,
                                cv::Size( im1.cols, im1.rows ) );
                    if ( im1.empty() && im2.empty() ) {
                        printf( "Something wrong with your pics." );
                    } else {
                        cv::Mat result;
                        cv::bitwise_or( im1, im2_resized, result );
                        cv::imshow( "Addition func", result );
                    }
                }
                if ( ImGui::MenuItem( "Bitwise not" ) ) {
                    cv::Mat im1 = cv::imread( m_ImagePaths.path0 );
                    cv::Mat im2 = cv::imread( m_ImagePaths.path1 );
                    cv::Mat im2_resized;
                    cv::resize( im2, im2_resized,
                                cv::Size( im1.cols, im1.rows ) );
                    if ( im1.empty() && im2.empty() ) {
                        printf( "Something wrong with your pics." );
                    } else {
                        cv::Mat result;
                        cv::bitwise_not( im1, result, result );
                        cv::imshow( "Addition func", result );
                    }
                }
                ImGui::EndMenu();
            }
            if ( ImGui::BeginMenu( "Edge Detection" ) ) {
                if ( ImGui::MenuItem( "Canny" ) ) {
                    m_MenuFlags.ced_IsShown = !m_MenuFlags.ced_IsShown;
                }
                if ( ImGui::MenuItem( "Sobel" ) ) {
                    cv::Mat resized_down;
                    /* Should be controlled parameters */
                    double s = .6;
                    cv::resize( m_Images.src, resized_down, cv::Size(), s, s,
                                cv::INTER_LINEAR );
                    /* x & y should be controlled parameters */
                    int x = 1;
                    int y = 1;
                    Filtering::SobelEdgeDet( resized_down, x, y );
                }
                ImGui::EndMenu();
            }
            if ( ImGui::MenuItem( "Open HSV edit" ) ) {
                m_MenuFlags.hsv_IsShown = !m_MenuFlags.hsv_IsShown;
            }
            if ( ImGui::MenuItem( "Threshold" ) ) {
                m_MenuFlags.thr_IsShown = !m_MenuFlags.thr_IsShown;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text( "Images to do with" );
    ImGui::InputText( "Image path", m_ImagePaths.path0,
                      sizeof( m_ImagePaths.buf ) - 1 );
    ImGui::InputText( "Path to second image", m_ImagePaths.path1,
                      sizeof( m_ImagePaths.buf ) - 1 );
    /* Doesn't work */
    if ( ImGui::Button( "Load images" ) ) {
        /* Load new image */
        cv::Mat srcImage = cv::imread( m_ImagePaths.path0 );
        /* Update variables */
        m_Images.processing = srcImage.clone();
        m_Images.out = srcImage.clone();
        m_Images.hsv = srcImage.clone();

        if ( srcImage.empty() ) {
            printf( "Something wrong with your pics." );
        }
    }

    ImGui::Separator();
    /* ADDITIONAL MENUES */
    m_InputTexture.OnImGuiRender();
    /* The window for the source image */
    // if ( ImGui::Button( "Show Source Image" ) ) {
    //     m_MenuFlags.input_IsShown = !m_MenuFlags.input_IsShown;
    // }
    // if ( m_MenuFlags.input_IsShown ) {
    //     if ( !ImGui::Begin( "Source Image", &m_MenuFlags.input_IsShown ) ) {
    //         ImGui::End();
    //     } else {
    //         m_InputTexture.OnImGuiRender();
    //         ImGui::End();
    //         // GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
    //     }
    // }
    ImGui::SameLine();
    m_OutputTexture.OnImGuiRender();
    /* The window for the processed image */
    // if ( ImGui::Button( "Show Processed Image" ) ) {
    //     m_MenuFlags.cvTestWin_IsShown = !m_MenuFlags.cvTestWin_IsShown;
    // }
    // if ( m_MenuFlags.cvTestWin_IsShown ) {
    //     if ( !ImGui::Begin( "CV Texture test",
    //                         &m_MenuFlags.cvTestWin_IsShown ) ) {
    //         ImGui::End();
    //     } else {
    //         m_OutputTexture.OnImGuiRender();
    //         ImGui::End();
    //         // GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
    //     }
    // }

    /* MainMenu -> Image -> */
    /* The Resize image menu */
    if ( m_MenuFlags.resize_IsShown ) {
        if ( !ImGui::Begin( "Resize", &m_MenuFlags.resize_IsShown ) ) {
            ImGui::End();
        } else {
            /* Parameters*/
            static int width = 320;
            static int height = 200;
            static bool byPrc = false;
            static int sizeByPrc = 100;
            static bool byAbs = true;
            static bool aspRat = false;

            ImGui::Text( "Resampling: " );
            ImGui::SameLine( 200 );
            ImGui::Text( "*here will be a combobox*" );
            // ImGui::combo();

            ImGui::Checkbox( "By percentage: ", &byPrc );
            if ( byPrc ) {
                byAbs = false;
                ImGui::SameLine( 200 );
                ImGui::InputInt( "%", &sizeByPrc );
            }
            ImGui::Checkbox( "By absolute size: ", &byAbs );
            if ( byAbs ) {
                byPrc = false;
                ImGui::Text( "    " );
                ImGui::SameLine();
                if ( ImGui::Checkbox( "Maintain aspect ratio", &aspRat ) ) {
                }
                ImGui::Separator();
                ImGui::Text( "Pixel size" );
                /* Setting width */
                ImGui::Text( "Width: " );
                ImGui::SameLine( 100 );
                ImGui::InputInt( "wpixels", &width, 10 );
                /* Setting height */
                ImGui::Text( "Height: " );
                ImGui::SameLine( 100 );
                ImGui::InputInt( "hpixels", &height, 10 );
            }
            ImGui::Separator();
            if ( ImGui::Button( "OK" ) ) { /* TextureCV func needed */
                cv::Mat img_resized;
                if ( byPrc ) { /* By percentage */
                    cv::resize( m_Images.src, img_resized, cv::Size(),
                                sizeByPrc / 100., sizeByPrc / 100. );
                }
                if ( byAbs ) { /* By absolute size */
                    cv::resize( m_Images.src, img_resized,
                                cv::Size( width, height ) );
                }
                // outCVTex.SetWidth(img_resized.cols);
                // outCVTex.SetHeight(img_resized.rows);
                m_OutputTexture.Update( img_resized );
                m_MenuFlags.resize_IsShown = false;
            }
            ImGui::SameLine();
            if ( ImGui::Button( "Cancel" ) ) {
                m_MenuFlags.resize_IsShown = false;
            }
            ImGui::End();
        }
    }
    if ( m_MenuFlags.shear_IsShown ) {
        if ( !ImGui::Begin( "Shearing by axis", &m_MenuFlags.shear_IsShown ) ) {
            ImGui::End();
        } else {
            /* Parameters*/
            ImGui::Text( "Randomely shear the image" );  // not really...

            ImGui::Separator();
            if ( ImGui::Button( "OK" ) ) {
                cv::Point2f srcTri[3];
                srcTri[0] = cv::Point2f( 0.f, 0.f );
                srcTri[1] = cv::Point2f( m_Images.src.cols - 1.f, 0.f );
                srcTri[2] = cv::Point2f( 0.f, m_Images.src.rows - 1.f );
                cv::Point2f dstTri[3];
                dstTri[0] = cv::Point2f( 0.f, m_Images.src.rows * 0.33f );
                dstTri[1] = cv::Point2f( m_Images.src.cols * 0.85f,
                                         m_Images.src.rows * 0.25f );
                dstTri[2] = cv::Point2f( m_Images.src.cols * 0.15f,
                                         m_Images.src.rows * 0.7f );

                cv::Mat warp_mat = cv::getAffineTransform( srcTri, dstTri );

                cv::Mat warp_dst = cv::Mat::zeros(
                    m_Images.src.rows, m_Images.src.cols, m_Images.src.type() );
                cv::warpAffine( m_Images.src, warp_dst, warp_mat,
                                warp_dst.size() );

                m_OutputTexture.Update( warp_dst );
                m_MenuFlags.shear_IsShown = false;
            }
            ImGui::SameLine();
            if ( ImGui::Button( "Cancel" ) ) {
                m_MenuFlags.shear_IsShown = false;
            }
            ImGui::End();
        }
    }
    if ( m_MenuFlags.rotate_IsShown ) {
        if ( !ImGui::Begin( "Resize", &m_MenuFlags.rotate_IsShown ) ) {
            ImGui::End();
        } else {
            /* Parameters*/
            static int anchor_x = 50;  // 0 - image.cols. It may be outside, but
                                       // idk how to do it..
            static int anchor_y = 60;  // 0 - image.rows. It may be outside
            static int angle = 20;     // 0 - 359

            ImGui::Separator();
            ImGui::Text( "Point for the anchor: " );
            // ImGui::InputInt( "x: ", &anchor_x, 10 );
            ImGui::SliderInt( "x: ", &anchor_x, 0, m_Images.src.cols );
            // ImGui::InputInt( "y: ", &anchor_y, 10 );
            ImGui::SliderInt( "y: ", &anchor_y, 0, m_Images.src.rows );

            ImGui::Text( "Rotate: " );
            // ImGui::InputInt( "°", &degree );
            ImGui::SliderInt( "°", &angle, 0, 359 );

            ImGui::Separator();
            if ( ImGui::Button( "OK" ) ) { /* TextureCV func needed */
                cv::Point center = cv::Point( anchor_x, anchor_y );
                cv::Mat rot_mat = cv::getRotationMatrix2D( center, angle, 1.0 );

                cv::Mat warp_rotate_dst;
                cv::warpAffine( m_Images.src, warp_rotate_dst, rot_mat,
                                m_Images.src.size() );

                m_OutputTexture.Update( warp_rotate_dst );
                m_MenuFlags.rotate_IsShown = false;
            }
            ImGui::SameLine();
            if ( ImGui::Button( "Cancel" ) ) {
                m_MenuFlags.rotate_IsShown = false;
            }
            ImGui::End();
        }
    }
    if ( m_MenuFlags.reflect_IsShown ) {
        if ( !ImGui::Begin( "Resize", &m_MenuFlags.reflect_IsShown ) ) {
            ImGui::End();
        } else {
            /* Parameters*/
            static bool flipHorizontally = false;
            static bool flipVertically = false;

            ImGui::Checkbox( "Flip horizontally", &flipHorizontally );
            ImGui::Checkbox( "Flip vertically", &flipVertically );

            ImGui::Separator();
            if ( ImGui::Button( "OK" ) ) { /* TextureCV func needed */
                cv::Mat flip_dst;
                ImGui::Separator();
                if ( flipHorizontally && flipVertically ) {
                    cv::flip( m_Images.src, flip_dst, -1 );
                } else if ( flipHorizontally ) {
                    cv::flip( m_Images.src, flip_dst, 0 );
                } else if ( flipVertically ) {
                    cv::flip( m_Images.src, flip_dst, 1 );
                } else {
                    flip_dst = m_Images.src;
                }

                m_OutputTexture.Update( flip_dst );
                m_MenuFlags.reflect_IsShown = false;
            }
            ImGui::SameLine();
            if ( ImGui::Button( "Cancel" ) ) {
                m_MenuFlags.reflect_IsShown = false;
            }
            ImGui::End();
        }
    }
    if ( m_MenuFlags.homography_IsShown ) {
        if ( !ImGui::Begin( "Resize", &m_MenuFlags.homography_IsShown ) ) {
            ImGui::End();
        } else {
            int k = 0;

            // clone the image to place the points
            cv::Mat im_src = m_Images.src.clone();
            // hardcode the points
            std::vector<cv::Point2f> pts_src;
            pts_src.push_back( cv::Point2f( 141, 131 ) );
            pts_src.push_back( cv::Point2f( 458, 168 ) );
            pts_src.push_back( cv::Point2f( 500, 649 ) );
            pts_src.push_back( cv::Point2f( 64, 601 ) );

            // Here we just copy the image
            cv::Mat im_dst = im_src.clone();
            // Set the corner points to fit the prev image points to these
            std::vector<cv::Point2f> pts_corners;
            pts_corners.push_back( cv::Point2f( 0, 0 ) );
            pts_corners.push_back( cv::Point2f( 0, im_src.rows ) );
            pts_corners.push_back( cv::Point2f( im_src.cols, im_src.rows ) );
            pts_corners.push_back( cv::Point2f( im_src.cols, 0 ) );

            // Next use build-in opencv functions to transform the image
            cv::Mat h = cv::findHomography( pts_src, pts_corners );
            cv::Mat im_out;

            cv::warpPerspective( im_src, im_out, h, im_dst.size() );

            // Static points container
            static struct input_params {
                std::vector<cv::Point2i> vertices;
                cv::Mat* canvas;
            } params{ std::vector<cv::Point2i>(), &im_src };

            // Canvas to shoose the points
            // First, if all vertices are chosen, close the window
            if ( params.vertices.size() == 4 &&
                 cv::getWindowProperty( "Set indices", cv::WND_PROP_VISIBLE ) >=
                     1 ) {
                cv::destroyWindow( "Set indices" );
                // Otherwise we filling the vector with new points which have
                // coords gotten by mouse clicks
            } else if ( params.vertices.size() < 4 ) {
                cv::imshow( "Set indices", im_src );
                cv::setMouseCallback(
                    "Set indices",
                    []( int event, int x, int y, int flags, void* params ) {
                        if ( event == cv::EVENT_LBUTTONDOWN ) {
                            auto prm = (input_params*)params;
                            prm->vertices.push_back( cv::Point2f( x, y ) );
                            std::printf( "x: %d, y: %d\n", x, y );
                            std::printf( "vertices count: %d\n",
                                         prm->vertices.size() );

                            cv::ellipse( *( prm->canvas ), cv::Point( x, y ),
                                         cv::Size( 3, 3 ), 0, 0, 360,
                                         cv::Scalar( 0, 0, 255 ), 2 );
                        }
                    },
                    (void*)&params );
            }

            ImGui::Separator();
            if ( ImGui::Button( "OK" ) ) {
                h = cv::findHomography( params.vertices, pts_corners );
                im_out;

                cv::warpPerspective( im_src, im_out, h, im_dst.size() );
                cv::imshow( "homography img", im_out );
                m_MenuFlags.homography_IsShown = false;

                params.vertices.clear();
            }
            ImGui::SameLine();
            if ( ImGui::Button( "Cancel" ) ) {
                m_MenuFlags.homography_IsShown = false;
            }
            ImGui::End();
        }
    }
    /* Threshold edit menu */
    if ( m_MenuFlags.thr_IsShown ) {
        if ( !ImGui::Begin( "Threshold", &m_MenuFlags.thr_IsShown ) ) {
            ImGui::End();
        } else {
            ImGui::Checkbox( "Threshold", &m_FilterFlags.thr_IsOn );
            if ( m_FilterFlags.thr_IsOn ) {
                ImGui::SliderInt( "Threshold min value",
                                  &m_FilterParameters.thrMinVal, 0, 255 );
                ImGui::NextColumn();
                ImGui::SliderInt( "Threshold max value",
                                  &m_FilterParameters.thrMaxVal, 0, 255 );
                ImGui::NextColumn();

                m_OutputTexture.Threshold( &m_FilterParameters.thrMinVal,
                                           &m_FilterParameters.thrMaxVal,
                                           cv::THRESH_BINARY );
            }
            ImGui::End();
        }
    }
    /* Brightness/Contrast */
    if ( m_MenuFlags.bc_IsShown ) {
        if ( !ImGui::Begin( "Brightness/Contrast", &m_MenuFlags.bc_IsShown ) ) {
            ImGui::End();
        } else {
            ImGui::SliderFloat( "Brightness", &m_FilterParameters.brVl, -255,
                                255 );
            ImGui::SliderFloat( "Contrast", &m_FilterParameters.cntVl, 0.001,
                                5 );
            m_OutputTexture.ContrastBrightness( &m_FilterParameters.cntVl,
                                                &m_FilterParameters.brVl );
            if ( ImGui::Button( "Reset" ) ) {
                m_FilterParameters.brVl = 0;
                m_FilterParameters.cntVl = 1;
            }
            ImGui::End();
        }
    }
    /*  Gaussian blur menu is shown/off
        (open with Filter->Gaussian Blur) */
    if ( m_MenuFlags.gb_IsShown ) {
        if ( !ImGui::Begin( "Gaussian Blur", &m_MenuFlags.gb_IsShown ) ) {
            ImGui::End();
        } else {
            ImGui::SliderInt( "Gaussian blur", &m_FilterParameters.gbVal, 3,
                              55 );
            m_OutputTexture.GaussianBlur( &m_FilterParameters.gbVal );
            ImGui::End();
        }
    }
    /*  Median blur menu */
    if ( m_MenuFlags.mb_IsShown ) {
        if ( !ImGui::Begin( "Median Blur", &m_MenuFlags.mb_IsShown ) ) {
            ImGui::End();
        } else {
            ImGui::SliderInt( "Median blur", &m_FilterParameters.mdbVal, 3,
                              55 );
            if ( ImGui::Button( "Apply MB" ) ) {
                if ( ImGui::IsMouseReleased( 0 ) ) {
                    m_OutputTexture.MedianBlur( &m_FilterParameters.mdbVal );
                }
            }
            ImGui::End();
        }
    }
    /* Canny Edge Detection menu */
    if ( m_MenuFlags.ced_IsShown ) {
        if ( !ImGui::Begin( "Canny ED", &m_MenuFlags.ced_IsShown ) ) {
            ImGui::End();
        } else {
            ImGui::Columns( 2 );
            ImGui::SliderInt( "Canny Min", &m_FilterParameters.cannyMinVal, 0,
                              255 );
            ImGui::NextColumn();
            ImGui::SliderInt( "Canny Max", &m_FilterParameters.cannyMaxVal, 0,
                              255 );
            ImGui::Columns( 1 );
            if ( ImGui::Button( "Apply" ) ) {
                cv::Mat resized_down;
                double s = .6;
                cv::resize( m_Images.src, resized_down, cv::Size(), s, s,
                            cv::INTER_LINEAR );
                /* min & max vals should be controlled parameters */
                Filtering::CannyEdgeDet( resized_down,
                                         m_FilterParameters.cannyMinVal,
                                         m_FilterParameters.cannyMaxVal );
            }
            ImGui::End();
        }
    }
    /* Custom Conv Kernel menu NOT DONE*/
    if ( m_MenuFlags.ck_IsShown ) {
        if ( !ImGui::Begin( "Apply Custom Convoluton Kernel",
                            &m_MenuFlags.ck_IsShown ) ) {
            ImGui::End();
        } else {
            if ( ImGui::BeginCombo( "Kernel", m_Kernels.current ) ) {
                for ( int n = 0; n < IM_ARRAYSIZE( m_Kernels.kernel ); n++ ) {
                    bool is_selected =
                        ( m_Kernels.current == m_Kernels.kernel[n] );
                    if ( ImGui::Selectable( m_Kernels.kernel[n],
                                            is_selected ) ) {
                        m_Kernels.current = m_Kernels.kernel[n];
                    }
                    if ( is_selected ) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Text( "Convoluton kernel matrix" );
            ImGui::InputFloat3( "row1", m_ConvKernel[0], "%.2f" );
            ImGui::InputFloat3( "row2", m_ConvKernel[1], "%.2f" );
            ImGui::InputFloat3( "row3", m_ConvKernel[2], "%.2f" );
            if ( ImGui::Button( "Apply" ) ) {
                if ( ImGui::IsMouseReleased( 0 ) ) {
                    cv::Mat im_src = m_Images.src.clone();
                    /* Custom kernel settings */
                    // cv::Mat kernel = ( cv::Mat_<double>( 3, 3 ) <<  //
                    //                        m_ConvKernel[0][0],    //
                    //                        m_ConvKernel[0][1],        //
                    //                        m_ConvKernel[0][2],        //
                    //                        m_ConvKernel[1][0],        //
                    //                        m_ConvKernel[1][1],        //
                    //                        m_ConvKernel[1][2],        //
                    //                        m_ConvKernel[2][0],        //
                    //                        m_ConvKernel[2][1],        //
                    //                        m_ConvKernel[2][2] );

                    cv::Mat output( im_src.size(), CV_8UC3 );
                    int kernelSize = 3;

                    // For each pixel of src image...
                        for ( int col = 0; col < im_src.cols; col++ ) {
                    for ( int row = 0; row < im_src.rows; row++ ) {
                            // Separate each color channel
                            // std::vector<uchar> kernelB;
                            // std::vector<uchar> kernelG;
                            // std::vector<uchar> kernelR;

                            cv::Rect roi( col - kernelSize / 2,
                                          row - kernelSize / 2, kernelSize,
                                          kernelSize );
                            // Ensure ROI is within image bounds
                            roi &= cv::Rect( 0, 0, im_src.cols, im_src.rows );

                            cv::Mat kernel = im_src( roi );

                            uchar colorB;
                            uchar colorG;
                            uchar colorR;


                            for ( int y = 0; y < kernel.rows; y++ ) {
                                for ( int x = 0; x < kernel.cols; x++ ) {
                            std::printf("kernel width: %d, point:(%d, %d), kernel:(%d, %d)\n",
                                            kernel.cols, col, row, x, y);
                                    colorB += kernel.at<cv::Vec3b>( x, y )[0] *
                                              m_ConvKernel[x][y];
                                    // colorG += kernel.at<cv::Vec3b>( x, y )[1] *
                                    //           m_ConvKernel[x][y];
                                    // colorR += kernel.at<cv::Vec3b>( x, y )[2] *
                                    //           m_ConvKernel[x][y];
                                }
                            }
                            output.at<cv::Vec3b>( col, row )[0] = colorB;
                            // output.at<cv::Vec3b>( col, row )[1] = colorG;
                            // output.at<cv::Vec3b>( col, row )[2] = colorR;
                        }
                    }

                    // ---

                    // cv::filter2D( m_Images.src, result, -1, kernel,
                    //               cv::Point( -1, -1 ), 4 );
                    // cv::imshow( "Result", result );
                    cv::imshow( "Result", output );
                }
            }
            ImGui::End();
        }
    }
    /* HSV menu is shown/off (open with Color->HSV) */
    if ( m_MenuFlags.hsv_IsShown ) {
        ImGui::Begin( "HSV editable", &m_MenuFlags.hsv_IsShown );
        /* This &colors[0] gives RED channel instead of proper H */

        cv::Mat hsvim;
        cv::cvtColor( m_Images.src, hsvim, cv::COLOR_BGR2HSV );
        cv::Mat hsvim_edit = hsvim.clone();

        /* HSV channels splitted out to process each one */
        std::vector<cv::Mat> hsv_chans( 3 );
        std::vector<cv::Mat> hsv_chans_edit( 3 );
        /* slpit */
        cv::split( hsvim, hsv_chans );
        /* editing HSV channels */
        cv::split( hsvim_edit, hsv_chans_edit );

        ImGui::SliderInt( "H", &m_FilterParameters.hsv_values.h, 0, 180 );
        cv::add( hsv_chans[0], m_FilterParameters.hsv_values.h,
                 hsv_chans_edit[0] );
        ImGui::SliderInt( "S", &m_FilterParameters.hsv_values.s, -255, 255 );
        cv::add( hsv_chans[1], m_FilterParameters.hsv_values.s,
                 hsv_chans_edit[1] );
        ImGui::SliderInt( "V", &m_FilterParameters.hsv_values.v, -255, 255 );
        cv::add( hsv_chans[2], m_FilterParameters.hsv_values.v,
                 hsv_chans_edit[2] );

        cv::merge( hsv_chans_edit, hsvim_edit );

        cv::Mat out;
        cv::cvtColor( hsvim_edit, out, cv::COLOR_HSV2BGR );

        m_HSVTexture.Update( out );
        m_HSVTexture.OnImGuiRender();
        ImGui::End();
        GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
    }

    ImGui::End();
}
