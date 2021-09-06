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
	ID_C1_SHOW,
	ID_S1_SHOW,
	ID_C2_SHOW,
	ID_S2_SHOW,
	ID_I1_SHOW,
	ID_I2_SHOW,
	ID_O_SHOW,
};

class BottomPanel : public wxPanel {
public:
	BottomPanel(wxWindow *parent);
	wxDECLARE_EVENT_TABLE();
private:
	void UpdateCoords(wxUpdateUIEvent &e);
};
