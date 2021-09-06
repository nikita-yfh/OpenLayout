#include "BottomPanel.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "OpenLayoutApp.h"

wxBEGIN_EVENT_TABLE(BottomPanel, wxPanel)
	EVT_UPDATE_UI(ID_POSX,BottomPanel::UpdateCoords)
	EVT_UPDATE_UI(ID_POSY,BottomPanel::UpdateCoords)
wxEND_EVENT_TABLE()

BottomPanel::BottomPanel(wxWindow *parent):
	wxPanel(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxVSCROLL) {

	wxBoxSizer *all_box=new wxBoxSizer(wxHORIZONTAL);
	{
		wxBoxSizer *coord=new wxBoxSizer(wxVERTICAL);
		for(int q=0;q<2;q++){
			wxStaticText *text=new wxStaticText(this,ID_POSX+q,"sometext");
			coord->Add(text,0,wxEXPAND);
		}
		all_box->Add(coord,0,wxEXPAND|wxALL,5);
	}
	SetSizerAndFit(all_box);
}
void BottomPanel::UpdateCoords(wxUpdateUIEvent &e){
	wxString text;
	if(e.GetId()==ID_POSX)
		text=wxString::Format("X:\t%.3f\t%s",APP.mouse_board_pos.x,_("mm"));
	else
		text=wxString::Format("Y:\t%.3f\t%s",APP.mouse_board_pos.y,_("mm"));

	static_cast<wxStaticText*>(e.GetEventObject())->SetLabel(text);
}
