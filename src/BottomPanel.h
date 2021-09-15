#pragma once
#include <wx/panel.h>
#include <wx/event.h>

class BottomPanel : public wxPanel {
public:
	BottomPanel(wxWindow *parent);
	void UpdateColors();
private:
	void UpdateCoords(wxUpdateUIEvent &e);
	void UpdateMultilayer(wxUpdateUIEvent &e);
	void UpdateLayers(wxUpdateUIEvent &e);
	void ShowLayerInfo(wxCommandEvent&);
	void UpdateGround(wxUpdateUIEvent &e);
	void ToggleGround(wxCommandEvent&);
	void UpdateCapture(wxUpdateUIEvent &e);
	void ToggleCapture(wxCommandEvent&);
	void UpdateRubberband(wxUpdateUIEvent &e);
	void ToggleRubberband(wxCommandEvent&);
	wxDECLARE_EVENT_TABLE();
};
