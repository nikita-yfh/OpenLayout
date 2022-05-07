#include "BottomPanel.h"

#include <wx/sizer.h>
#include <wx/radiobut.h>
#include <wx/toolbar.h>
#include <wx/dialog.h>
#include <wx/bmpbuttn.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statline.h>

extern "C" {
#include "xpm/bottompanel/capture_disabled.xpm"
#include "xpm/bottompanel/capture_enabled.xpm"
#include "xpm/bottompanel/ground_disabled.xpm"
#include "xpm/bottompanel/ground_enabled.xpm"
#include "xpm/bottompanel/rubberband_disabled.xpm"
#include "xpm/bottompanel/rubberband_small.xpm"
#include "xpm/bottompanel/rubberband_large.xpm"

#include "xpm/layerinfo/layer_info1.xpm"
#include "xpm/layerinfo/layer_info2.xpm"
#include "xpm/layerinfo/layer_info3.xpm"
#include "xpm/layerinfo/layer_info4.xpm"
#include "xpm/layerinfo/layer_info.xpm"
}

enum {
	ID_POSITION = 1000,

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
	EVT_UPDATE_UI(ID_POSITION,					BottomPanel::UpdatePosition)
	EVT_UPDATE_UI_RANGE(ID_I1,		ID_I2,		BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_I1_TEXT,	ID_I2_TEXT,	BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_C1,		ID_O,		BottomPanel::UpdateLayers)
	EVT_RADIOBUTTON(ID_C1,						BottomPanel::SetLayer)
	EVT_RADIOBUTTON(ID_S1,						BottomPanel::SetLayer)
	EVT_RADIOBUTTON(ID_C2,						BottomPanel::SetLayer)
	EVT_RADIOBUTTON(ID_S2,						BottomPanel::SetLayer)
	EVT_RADIOBUTTON(ID_I1,						BottomPanel::SetLayer)
	EVT_RADIOBUTTON(ID_I2,						BottomPanel::SetLayer)
	EVT_RADIOBUTTON(ID_O,						BottomPanel::SetLayer)
	EVT_BUTTON(ID_HELP,							BottomPanel::ShowLayerInfo)
	EVT_BUTTON(ID_GROUND,						BottomPanel::ToggleGround)
	EVT_BUTTON(ID_CAPTURE,						BottomPanel::ToggleCapture)
	EVT_BUTTON(ID_RUBBERBAND,					BottomPanel::ToggleRubberband)
	EVT_UPDATE_UI(ID_GROUND,					BottomPanel::UpdateGround)
wxEND_EVENT_TABLE()

class LayerInfoDialog : public wxDialog {
public:
	LayerInfoDialog(wxWindow *parent) : wxDialog(parent, wxID_ANY, _("Layer info")) {
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
		char **images[5]= {
			layer_info1_xpm,
			layer_info2_xpm,
			layer_info3_xpm,
			layer_info4_xpm,
			layer_info_xpm
		};
		for(int i = 0; i < 5; i++) {
			wxStaticBitmap *image = new wxStaticBitmap(this, wxID_ANY, wxBitmap(images[i]));
			content->Add(image, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		}
		SetSizerAndFit(content);
	}
};


BottomPanel::BottomPanel(wxWindow *parent, PCB *_pcb)
		: wxPanel(parent), pcb(_pcb) {
	wxBoxSizer *content = new wxBoxSizer(wxHORIZONTAL);
	{
		wxStaticText *position = new wxStaticText(this, ID_POSITION, wxEmptyString);
		content->Add(position, 0, wxALIGN_CENTER | wxALL, 5);
	}
	content->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND);
	{
		wxFlexGridSizer *grid = new wxFlexGridSizer(2, 8, 0, 0);
		const char* colorNames[]= {
			_("C1"),
			_("S1"),
			_("C2"),
			_("S2"),
			_("I1"),
			_("I2"),
			_("O")
		};
		const char* colorHelp[]= {
			_("C1 = Copper - Top"),
			_("S1 = Silkscreen - Top"),
			_("C2 = Copper - Bottom"),
			_("S2 = Silkscreen - Bottom"),
			_("I1 = Inner Layer 1 (multilayer)"),
			_("I2 = Inner Layer 2 (multilayer)"),
			_("O = PCB-Outline")
		};
		grid->Add(new wxStaticText(this, wxID_ANY, _("visible")), 0, wxALIGN_CENTER);
		for(int i = 0; i < 7; i++) {
			wxStaticText *text = new wxStaticText(this, ID_C1_TEXT + i, colorNames[i]);
			text->SetToolTip(colorHelp[i]);
			grid->Add(text, 1, wxALIGN_CENTER);
		}
		grid->Add(new wxStaticText(this, wxID_ANY, _("active")), 0, wxALIGN_CENTER);
		for(int i = 0; i < 7; i++) {
			wxRadioButton *button = new wxRadioButton(this, ID_C1 + i, wxEmptyString);
			button->SetToolTip(colorHelp[i]);
			grid->Add(button, 1, wxALIGN_CENTER);
		}
		content->Add(grid, 0, wxEXPAND | wxALL, 5);
	}
	{
		wxButton *help = new wxButton(this, ID_HELP, "?", wxDefaultPosition, wxSize(20, -1));
		help->SetToolTip(_("Show the layer-colors and their meaning"));
		content->Add(help, 0, wxEXPAND);
	}
	{
		wxBitmapButton *ground = new wxBitmapButton(this, ID_GROUND, ground_disabled_xpm);
		ground->SetToolTip(_("Enable or disable the automatic ground plane for the active copper layer"));
		content->Add(ground, 0, wxEXPAND);
	}
	{
		wxBitmapButton *capture = new wxBitmapButton(this, ID_CAPTURE, capture_enabled_xpm);
		capture->SetToolTip(_("Enable or disable the automatic capture mode"));
		content->Add(capture, 0, wxEXPAND);
	}
	{
		wxBitmapButton *rubberband = new wxBitmapButton(this, ID_RUBBERBAND, rubberband_disabled_xpm);
		rubberband->SetToolTip(_("Toggle the level of Rubberband function (small range / big range / off)"));
		content->Add(rubberband, 0, wxEXPAND);
	}
	SetSizer(content);
}

void BottomPanel::ShowLayerInfo(wxCommandEvent&) {
	LayerInfoDialog(this).ShowModal();
}

static void SetButtonBitmap(wxCommandEvent &e, const wxBitmap &bitmap) {
	static_cast<wxBitmapButton*>(e.GetEventObject())->SetBitmap(bitmap);
}

void BottomPanel::ToggleGround(wxCommandEvent &e) {
	SetButtonBitmap(e, pcb->GetSelectedBoard()->ToggleCurrentLayerGround() ? ground_enabled_xpm : ground_disabled_xpm);
	GetParent()->Refresh();
}

void BottomPanel::UpdateGround(wxUpdateUIEvent &e) {
	uint8_t mode = pcb->GetSelectedBoard()->GetCurrentLayerGround();
	static uint8_t prevMode = mode;
	if(mode != prevMode) {
		prevMode = mode;
		SetButtonBitmap(e, mode ? ground_enabled_xpm : ground_disabled_xpm);
	}
}

void BottomPanel::ToggleCapture(wxCommandEvent &e) {
	SetButtonBitmap(e, pcb->ToggleCaptureMode() ? capture_enabled_xpm : capture_disabled_xpm);
}

void BottomPanel::ToggleRubberband(wxCommandEvent &e) {
	uint8_t mode = pcb->ToggleRubberbandMode();
	SetButtonBitmap(e,	mode == RUBBERBAND_SMALL ? rubberband_large_xpm :
						mode == RUBBERBAND_LARGE ? rubberband_small_xpm :
						rubberband_disabled_xpm);
}

void BottomPanel::UpdateMultilayer(wxUpdateUIEvent &e) {
	e.Show(pcb->GetSelectedBoard()->IsMultilayer());
}

void BottomPanel::UpdatePosition(wxUpdateUIEvent &e) {
	const char *unit = _("mm");
	e.SetText(wxString::Format("X:\t%.3f \t%s\nY:\t%.03f \t%s",
		pcb->GetMousePosition().x, unit,
		pcb->GetMousePosition().y, unit));
}

void BottomPanel::UpdateLayers(wxUpdateUIEvent &e) {
	e.Check(e.GetId() - ID_C1 == pcb->GetSelectedBoard()->GetSelectedLayer());
}

void BottomPanel::SetLayer(wxCommandEvent &e) {
	pcb->GetSelectedBoard()->SetSelectedLayer(e.GetId() - ID_C1);
}

