#pragma once
#include "Settings.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <wx/radiobut.h>
#include <wx/spinctrl.h>

class SettingsDialog: public wxDialog {
public:
	SettingsDialog(wxWindow* parent, const Settings &s);

	void Get(Settings &s);

	ColorScheme colors[3];

protected:

	wxChoice *colorSelection;
	wxDirPickerCtrl *macroDir;
	wxRadioButton *rbBlack;
	wxRadioButton *rbWhite;
	wxCheckBox *autosaveEnabled;
	wxSpinCtrl *autosaveTimer;
	wxChoice *unitsChoice;
	wxChoice *drillChoice;
	wxSpinCtrl *undoDepth;
	wxCheckBox *show45;
	wxCheckBox *measureShow;
	wxCheckBox *measureTp;
	wxCheckBox *measureBig;
	wxSpinCtrl *copperThickness;
	wxSpinCtrl *tempEnhance;
	wxChoice *keyList;
	wxListView *toolList;
	wxDirPickerCtrl *dirs[5];
	wxCheckBox *sameDirs;
	wxColourPickerCtrl *colorPicker[12];
	wxCheckBox *checkboxes[11];

private:
	void GetColorScheme(ColorScheme &c);
	void SetColorScheme(const ColorScheme &c);

	void UpdateColorSchemePanel(wxUpdateUIEvent&);
	void UpdateDirs(wxUpdateUIEvent&);

	void SelectColorScheme(wxCommandEvent&);
	void ResetColorScheme(wxCommandEvent&);
	void ChangeColorScheme(wxColourPickerEvent&);
	void ResetMacroDir(wxCommandEvent&);
	void OpenMacroDir(wxCommandEvent&);

	void SelectTool(wxListEvent&);
	void SelectKey(wxCommandEvent&);

	void UpdateCrosshairPanel(wxUpdateUIEvent&);
	void UpdateAutosavePanel(wxUpdateUIEvent&);

	wxDECLARE_EVENT_TABLE();
};
