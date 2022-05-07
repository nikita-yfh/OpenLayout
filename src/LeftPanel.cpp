#include "LeftPanel.h"
#include <wx/sizer.h>
#include <wx/menu.h>

extern "C" {
#include "xpm/leftpanel/autoroute.xpm"
#include "xpm/leftpanel/ch_grid.xpm"
#include "xpm/leftpanel/ch_pad.xpm"
#include "xpm/leftpanel/ch_smd.xpm"
#include "xpm/leftpanel/ch_track.xpm"
#include "xpm/leftpanel/circle.xpm"
#include "xpm/leftpanel/connections.xpm"
#include "xpm/leftpanel/cross.xpm"
#include "xpm/leftpanel/edit.xpm"
#include "xpm/leftpanel/mask.xpm"
#include "xpm/leftpanel/measure.xpm"
#include "xpm/leftpanel/photoview.xpm"
#include "xpm/leftpanel/plus.xpm"
#include "xpm/leftpanel/polygon.xpm"
#include "xpm/leftpanel/rect_track.xpm"
#include "xpm/leftpanel/rect_zone.xpm"
#include "xpm/leftpanel/smd.xpm"
#include "xpm/leftpanel/special.xpm"
#include "xpm/leftpanel/swap_smd.xpm"
#include "xpm/leftpanel/test.xpm"
#include "xpm/leftpanel/text.xpm"
#include "xpm/leftpanel/track.xpm"
#include "xpm/leftpanel/zoom.xpm"

#include "xpm/leftpanel/pad_circle_h.xpm"
#include "xpm/leftpanel/pad_circle_v.xpm"
#include "xpm/leftpanel/pad_circle.xpm"
#include "xpm/leftpanel/pad_octagon_h.xpm"
#include "xpm/leftpanel/pad_octagon_v.xpm"
#include "xpm/leftpanel/pad_octagon.xpm"
#include "xpm/leftpanel/pad_square_h.xpm"
#include "xpm/leftpanel/pad_square_v.xpm"
#include "xpm/leftpanel/pad_square.xpm"

#include "xpm/leftpanel/padm_circle_h.xpm"
#include "xpm/leftpanel/padm_circle_v.xpm"
#include "xpm/leftpanel/padm_circle.xpm"
#include "xpm/leftpanel/padm_octagon_h.xpm"
#include "xpm/leftpanel/padm_octagon_v.xpm"
#include "xpm/leftpanel/padm_octagon.xpm"
#include "xpm/leftpanel/padm_square_h.xpm"
#include "xpm/leftpanel/padm_square_v.xpm"
#include "xpm/leftpanel/padm_square.xpm"
}

static const int toolCount = 16;

struct Tool {
	char **bitmap;
	const char *label;
	const char *tooltip;
};

static const Tool tools[] = {
	{edit_xpm,		_("Edit"), 			_("Edit mode to select. move. copy. cut, paste or delete elements")},
	{zoom_xpm,		_("Zoom"),			_("Zoom mode")},
	{track_xpm,		_("Track"),			_("Draw tracks")},
	{pad_circle_xpm,_("Pad"),			_("Add pads, right click to select a pad shape")},
	{smd_xpm,		_("SMD-Pad"),		_("Add SMD-Pads")},
	{circle_xpm,	_("Circle"),		_("Draw circles or arcs")},
	{rect_track_xpm,_("Rectangle"),		_("Draw rectangles")},
	{polygon_xpm,	_("Zone"),			_("Draw filled areas")},
	{special_xpm,	_("Special form"),	_("Wizard for equilateral polygons, apirals, etc.")},
	{text_xpm,		_("Text"),			_("Add text label")},
	{mask_xpm,		_("Solder mask"),	_("Edit solder mask, include or exclude elements from solder mask")},
	{connections_xpm,_("Connctions"),	_("Draw rubberbands to define connections")},
	{autoroute_xpm,	_("Autoroute"),		_("Autorouter for routing rubberband connections")},
	{test_xpm,		_("Test"),			_("Test mode to check connected elements")},
	{measure_xpm,	_("Measure"),		_("Measure mode to distance and angles")},
	{photoview_xpm,	_("Photoview"),		_("Display the board as WYSIWYG")}
};

static char **padBitmaps[2][9] = {
	{
		pad_circle_xpm,		pad_octagon_xpm,	pad_square_xpm,
		pad_circle_h_xpm,	pad_octagon_h_xpm,	pad_square_h_xpm,
		pad_circle_v_xpm,	pad_octagon_v_xpm,	pad_square_v_xpm,
	},
	{
		padm_circle_xpm,	padm_octagon_xpm,	padm_square_xpm,
		padm_circle_h_xpm,	padm_octagon_h_xpm,	padm_square_h_xpm,
		padm_circle_v_xpm,	padm_octagon_v_xpm,	padm_square_v_xpm,
	}
};

enum {
	ID_TOOL_EDIT = 2000,
	ID_TOOL_ZOOM,
	ID_TOOL_TRACK,
	ID_TOOL_PAD,
	ID_TOOL_SMD,
	ID_TOOL_CIRCLE,
	ID_TOOL_RECT,
	ID_TOOL_ZONE,
	ID_TOOL_FORM,
	ID_TOOL_TEXT,
	ID_TOOL_SOLDER_MASK,
	ID_TOOL_CONNECTIONS,
	ID_TOOL_AUTOROUTE,
	ID_TOOL_TEST,
	ID_TOOL_MEASURE,
	ID_TOOL_PHOTOVIEW,

	ID_PAD_CIRCLE,
	ID_PAD_OCTAGON,
	ID_PAD_SQUARE,
	ID_PAD_CIRCLE_H,
	ID_PAD_OCTAGON_H,
	ID_PAD_SQUARE_H,
	ID_PAD_CIRCLE_V,
	ID_PAD_OCTAGON_V,
	ID_PAD_SQUARE_V,
	ID_METALLIZATION,
	
	ID_RECT_TRACK,
	ID_RECT_ZONE
};

wxBEGIN_EVENT_TABLE(LeftPanel, wxPanel)
	EVT_TOOL_RCLICKED(ID_TOOL_PAD,					LeftPanel::ShowPadMenu)
	EVT_TOOL_RCLICKED(ID_TOOL_RECT,					LeftPanel::ShowRectMenu)
	EVT_MENU(ID_METALLIZATION,						LeftPanel::ToggleMetallization)
	EVT_MENU(ID_METALLIZATION,						LeftPanel::ToggleMetallization)
	EVT_MENU_RANGE(ID_PAD_CIRCLE, ID_PAD_SQUARE_V,	LeftPanel::SetPadShape)
	EVT_MENU_RANGE(ID_RECT_TRACK, ID_RECT_ZONE,		LeftPanel::SetRectFill)
wxEND_EVENT_TABLE()

LeftPanel::LeftPanel(wxWindow *parent, PCB *_pcb) : wxPanel(parent), pcb(_pcb) {
	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	{
		toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
				wxTB_FLAT | wxTB_VERTICAL | wxTB_HORZ_LAYOUT | wxTB_HORZ_TEXT);

		for(int i = 0; i < toolCount; i++)
			toolbar->AddRadioTool(ID_TOOL_EDIT + i, tools[i].label,
				tools[i].bitmap, wxNullBitmap, tools[i].tooltip);

		toolbar->Realize();
		content->Add(toolbar, 1, wxEXPAND);
	}
	SetSizer(content);
	rectFill = false;
}
void LeftPanel::SetRectFill(wxCommandEvent &e) {
	rectFill = e.GetId() - ID_RECT_TRACK;
	toolbar->DeleteTool(ID_TOOL_RECT);
	const Tool &tool = tools[ID_TOOL_RECT - ID_TOOL_EDIT];
	toolbar->InsertTool(ID_TOOL_RECT - ID_TOOL_EDIT, ID_TOOL_RECT, tool.label,
		 rectFill ? rect_zone_xpm : rect_track_xpm, wxNullBitmap, wxITEM_RADIO, tool.tooltip);
	toolbar->Realize();
}

void LeftPanel::SetPadShape(wxCommandEvent &e) {
	uint8_t shape = e.GetId() - ID_PAD_CIRCLE;
	pcb->SetPadShape(shape);
	
	UpdatePad(pcb->GetMetallization(), shape);
}

void LeftPanel::ToggleMetallization(wxCommandEvent&) {
	UpdatePad(pcb->ToggleMetallization(), pcb->GetPadShape());
}
void LeftPanel::UpdateMetallization(wxUpdateUIEvent &e) {
	static bool lastState = pcb->GetMetallization();
	if(lastState != pcb->GetMetallization())
		ToggleMetallization(e);
}
void LeftPanel::UpdatePad(bool metallization, uint8_t shape) {
	toolbar->DeleteTool(ID_TOOL_PAD);
	const Tool &tool = tools[ID_TOOL_PAD - ID_TOOL_EDIT];
	toolbar->InsertTool(ID_TOOL_PAD - ID_TOOL_EDIT, ID_TOOL_PAD, tool.label,
			padBitmaps[metallization][shape], wxNullBitmap, wxITEM_RADIO, tool.tooltip);
	toolbar->Realize();
}

void LeftPanel::ShowPadMenu(wxCommandEvent&) {
	bool m = pcb->GetMetallization();
	wxMenu *menu = new wxMenu();
	AddItem(menu, ID_PAD_CIRCLE,	_("&Circular"),				padBitmaps[m][0]);
	AddItem(menu, ID_PAD_OCTAGON,	_("&Ocragon"),				padBitmaps[m][1]);
	AddItem(menu, ID_PAD_SQUARE,	_("&Square"),				padBitmaps[m][2]);
	menu->AppendSeparator();
	AddItem(menu, ID_PAD_CIRCLE_H,	_("&Rounded, horizontal"),	padBitmaps[m][3]);
	AddItem(menu, ID_PAD_OCTAGON_H,	_("Oct&agon, horizontal"),	padBitmaps[m][4]);
	AddItem(menu, ID_PAD_SQUARE_H,	_("R&ectangle, horizontal"),padBitmaps[m][5]);
	menu->AppendSeparator();
	AddItem(menu, ID_PAD_CIRCLE_V,	_("Ro&unded, vertical"),	padBitmaps[m][6]);
	AddItem(menu, ID_PAD_OCTAGON_V,	_("Octa&gon, vertical"),	padBitmaps[m][7]);
	AddItem(menu, ID_PAD_SQUARE_V,	_("Recta&ngle, vertical"),	padBitmaps[m][8]);
	menu->AppendSeparator();
	menu->AppendCheckItem(ID_METALLIZATION, _("Through pad\tF12"));
	PopupMenu(menu, ID_TOOL_PAD - ID_TOOL_EDIT + 1);
}
void LeftPanel::ShowRectMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	AddItem(menu, ID_RECT_TRACK,	_("&Track"),	rect_track_xpm);
	AddItem(menu, ID_RECT_ZONE,		_("&Zone"),		rect_zone_xpm);
	PopupMenu(menu, ID_TOOL_RECT - ID_TOOL_EDIT + 1);
}

void LeftPanel::PopupMenu(wxMenu *menu, int n) {
	wxWindow::PopupMenu(menu, GetPosition() + wxSize(4, toolbar->GetSize().y / toolCount * n));
}

void LeftPanel::AddItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap) {
	wxMenuItem *item = new wxMenuItem(parent, id, text);
	item->SetBitmap(bitmap);
	parent->Append(item);
}
void LeftPanel::AddCheckItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap) {
	wxMenuItem *item = new wxMenuItem(parent, id, text, wxEmptyString, wxITEM_CHECK);
	item->SetBitmap(bitmap);
	parent->Append(item);
}
