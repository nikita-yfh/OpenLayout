#pragma once
#include "Settings.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/treebook.h>

class SettingsDialog: public wxDialog {
public:
    SettingsDialog(wxWindow* parent,const Settings &s);
    virtual ~SettingsDialog();

    void Set(const Settings s);
    void Get(Settings &s);

    ColorScheme colors[3];


protected:

    wxPanel *panel_colors;
    wxPanel *panel_dirs;
    wxPanel *panel_crosshair;
    wxPanel *panel_autosave;
    wxChoice *colorselection;
    wxDirPickerCtrl *macro_dir;
    wxRadioButton *rb_black;
    wxRadioButton *rb_white;
    wxCheckBox *autosave_enable;
    wxSpinCtrl *autosave_timer;
    wxChoice *units_choice;
    wxChoice *drill_choice;
    wxSpinCtrl *undo_depth;
    wxCheckBox *show_45;
    wxCheckBox *ccoord_show;
    wxCheckBox *ccoord_tp;
    wxCheckBox *ccoord_big;
    wxSpinCtrl *copper_thickness;
    wxSpinCtrl *temp_enhance;
    wxChoice *key_choice;
    wxListView *key_list;
    wxDirPickerCtrl *dirs[5];
    wxCheckBox *same_dirs;
    wxColourPickerCtrl *c_colors[12];
    wxCheckBox *checkboxes[11];


private:
    void GetColorScheme(ColorScheme &c);
    void SetColorScheme(const ColorScheme c);

    void UpdateColorPanel(wxUpdateUIEvent& event);
    void UpdateDirsPanel(wxUpdateUIEvent& event);
    void UpdateCrosshairPanel(wxUpdateUIEvent& event);
    void UpdateAutosavePanel(wxUpdateUIEvent& event);
    static void OpenFileManager(string path);
    void UpdateKeyBindings();

    void OnColorSchemeChoiseSelect(wxCommandEvent& event);
    void OnResetColorClick(wxCommandEvent& event);
    void OnColorSchemeChanged(wxColourPickerEvent& event);
    void OnSameDirClick(wxCommandEvent& event);
    void OnResetMacroClick(wxCommandEvent& event);
    void OnFolderMacroClick(wxCommandEvent& event);
};
