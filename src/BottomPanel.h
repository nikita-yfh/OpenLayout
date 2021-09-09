#pragma once
#include <wx/panel.h>
#include <wx/event.h>

class BottomPanel : public wxPanel {
public:
	BottomPanel(wxWindow *parent);
	void UpdateColors();
	wxDECLARE_EVENT_TABLE();
private:
	void UpdateCoords(wxUpdateUIEvent &e);
	void UpdateMultilayer(wxUpdateUIEvent &e);
	void UpdateLayers(wxUpdateUIEvent &e);
	void ShowLayerInfo(wxCommandEvent&);
};
