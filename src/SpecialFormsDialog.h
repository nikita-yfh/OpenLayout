#pragma once

#include "ObjectGroup.h"
#include "Settings.h"
#include <wx/dialog.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/notebook.h>

class SpecialFormsDialog : public wxDialog {
public:
	SpecialFormsDialog(wxWindow* parent, const Settings &settings, const Vec2 &boardSize, uint8_t layer);
private:
	void AutosizeFrame(wxCommandEvent&);
	void UpdatePreview(wxSpinDoubleEvent&);
	void UpdatePreview(wxSpinEvent&);
	void UpdatePreview(wxBookCtrlEvent&);
	void UpdatePreview(wxCommandEvent&);
	void UpdatePreview();

	wxGLCanvas *canvas;
	Vec2 boardSize;
	uint8_t layer;
	ObjectGroup objects;

	wxDECLARE_EVENT_TABLE();
};

