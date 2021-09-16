#include "BottomPanel.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/toolbar.h>
#include <wx/bmpbuttn.h>
#include "OpenLayoutApp.h"
#include "OpenLayoutMain.h"
#include "LayerInfoDialog.h"

#define BOTTOM_PANEL
#include "images.h"
#undef BOTTOM_PANEL

#define WIDGET(type,e)\
	static_cast<type>(e.GetEventObject())

enum {
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

	ID_HELP,
	ID_GROUND,
	ID_CAPTURE,
	ID_RUBBERBAND
};
wxBEGIN_EVENT_TABLE(BottomPanel, wxPanel)
	EVT_UPDATE_UI(ID_POSX,BottomPanel::UpdateCoords)
	EVT_UPDATE_UI(ID_POSY,BottomPanel::UpdateCoords)
	EVT_UPDATE_UI_RANGE(ID_I1,     ID_I2,     BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_I1_TEXT,ID_I2_TEXT,BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_C1,ID_O,BottomPanel::UpdateLayers)
	EVT_BUTTON(ID_HELP,BottomPanel::ShowLayerInfo)
	EVT_UPDATE_UI	(ID_GROUND,BottomPanel::UpdateGround)
	EVT_BUTTON		(ID_GROUND,BottomPanel::ToggleGround)
	EVT_UPDATE_UI	(ID_CAPTURE,BottomPanel::UpdateCapture)
	EVT_BUTTON		(ID_CAPTURE,BottomPanel::ToggleCapture)
	EVT_UPDATE_UI	(ID_RUBBERBAND,BottomPanel::UpdateRubberband)
	EVT_BUTTON		(ID_RUBBERBAND,BottomPanel::ToggleRubberband)
wxEND_EVENT_TABLE()

static void set_bitmap(wxCommandEvent&e,const wxBitmap &bitmap) {
	static_cast<wxBitmapButton*>(e.GetEventObject())->SetBitmap(bitmap);
}

BottomPanel::BottomPanel(wxWindow *parent):
	wxPanel(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxVSCROLL) {

	wxBoxSizer *all_box=new wxBoxSizer(wxHORIZONTAL);
	{
		wxBoxSizer *coord=new wxBoxSizer(wxVERTICAL);
		for(int q=0; q<2; q++) {
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
		const char* color_names[]= {
			_("C1"),
			_("S1"),
			_("C2"),
			_("S2"),
			_("I1"),
			_("I2"),
			_("O")
		};
		const char* color_help[]= {
			_("C1 = Copper - Top"),
			_("S1 = Silkscreen - Top"),
			_("C2 = Copper - Bottom"),
			_("S2 = Silkscreen - Bottom"),
			_("I1 = Inner Layer 1 (multilayer)"),
			_("I2 = Inner Layer 1 (multilayer)"),
			_("O = PCB-Outline")
		};
		{
			wxPanel *panel=new wxPanel(this);
			panel->SetBackgroundColour({0,0,0});
			{
				wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
				for(int q=0; q<7; q++) {
					wxStaticText *text=new wxStaticText(panel,ID_C1_TEXT+q,color_names[q]);
					text->SetToolTip(color_help[q]);
					sizer->Add(text,1,wxCENTER|wxEXPAND);
				}
				panel->SetSizerAndFit(sizer);
			}

			table->Add(panel,1,wxEXPAND);
		}
		{
			wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
			for(int q=0; q<7; q++) {
				wxRadioButton *button=new wxRadioButton(this,ID_C1+q,"");
				button->SetToolTip(color_help[q]);
				sizer->Add(button,1,wxEXPAND);
			}
			table->Add(sizer,1,wxEXPAND);
			Bind(wxEVT_RADIOBUTTON,[&](wxCommandEvent&e) {
				BOARD.active_layer=e.GetId()-ID_C1+1;
				static_cast<OpenLayoutFrame*>(GetParent())->RefreshCanvas();
			},ID_C1,ID_O);
		}
		all_box->Add(table,0,wxEXPAND);
	}
	{
		wxButton *help_layer=new wxButton(this,ID_HELP,"?",wxDefaultPosition, {20,-1});
		help_layer->SetToolTip(_("Show the layer-colors and their meaning"));
		all_box->Add(help_layer,0,wxEXPAND);
	}
	{
		wxBitmapButton *ground=new wxBitmapButton(this,ID_GROUND,ground_disabled_xpm);
		ground->SetToolTip(_("Enable or disable the automatic ground plane for the active copper layer"));
		all_box->Add(ground,0,wxEXPAND);
	}
	{
		wxBitmapButton *capture=new wxBitmapButton(this,ID_CAPTURE,capture_disabled_xpm);
		capture->SetToolTip(_("Enable or disable the automatic capture mode"));
		all_box->Add(capture,0,wxEXPAND);
	}
	{
		wxBitmapButton *rubberband=new wxBitmapButton(this,ID_RUBBERBAND,rubberband_0_xpm);
		rubberband->SetToolTip(_("Toggle the level of Rubberband function (small range / big range / off)"));
		all_box->Add(rubberband,0,wxEXPAND);
	}
	SetSizerAndFit(all_box);
	SetAutoLayout(true);

	UpdateColors();
}
void BottomPanel::UpdateCoords(wxUpdateUIEvent &e) {
	wxString text;
	if(e.GetId()==ID_POSX)
		text=wxString::Format("X:\t%.3f\t%s",APP.mouse_board_pos.x,_("mm"));
	else
		text=wxString::Format("Y:\t%.3f\t%s",APP.mouse_board_pos.y,_("mm"));
	e.SetText(text);
}
void BottomPanel::UpdateGround(wxUpdateUIEvent &e) {
	e.Enable(BOARD.ground_allow());
	uint8_t &state=BOARD.ground_pane[BOARD.active_layer-1];
	set_bitmap(e,state?ground_enabled_xpm:ground_disabled_xpm);
}
void BottomPanel::ToggleGround(wxCommandEvent &e) {
	uint8_t &state=BOARD.ground_pane[BOARD.active_layer-1];
	state=!state;
	set_bitmap(e,state?ground_enabled_xpm:ground_disabled_xpm);
	static_cast<OpenLayoutFrame*>(GetParent())->RefreshCanvas();
}
void BottomPanel::UpdateCapture(wxUpdateUIEvent &e) {
	set_bitmap(e,APP.capture?capture_enabled_xpm:capture_disabled_xpm);
}
void BottomPanel::ToggleCapture(wxCommandEvent &e) {
	APP.capture=!APP.capture;
	set_bitmap(e,APP.capture?capture_enabled_xpm:capture_disabled_xpm);
}
void BottomPanel::UpdateRubberband(wxUpdateUIEvent &e) {
	set_bitmap(e,APP.rubberband==RUBBERBAND_DISABLED?rubberband_0_xpm:
			   APP.rubberband==RUBBERBAND_SMALL_RANGE?rubberband_1_xpm:
			   rubberband_2_xpm);
}
void BottomPanel::ToggleRubberband(wxCommandEvent &e) {
	APP.rubberband=(APP.rubberband+1)%3;
	set_bitmap(e,APP.rubberband==RUBBERBAND_DISABLED?rubberband_0_xpm:
			   APP.rubberband==RUBBERBAND_SMALL_RANGE?rubberband_1_xpm:
			   rubberband_2_xpm);
}
void BottomPanel::UpdateMultilayer(wxUpdateUIEvent &e) {
	if(static_cast<wxWindow*>(e.GetEventObject())->IsShown()!=BOARD.is_multilayer) {
		static_cast<wxWindow*>(e.GetEventObject())->Show(BOARD.is_multilayer);
		Layout();
	}
}
void BottomPanel::UpdateLayers(wxUpdateUIEvent &e) {
	e.Check(e.GetId()-ID_C1==BOARD.active_layer-1);
}
void BottomPanel::UpdateColors() {
	for(int q=0; q<7; q++) {
		wxStaticText *text=static_cast<wxStaticText*>(FindWindowById(ID_C1_TEXT+q));
		text->SetForegroundColour(SETTINGS.get_color(q));
	}
}
void BottomPanel::ShowLayerInfo(wxCommandEvent&) {
	LayerInfoDialog dialog(this);
	dialog.ShowModal();
}
