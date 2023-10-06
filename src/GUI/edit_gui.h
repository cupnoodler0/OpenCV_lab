#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

class EditGUI {
   private:
    struct Menu_flags {
        bool resize_menu = false;
        bool gb = false;
        bool mb = false;
        bool bc = false;
        bool ced = false;
        bool sed = false;
        bool ck = false;
        bool thr = false;
        bool hsv = false;
    } menu_flags;
    struct Window_IsShown {
        bool cvTestWin_IsShown = false;
        bool srcImWin_IsShown = false;
        bool prcedImWin_IsShowing = false;
    } win_isShown;

   public:
    EditGUI();
    ~EditGUI();

    void ShowCustomKernelMenu() const;

    bool GetResizeMenuFlag() const;
    void SetResizeMenuFlag( bool value );

    bool GetBrightnessContrastMenuFlag() const;
    void SetBrightnessContrastMenuFlag( bool value );

    bool GetGaussianBlurMenuFlag() const;
    void SetGaussianBlurMenuFlag( bool value );

    bool GetMedianBlurMenuFlag() const;
    void SetMedianBlurMenuFlag( bool value );

    bool GetCannyEdgeDetMenuFlag() const;
    void SetCannyEdgeDetMenuFlag( bool value );

    bool GetSobelEdgeDetMenuFlag() const;
    void SetSobelEdgeDetMenuFlag( bool value );

    bool GetCustomKernelMenuFlag() const;
    void SetCustomKernelMenuFlag( bool value );

    bool GetThresholdMenuFlag() const;
    void SetThresholdMenuFlag( bool value );

    bool GetHSVMenuFlag() const;
    void SetHSVMenuFlag( bool value );
};