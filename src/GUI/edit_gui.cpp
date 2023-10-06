#include "edit_gui.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>

EditGUI::EditGUI() {}

EditGUI::~EditGUI() {}

void EditGUI::ShowCustomKernelMenu() const {
    if ( menu_flags.ck ) {
        if ( !ImGui::Begin( "Apply Custom Convoluton Kernel",
                            &menu_flags.ck ) ) {
            ImGui::End();
        } else {
            if ( ImGui::BeginCombo( "Kernel", current_kernel ) ) {
                for ( int n = 0; n < IM_ARRAYSIZE( kernels ); n++ ) {
                    bool is_selected = ( current_kernel == kernels[n] );
                    if ( ImGui::Selectable( kernels[n], is_selected ) ) {
                        current_kernel = kernels[n];
                    }
                    if ( is_selected ) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Text( "Convoluton kernel matrix" );
            ImGui::InputFloat3( "row1", ck.Row1, 2 );
            ImGui::InputFloat3( "row2", ck.Row2, 2 );
            ImGui::InputFloat3( "row3", ck.Row3, 2 );
            if ( ImGui::Button( "Apply" ) ) {
                cv::Mat result;
                /* Custom kernel settings */
                cv::Mat kernel = ( cv::Mat_<double>( 3, 3 ) <<  //
                                       ck.Row1[0],              //
                                   ck.Row1[1],                  //
                                   ck.Row1[2],                  //
                                   ck.Row2[0],                  //
                                   ck.Row2[1],                  //
                                   ck.Row2[2],                  //
                                   ck.Row3[0],                  //
                                   ck.Row3[1],                  //
                                   ck.Row3[2] );
                /* Apply this one instantly */
                cv::filter2D( srcImage, result, -1, kernel, cv::Point( -1, -1 ),
                              4 );
                cv::imshow( "Result", result );
            }
            ImGui::End();
        }
    }
}

bool EditGUI::GetResizeMenuFlag() const { return menu_flags.resize_menu; }
void EditGUI::SetResizeMenuFlag( bool value ) {
    menu_flags.resize_menu = value;
}

bool EditGUI::GetBrightnessContrastMenuFlag() const { return menu_flags.bc; }
void EditGUI::SetBrightnessContrastMenuFlag( bool value ) {
    menu_flags.bc = value;
}

bool EditGUI::GetGaussianBlurMenuFlag() const { return menu_flags.gb; }
void EditGUI::SetGaussianBlurMenuFlag( bool value ) { menu_flags.gb = value; }

bool EditGUI::GetMedianBlurMenuFlag() const { return menu_flags.mb; }
void EditGUI::SetMedianBlurMenuFlag( bool value ) { menu_flags.mb = value; }

bool EditGUI::GetCannyEdgeDetMenuFlag() const { return menu_flags.ced; }
void EditGUI::SetCannyEdgeDetMenuFlag( bool value ) { menu_flags.ced = value; }

bool EditGUI::GetSobelEdgeDetMenuFlag() const { return menu_flags.sed; }
void EditGUI::SetSobelEdgeDetMenuFlag( bool value ) { menu_flags.sed = value; }

bool EditGUI::GetCustomKernelMenuFlag() const { return menu_flags.ck; }
void EditGUI::SetCustomKernelMenuFlag( bool value ) { menu_flags.ck = value; }

bool EditGUI::GetThresholdMenuFlag() const { return menu_flags.thr; }
void EditGUI::SetThresholdMenuFlag( bool value ) { menu_flags.thr = value; }

bool EditGUI::GetHSVMenuFlag() const { return menu_flags.hsv; }
void EditGUI::SetHSVMenuFlag( bool value ) { menu_flags.hsv = value; }
