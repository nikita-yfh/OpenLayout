#include "BottomPanel.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/toolbar.h>
#include "OpenLayoutApp.h"

wxBEGIN_EVENT_TABLE(BottomPanel, wxPanel)
	EVT_UPDATE_UI(ID_POSX,BottomPanel::UpdateCoords)
	EVT_UPDATE_UI(ID_POSY,BottomPanel::UpdateCoords)
	EVT_UPDATE_UI_RANGE(ID_I1,     ID_I2,     BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_I1_TEXT,ID_I2_TEXT,BottomPanel::UpdateMultilayer)
wxEND_EVENT_TABLE()

BottomPanel::BottomPanel(wxWindow *parent):
	wxPanel(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxVSCROLL) {

	wxBoxSizer *all_box=new wxBoxSizer(wxHORIZONTAL);
	{
		wxBoxSizer *coord=new wxBoxSizer(wxVERTICAL);
		for(int q=0;q<2;q++){
			wxStaticText *text=new wxStaticText(this,ID_POSX+q,"sometext");
			coord->Add(text,1,wxEXPAND);
		}
		all_box->Add(coord,0,wxEXPAND|wxRIGHT|wxLEFT,20);
	}
	{
		wxBoxSizer *sizer=new wxBoxSizer(wxVERTICAL);
		sizer->Add(new wxStaticText(this,wxID_ANY,_("visible")),1,wxCENTER);
		sizer->Add(new wxStaticText(this,wxID_ANY,_("active")),1,wxCENTER);
		all_box->Add(sizer,0,wxEXPAND|wxRIGHT|wxLEFT,5);

	}
	{
		wxBoxSizer *table=new wxBoxSizer(wxVERTICAL);
		{
			wxPanel *panel=new wxPanel(this);
			panel->SetBackgroundColour({0,0,0});
			{
				wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
				const char* color_names[]= {
					_("C1"),
					_("S1"),
					_("C2"),
					_("S2"),
					_("I1"),
					_("I2"),
					_("O")
				};
				for(int q=0;q<7;q++){
					wxStaticText *text=new wxStaticText(panel,ID_C1_TEXT+q,color_names[q]);
					text->SetForegroundColour(SETTINGS.get_color(q));
					sizer->Add(text,1,wxCENTER|wxEXPAND);
				}
				panel->SetSizerAndFit(sizer);
			}

			table->Add(panel,1,wxEXPAND);
		}
		{
			wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
			for(int q=0;q<7;q++){
				wxRadioButton *button=new wxRadioButton(this,ID_C1+q,"");
				sizer->Add(button,1,wxEXPAND);
			}
			table->Add(sizer,1,wxEXPAND);
			Bind(wxEVT_RADIOBUTTON,[&](wxCommandEvent&e){
				BOARD.active_layer=e.GetId()-ID_C1+1;
			},ID_C1,ID_O);
		}
		all_box->Add(table,0,wxEXPAND);
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
void BottomPanel::UpdateMultilayer(wxUpdateUIEvent &e){
	e.Show(BOARD.is_multilayer);
}
