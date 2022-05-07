#pragma once
#include "PCB.h"
#include <wx/panel.h>

class BottomPanel : public wxPanel {
public:
	BottomPanel(wxWindow *parent, PCB *pcb);
private:
	void UpdatePosition(wxUpdateUIEvent &e);
	void UpdateMultilayer(wxUpdateUIEvent &e);
	void UpdateLayers(wxUpdateUIEvent &e);
	void SetLayer(wxCommandEvent &e);
	void ShowLayerInfo(wxCommandEvent&);
	void UpdateGround(wxUpdateUIEvent &e);
	void ToggleGround(wxCommandEvent&);
	void ToggleCapture(wxCommandEvent&);
	void ToggleRubberband(wxCommandEvent&);
	wxDECLARE_EVENT_TABLE();

	PCB *pcb;
};
