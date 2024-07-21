#pragma once

#include "MacroPreview.h"
#include <wx/treectrl.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/toolbar.h>
#include <wx/splitter.h>

class MacroPanel : public wxSplitterWindow {
public:
	MacroPanel(wxWindow *parent);

private:
	void Save(wxCommandEvent&);
	void Delete(wxCommandEvent&);
	void Rotate(wxCommandEvent&);
	void ToggleMetallization(wxCommandEvent&);
	void ToggleSide(wxCommandEvent&);

	void ScanDir(wxTreeItemId parent, const char *dir);
	wxTreeCtrl *tree;
	wxToolBar *buttons;
	wxCheckBox *component;
	MacroPreview *preview;
	
	bool side;
	bool metallization;
	uint8_t rotate;

	wxDECLARE_EVENT_TABLE();
};
