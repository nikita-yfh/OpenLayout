#pragma once
#include "PCB.h"
#include <wx/panel.h>
#include <wx/spinctrl.h>

class BottomPanel : public wxPanel {
public:
	BottomPanel(wxWindow *parent, PCB &pcb, Settings &settings);
private:

	wxSpinCtrlDouble *groundDistance;
	void UpdatePosition(wxUpdateUIEvent &e);
	void UpdateMultilayer(wxUpdateUIEvent &e);
	void UpdateLayers(wxUpdateUIEvent &e);
	void SetLayer(wxCommandEvent &e);
	void ShowLayerInfo(wxCommandEvent&);
	void UpdateGround(wxUpdateUIEvent &e);
	void UpdateCutout(wxUpdateUIEvent &e);
	void UpdateGroundDistance(wxUpdateUIEvent&);
	void ToggleGround(wxCommandEvent&);
	void ToggleCapture(wxCommandEvent&);
	void ToggleRubberband(wxCommandEvent&);
	void SetGroundDistance(wxSpinDoubleEvent&);
	wxDECLARE_EVENT_TABLE();

	PCB &pcb;
	Settings &settings;
};
