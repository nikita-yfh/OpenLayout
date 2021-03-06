#include "BottomPanel.h"
#include "OpenLayoutMain.h"
#include "LayerCheckBox.h"

#include <wx/sizer.h>
#include <wx/radiobut.h>
#include <wx/toolbar.h>
#include <wx/dialog.h>
#include <wx/bmpbuttn.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statline.h>

extern "C" {
#include "xpm/bottompanel/cutout_rect.xpm"
#include "xpm/bottompanel/cutout_polygon.xpm"
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
	ID_C1_SHOW,
	ID_S1_SHOW,
	ID_C2_SHOW,
	ID_S2_SHOW,
	ID_I1_SHOW,
	ID_I2_SHOW,
	ID_O_SHOW,

	ID_HELP,
	ID_GROUND,
	ID_CAPTURE,
	ID_RUBBERBAND,

	ID_GROUND_DISTANCE,
	ID_CUTOUT_RECT,
	ID_CUTOUT_POLYGON
};

#define EVT_RADIOBUTTON_RANGE(id1, id2, func) wx__DECLARE_EVT2(wxEVT_RADIOBUTTON, id1, id2, wxCommandEventHandler(func))
#define EVT_CHECKBOX_RANGE(id1, id2, func) wx__DECLARE_EVT2(wxEVT_CHECKBOX, id1, id2, wxCommandEventHandler(func))

wxBEGIN_EVENT_TABLE(BottomPanel, wxPanel)
	EVT_UPDATE_UI(ID_POSITION,					BottomPanel::UpdatePosition)
	EVT_UPDATE_UI_RANGE(ID_I1,		ID_I2,		BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_I1_SHOW,	ID_I2_SHOW,	BottomPanel::UpdateMultilayer)
	EVT_UPDATE_UI_RANGE(ID_C1,		ID_O,		BottomPanel::UpdateLayers)
	EVT_UPDATE_UI_RANGE(ID_C1_SHOW,	ID_O_SHOW,	BottomPanel::UpdateLayersVisible)
	EVT_RADIOBUTTON_RANGE(ID_C1,	ID_O,		BottomPanel::SetLayer)
	EVT_CHECKBOX_RANGE(ID_C1_SHOW,	ID_O_SHOW,	BottomPanel::SetLayerVisible)
	EVT_BUTTON(ID_HELP,							BottomPanel::ShowLayerInfo)
	EVT_BUTTON(ID_GROUND,						BottomPanel::ToggleGround)
	EVT_BUTTON(ID_CAPTURE,						BottomPanel::ToggleCapture)
	EVT_BUTTON(ID_RUBBERBAND,					BottomPanel::ToggleRubberband)
	EVT_SPINCTRLDOUBLE(ID_GROUND_DISTANCE,		BottomPanel::SetGroundDistance)
	EVT_UPDATE_UI(ID_GROUND,					BottomPanel::UpdateGround)
	EVT_UPDATE_UI(ID_GROUND_DISTANCE,			BottomPanel::UpdateGroundDistance)
	EVT_UPDATE_UI(ID_CUTOUT_RECT,				BottomPanel::UpdateCutout)
	EVT_UPDATE_UI(ID_CUTOUT_POLYGON,			BottomPanel::UpdateCutout)
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


BottomPanel::BottomPanel(wxWindow *parent, PCB &_pcb, Settings &_settings)
		: wxPanel(parent), pcb(_pcb), settings(_settings) {
	wxBoxSizer *content = new wxBoxSizer(wxHORIZONTAL);
	{
		wxStaticText *position = new wxStaticText(this, ID_POSITION, wxEmptyString, wxDefaultPosition, wxSize(100, -1));
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
		grid->Add(new wxStaticText(this, wxID_ANY, _("visible")), 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
		for(int i = 0; i < 7; i++) {
			LayerCheckBox *checkbox = new LayerCheckBox(this, ID_C1_SHOW + i, colorNames[i]);
			checkbox->SetForegroundColour(settings.GetColorScheme()[COLOR_C1 + i]);
			checkbox->SetToolTip(colorHelp[i]);
			grid->Add(checkbox, 1, wxEXPAND);
		}
		grid->Add(new wxStaticText(this, wxID_ANY, _("active")), 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 10);
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
		wxBoxSizer *vsizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);

		groundDistance = new wxSpinCtrlDouble(this, ID_GROUND_DISTANCE, wxEmptyString,
				wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0.0, 10.0, 0.4, 0.1);

		wxBitmapButton *cutoutRect = new wxBitmapButton(this, ID_CUTOUT_RECT, cutout_rect_xpm);
		wxBitmapButton *cutoutPolygon = new wxBitmapButton(this, ID_CUTOUT_POLYGON, cutout_polygon_xpm);

		cutoutRect->SetToolTip(_("Generate rectangle-cutout-areas in the ground plane"));
		cutoutPolygon->SetToolTip(_("Generate polygon-cutout-areas in the ground plane"));

		hsizer->Add(cutoutRect, 1, wxEXPAND);
		hsizer->Add(cutoutPolygon, 1, wxEXPAND);
		vsizer->Add(groundDistance, 0, wxEXPAND);
		vsizer->Add(hsizer, 0, wxEXPAND);
		content->Add(vsizer, 0, wxEXPAND);
	}
	{
		wxBitmapButton *capture = new wxBitmapButton(this, ID_CAPTURE,
			settings.capture ? capture_enabled_xpm : capture_disabled_xpm);
		capture->SetToolTip(_("Enable or disable the automatic capture mode"));
		content->Add(capture, 0, wxEXPAND);
	}
	{
		wxBitmapButton *rubberband = new wxBitmapButton(this, ID_RUBBERBAND,
			settings.rubberband == RUBBERBAND_LARGE ? rubberband_large_xpm :
			settings.rubberband == RUBBERBAND_SMALL ? rubberband_small_xpm :
			rubberband_disabled_xpm);
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
	pcb.GetSelectedBoard()->ToggleCurrentLayerGround();
	GetParent()->Refresh();
}

void BottomPanel::UpdateGround(wxUpdateUIEvent &e) {
	uint8_t mode = pcb.GetSelectedBoard()->GetCurrentLayerGround();
	static uint8_t prevMode = !mode;
	if(mode != prevMode) {
		prevMode = mode;
		SetButtonBitmap(e, mode ? ground_enabled_xpm : ground_disabled_xpm);
	}
	e.Enable(pcb.GetSelectedBoard()->IsSelectedLayerCopper());
}

void BottomPanel::ToggleCapture(wxCommandEvent &e) {
	settings.capture = !settings.capture;
	SetButtonBitmap(e, settings.capture ? capture_enabled_xpm : capture_disabled_xpm);
}

void BottomPanel::ToggleRubberband(wxCommandEvent &e) {
	settings.rubberband = (settings.rubberband + 1) % 3;
	SetButtonBitmap(e,	settings.rubberband == RUBBERBAND_LARGE ? rubberband_large_xpm :
						settings.rubberband == RUBBERBAND_SMALL ? rubberband_small_xpm :
						rubberband_disabled_xpm);
}

void BottomPanel::UpdateMultilayer(wxUpdateUIEvent &e) {
	e.Show(pcb.GetSelectedBoard()->IsMultilayer());
}

void BottomPanel::UpdatePosition(wxUpdateUIEvent &e) {
	const char *unit = _("mm");
	const Vec2 &mouse = ((OpenLayoutFrame*) GetParent())->GetCanvas()->GetMousePosition();
	e.SetText(wxString::Format("X:\t%.3f   %s\nY:\t%.03f   %s", mouse.x, unit, mouse.y, unit));
}

void BottomPanel::UpdateLayers(wxUpdateUIEvent &e) {
	e.Check(e.GetId() - ID_C1 == pcb.GetSelectedBoard()->GetSelectedLayer());
}

void BottomPanel::UpdateLayersVisible(wxUpdateUIEvent &e) {
	e.Check(pcb.GetSelectedBoard()->IsLayerVisible(e.GetId() - ID_C1_SHOW));
	e.Enable(pcb.GetSelectedBoard()->GetSelectedLayer() != e.GetId() - ID_C1_SHOW);
}

void BottomPanel::SetLayer(wxCommandEvent &e) {
	pcb.GetSelectedBoard()->SetSelectedLayer(e.GetId() - ID_C1);
	pcb.GetSelectedBoard()->SetLayerVisible(e.GetId() - ID_C1, true);
	GetParent()->Refresh();
}

void BottomPanel::SetLayerVisible(wxCommandEvent &e) {
	pcb.GetSelectedBoard()->SetLayerVisible(e.GetId() - ID_C1_SHOW, e.GetInt());
	GetParent()->Refresh();
}

void BottomPanel::SetGroundDistance(wxSpinDoubleEvent &e) {
	settings.groundDistance = e.GetValue();
	for(Object *object = pcb.GetSelectedBoard()->GetObjects(); object; object = object->GetNext())
		if(object->IsSelected())
			object->SetGroundDistance(settings.groundDistance);
	GetParent()->Refresh();
}

void BottomPanel::UpdateGroundDistance(wxUpdateUIEvent &e) {
	Object *selected = pcb.GetSelectedBoard()->GetFirstSelected();
	if(selected)
		groundDistance->SetValue(selected->GetGroundDistance());
	else
		groundDistance->SetValue(settings.groundDistance);
	UpdateCutout(e);
}

void BottomPanel::UpdateCutout(wxUpdateUIEvent &e) {
	e.Enable(pcb.GetSelectedBoard()->GetCurrentLayerGround());
}


