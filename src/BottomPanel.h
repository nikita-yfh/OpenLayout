#pragma once
#include <wx/panel.h>
#include <wx/event.h>

enum{
	ID_POSX=1,
	ID_POSY,

	ID_C1,
	ID_S1,
	ID_C2,
	ID_S2,
	ID_I1,
	ID_I2,
	ID_O,
	ID_C1_TEXT,
	ID_S1_TEXT,
	ID_C2_TEXT,
	ID_S2_TEXT,
	ID_I1_TEXT,
	ID_I2_TEXT,
	ID_O_TEXT,
};

class BottomPanel : public wxPanel {
public:
	BottomPanel(wxWindow *parent);
	wxDECLARE_EVENT_TABLE();
private:
	void UpdateCoords(wxUpdateUIEvent &e);
	void UpdateMultilayer(wxUpdateUIEvent &e);
	void UpdateLayers(wxUpdateUIEvent &e);
};
