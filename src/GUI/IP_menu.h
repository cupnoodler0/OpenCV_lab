#pragma once

#include "../texture_cv.h"

class ImageProcessingMenu {
   public:
    TextureCV m_InputTexture;
    TextureCV m_OutputTexture;
    TextureCV m_HSVTexture;

    ImageProcessingMenu();
    ~ImageProcessingMenu();

    void OnImGuiRender();

   private:
    GLuint m_InImageID;
    GLuint m_OutImID;
    GLuint m_HSVImID;
    struct Images {
        cv::Mat src;
        cv::Mat processing;
        cv::Mat out;
        cv::Mat hsv;
    } m_Images;
    struct ImagePaths {
        char buf[255]{};
        char path0[255] = "res/textures/tiger.jpg";
        char path1[255] = "res/textures/wc.jpg";
        char path2[255];
    } m_ImagePaths;
    struct Kernels {
        const char* kernel[10] = { "blur",     "bottom sobel", "custom",
                                   "emboss",   "identity",     "left sobel",
                                   "outline",  "right sobel",  "sharpen",
                                   "top sobel" };
        const char* current = NULL;
    } m_Kernels;
    // struct convKernel {
    //     float Row[9]{ 0., 0., 0., 0., 0., 0., 0., 0., 0. };
    // } m_ConvKernel;  // REMOVE IT OR MAKE THE CONTAINER FOR THE PRESETS
    float m_ConvKernel[3][3]{ 0., 0., 0., 0., 0., 0., 0., 0., 0. };
    struct FilterParameters {
        /* Filters parameters */
        float brVl = 0;
        float cntVl = 1;
        int gbVal = 3;
        int mdbVal = 3;
        int thrMinVal = 0;
        int thrMaxVal = 255;
        int cannyMinVal = 100;
        int cannyMaxVal = 200;
        struct hsvVal {
            int h = 0;
            int s = 0;
            int v = 0;
        } hsv_values;
    } m_FilterParameters;
    /* Filters on/off */
    struct FilterFlags {
        bool brightChange_IsOn = false;
        bool contrastChange_IsOn = false;
        bool gb_IsOn = false;
        bool mdb_IsOn = false;
        bool thr_IsOn = false;
    } m_FilterFlags;

    /* EDIT GUI MENUES */
    struct MenuFlags {
        /* Filters controls */
        bool resize_IsShown = false;
        bool shear_IsShown = false;
        bool rotate_IsShown = false;
        bool reflect_IsShown = false;
        bool homography_IsShown = false;

        bool gb_IsShown = false;
        bool bc_IsShown = false;
        bool mb_IsShown = false;
        bool hsv_IsShown = false;
        bool ced_IsShown = false;
        bool sed_IsShown = false;
        bool ck_IsShown = false;
        bool thr_IsShown = false;
        /* Images windows */
        bool input_IsShown = false;
        bool cvTestWin_IsShown = false; /*???*/
        bool output_IsShowing = false;
    } m_MenuFlags;
};