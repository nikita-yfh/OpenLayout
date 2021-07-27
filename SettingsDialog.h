#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H
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
    static const long ID_PANEL_COLORS;
    static const long ID_PANEL_DIRS;
    static const long ID_PANEL_CROSSHAIR;
    static const long ID_PANEL_AUTOSAVE;
    static const long ID_COLORSELECTION;
    static const long ID_MACRO_DIR;
    static const long ID_RB_BLACK;
    static const long ID_RB_WHITE;
    static const long ID_TEST_PANEL;
    static const long ID_TEST_TEXT1;
    static const long ID_TEST_TEXT2;
    static const long ID_AUTOSAVE_CHECK;
    static const long ID_AUTOSAVE_TIMER;
    static const long ID_UNITS_CHOISE;
    static const long ID_DRILL_CHOISE;
    static const long ID_UNDO_DEPTH;
    static const long ID_SHOW_45;
    static const long ID_CCOORD_SHOW;
    static const long ID_CCOORD_TP;
    static const long ID_CCOORD_BIG;
    static const long ID_COPPER_THICKNESS;
    static const long ID_TEMP_ENHANCE;
    static const long ID_KEY_CHOICE;
    static const long ID_KEY_LIST;
    static const long ID_DIRS[5];
    static const long ID_SAME_DIRS;
    static const long ID_COLORS[12];
    static const long ID_CHECKBOXES[11];

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

#endif
