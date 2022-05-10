#include "LeftPanel.h"
#include "Settings.h"
#include "InputGridDialog.h"
#include "GridBinderDialog.h"
#include "Track.h"
#include "SMDPad.h"
#include "THTPad.h"
#include "Poly.h"
#include "Arc.h"
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/bmpbuttn.h>

extern "C" {
#include "xpm/leftpanel/autoroute.xpm"
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

#include "xpm/leftpanel/ch_grid.xpm"
#include "xpm/leftpanel/ch_pad.xpm"
#include "xpm/leftpanel/ch_smd.xpm"
#include "xpm/leftpanel/ch_track.xpm"
#include "xpm/leftpanel/swap_smd.xpm"
}

static const int normalGridsCount = 8;
static const double normalGrids[] = {
	0.0396875, 0.079375, 0.15875,
	0.3175, 0.635, 1.27, 2.54, 5.08
};

static const int metricGridsCount = 11;
static const double metricGrids[] = {
	0.01,	0.02,	0.025,	0.05,
	0.1,	0.2,	0.25,	0.5,
	1.0,	2.0,	2.5
};

static const int toolCount = 16;

struct Tool {
	char **bitmap;
	const char *label;
	const char *tooltip;
};

static const Tool tools[] = {
	{edit_xpm,		_("Edit"), 			_("Edit mode to select, move, copy, cut, paste or delete elements")},
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
	{connections_xpm,_("Connections"),	_("Draw rubberbands to define connections")},
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
	ID_TOOL_ARC,
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
	ID_RECT_ZONE,
	
	ID_GRID_MENU,
	ID_TRACK_MENU,
	ID_PAD_MENU,
	ID_SMD_MENU,

	ID_TRACK_SIZE,
	ID_TRACK_SEL,
	ID_TRACK_ADD = ID_TRACK_SEL + 100,
	ID_TRACK_DEL,

	ID_PAD_OUTER = ID_TRACK_DEL + 100,
	ID_PAD_INNER,
	ID_PAD_SEL,
	ID_PAD_ADD = ID_PAD_SEL + 100,
	ID_PAD_DEL,

	ID_SMD_WIDTH = ID_PAD_DEL + 100,
	ID_SMD_HEIGHT,
	ID_SMD_SEL,
	ID_SMD_ADD = ID_SMD_SEL + 100,
	ID_SMD_DEL,
	ID_SMD_SWAP = ID_SMD_DEL + 100,

	ID_GRID_NORMAL,
	ID_GRID_METRIC = ID_GRID_NORMAL + 8,
	ID_GRID_USER = ID_GRID_METRIC + 11,
	ID_GRID_USER_DEL = ID_GRID_USER + 100,
	ID_GRID_USER_NEW = ID_GRID_USER_DEL + 100,

	ID_GRID_HOTKEYS,

	ID_GRID_LINES,
	ID_GRID_DOTS,

	ID_SUBGRID_OFF,
	ID_SUBGRID_2,
	ID_SUBGRID_4,
	ID_SUBGRID_5,
	ID_SUBGRID_10,

	ID_GRID_SHOW,
};

wxBEGIN_EVENT_TABLE(LeftPanel, wxPanel)
	EVT_TOOL_RCLICKED(ID_TOOL_PAD,							LeftPanel::ShowPadSizeMenu)
	EVT_TOOL_RCLICKED(ID_TOOL_RECT,							LeftPanel::ShowRectFillMenu)
	EVT_MENU(ID_METALLIZATION,								LeftPanel::ToggleMetallization)
	EVT_MENU(ID_METALLIZATION,								LeftPanel::ToggleMetallization)
	EVT_MENU_RANGE(ID_PAD_CIRCLE, ID_PAD_SQUARE_V,			LeftPanel::SetPadShape)
	EVT_MENU_RANGE(ID_RECT_TRACK, ID_RECT_ZONE,				LeftPanel::SetRectFill)
	EVT_BUTTON(ID_GRID_MENU,								LeftPanel::ShowGridMenu)
	EVT_BUTTON(ID_TRACK_MENU,								LeftPanel::ShowTrackMenu)
	EVT_BUTTON(ID_PAD_MENU,									LeftPanel::ShowPadMenu)
	EVT_BUTTON(ID_SMD_MENU,									LeftPanel::ShowSmdMenu)
	EVT_UPDATE_UI(ID_GRID_MENU,								LeftPanel::UpdateGrid)
	EVT_MENU(ID_GRID_USER_NEW,								LeftPanel::AddNewGrid)
	EVT_MENU_RANGE(ID_GRID_NORMAL, ID_GRID_NORMAL + 118,	LeftPanel::SelectGrid)
	EVT_MENU_RANGE(ID_GRID_USER_DEL, ID_GRID_USER_DEL + 99,	LeftPanel::RemoveGrid)
	EVT_MENU_RANGE(ID_SUBGRID_OFF, ID_SUBGRID_10,			LeftPanel::SetSubgrid)
	EVT_MENU_RANGE(ID_GRID_LINES, ID_GRID_DOTS,				LeftPanel::SetGridStyle)
	EVT_MENU(ID_GRID_SHOW,									LeftPanel::ToggleGrid)
	EVT_MENU(ID_GRID_HOTKEYS,								LeftPanel::ShowGridBinder)
	EVT_UPDATE_UI(wxID_ANY,									LeftPanel::UpdateSizes)

	EVT_SPINCTRLDOUBLE(ID_TRACK_SIZE,						LeftPanel::SetTrackSize)
	EVT_MENU(ID_TRACK_ADD,									LeftPanel::AddTrackSize)
	EVT_MENU_RANGE(ID_TRACK_SEL, ID_TRACK_SEL + 99,			LeftPanel::SelectTrackSize)
	EVT_MENU_RANGE(ID_TRACK_DEL, ID_TRACK_DEL + 99,			LeftPanel::RemoveTrackSize)

	EVT_SPINCTRLDOUBLE(ID_PAD_OUTER,						LeftPanel::SetPadSize)
	EVT_SPINCTRLDOUBLE(ID_PAD_INNER,						LeftPanel::SetPadSize)
	EVT_MENU(ID_PAD_ADD,									LeftPanel::AddPadSize)
	EVT_MENU_RANGE(ID_PAD_SEL, ID_PAD_SEL + 99, 			LeftPanel::SelectPadSize)
	EVT_MENU_RANGE(ID_PAD_DEL, ID_PAD_DEL + 99, 			LeftPanel::RemovePadSize)

	EVT_SPINCTRLDOUBLE(ID_SMD_WIDTH,						LeftPanel::SetSmdSize)
	EVT_SPINCTRLDOUBLE(ID_SMD_HEIGHT,						LeftPanel::SetSmdSize)
	EVT_BUTTON(ID_SMD_SWAP,									LeftPanel::SwapSmdSize)
	EVT_MENU(ID_SMD_ADD,									LeftPanel::AddSmdSize)
	EVT_MENU_RANGE(ID_SMD_SEL, ID_SMD_SEL + 99, 			LeftPanel::SelectSmdSize)
	EVT_MENU_RANGE(ID_SMD_DEL, ID_SMD_DEL + 99, 			LeftPanel::RemoveSmdSize)
wxEND_EVENT_TABLE()

LeftPanel::LeftPanel(wxWindow *parent, PCB *_pcb, Settings *_settings)
		: wxPanel(parent), pcb(_pcb), settings(_settings) {
	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	{
		toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
				wxTB_FLAT | wxTB_VERTICAL | wxTB_HORZ_LAYOUT | wxTB_HORZ_TEXT);

		for(int i = 0; i < toolCount; i++)
			toolbar->AddRadioTool(ID_TOOL_EDIT + i, tools[i].label,
				tools[i].bitmap, wxNullBitmap, tools[i].tooltip);

		toolbar->Realize();
		content->Add(toolbar, 0, wxEXPAND);
	}
	/* content->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND); */
	{
		wxButton *gridButton = new wxButton(this, ID_GRID_MENU, "1.27 mm",
			wxDefaultPosition, wxDefaultSize, wxBU_LEFT | wxBORDER_NONE);
		gridButton->SetBitmap(ch_grid_xpm, wxLEFT);
		gridButton->SetToolTip(_("Snap to grid"));
		content->Add(gridButton, 0, wxEXPAND | wxALL, 5);
	}
	content->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND);
	{
		wxFlexGridSizer *sizer = new wxFlexGridSizer(3, 2, 5, 10);
		wxSize size(70, -1);
		{
			wxBitmapButton *track = new wxBitmapButton(this, ID_TRACK_MENU, ch_track_xpm);
			track->SetToolTip(_("Favorite list for Tracks"));
			sizer->Add(track, 0, wxEXPAND);
			trackSize = new wxSpinCtrlDouble(this, ID_TRACK_SIZE,
				wxEmptyString, wxDefaultPosition, size, wxSP_ARROW_KEYS, 0.0, 99.99, 0.0, 0.05);
			trackSize->SetToolTip(_("Track width"));
			sizer->Add(trackSize, 0, wxEXPAND);
		}
		{
			wxBitmapButton *pad = new wxBitmapButton(this, ID_PAD_MENU, ch_pad_xpm);
			pad->SetToolTip(_("Favorite list for Tracks"));
			sizer->Add(pad, 0, wxEXPAND);
			{
				wxBoxSizer *padSize = new wxBoxSizer(wxVERTICAL);
				padSizeOuter = new wxSpinCtrlDouble(this, ID_PAD_OUTER, wxEmptyString,
					wxDefaultPosition, size, wxSP_ARROW_KEYS, 0.05, 99.99, 0.0, 0.05);
				padSizeInner = new wxSpinCtrlDouble(this, ID_PAD_INNER, wxEmptyString,
					wxDefaultPosition, size, wxSP_ARROW_KEYS, 0.0, 99.99, 0.0, 0.05);
				padSizeOuter->SetToolTip(_("Pad outside diameter"));
				padSizeInner->SetToolTip(_("Pad drilling diameter"));

				padSize->Add(padSizeOuter, 0, wxEXPAND);
				padSize->Add(padSizeInner, 0, wxEXPAND);
				sizer->Add(padSize);
			}
		}
		{
			wxBitmapButton *smd = new wxBitmapButton(this, ID_SMD_MENU, ch_smd_xpm);
			smd->SetToolTip(_("Favorite list for Tracks"));
			sizer->Add(smd, 0, wxEXPAND);
			{
				wxBoxSizer *smdSize = new wxBoxSizer(wxVERTICAL);
				smdWidth = new wxSpinCtrlDouble(this, ID_SMD_WIDTH, wxEmptyString,
					wxDefaultPosition, size, wxSP_ARROW_KEYS, 0.05, 99.99, 0.0, 0.05);
				smdHeight = new wxSpinCtrlDouble(this, ID_SMD_HEIGHT, wxEmptyString,
					wxDefaultPosition, size, wxSP_ARROW_KEYS, 0.05, 99.99, 0.0, 0.05);
				smdWidth->SetToolTip(_("SMD-Pad size"));
				smdHeight->SetToolTip(_("SMD-Pad size"));
				wxBitmapButton *smdSwap = new wxBitmapButton(this, ID_SMD_SWAP, swap_smd_xpm);
				smdSwap->SetToolTip(_("Swap smd sizes"));

				smdSize->Add(smdWidth,	0, wxEXPAND);
				smdSize->Add(smdHeight,	0, wxEXPAND);
				smdSize->Add(smdSwap,	0, wxEXPAND);
				sizer->Add(smdSize);
			}
		}
		content->Add(sizer, 0, wxEXPAND | wxALL, 5);
	}
	SetSizer(content);
	rectFill = false;
}

void LeftPanel::PopupToolbarMenu(wxMenu *menu, int n) {
	PopupMenu(menu, GetPosition() + wxSize(4, toolbar->GetSize().y / toolCount * n));
}

void LeftPanel::AddSubmenu(wxMenu *parent, wxMenu *child, const char *text, const wxBitmap &bitmap, bool enabled) {
	wxMenuItem *item = new wxMenuItem(parent, wxID_ANY, text);
	item->SetBitmap(bitmap);
	item->SetSubMenu(child);
	parent->Append(item);
	item->Enable(enabled);
}

void LeftPanel::AddItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap, bool enabled) {
	wxMenuItem *item = new wxMenuItem(parent, id, text);
	item->SetBitmap(bitmap);
	parent->Append(item);
	item->Enable(enabled);
}

void LeftPanel::AddCheckItem(wxMenu *parent, int id, const char *text, const wxBitmap &bitmap, bool enabled) {
	wxMenuItem *item = new wxMenuItem(parent, id, text, wxEmptyString, wxITEM_CHECK);
	item->SetBitmap(bitmap);
	parent->Append(item);
	item->Enable(enabled);
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

void LeftPanel::ShowPadSizeMenu(wxCommandEvent&) {
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
	PopupToolbarMenu(menu, ID_TOOL_PAD - ID_TOOL_EDIT + 1);
}

void LeftPanel::ShowRectFillMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	AddItem(menu, ID_RECT_TRACK,	_("&Track"),	rect_track_xpm);
	AddItem(menu, ID_RECT_ZONE,		_("&Zone"),		rect_zone_xpm);
	PopupToolbarMenu(menu, ID_TOOL_RECT - ID_TOOL_EDIT + 1);
}


void LeftPanel::AddNewGrid(wxCommandEvent&) {
	double grid = InputGridDialog::Show(this, settings, 1.0);
	if(grid == 0.0)
		return;
	settings->grids.Add(grid);
	pcb->GetSelectedBoard()->SetGrid(grid);
}

void LeftPanel::RemoveGrid(wxCommandEvent &e) {
	int index = e.GetId() - ID_GRID_USER_DEL;
	settings->grids.RemoveIndex(index);
}

void LeftPanel::SelectGrid(wxCommandEvent &e) {
	int id = e.GetId();
	double grid;
	if(id < ID_GRID_METRIC)
		grid = normalGrids[id - ID_GRID_NORMAL];
	else if(id < ID_GRID_USER)
		grid = metricGrids[id - ID_GRID_METRIC];
	else //user grid
		grid = settings->grids[id - ID_GRID_USER];
	pcb->GetSelectedBoard()->SetGrid(grid);
}

void LeftPanel::UpdateGrid(wxUpdateUIEvent &e) {
	e.SetText(settings->GetGridStr(pcb->GetSelectedBoard()->GetGrid()));
}

void LeftPanel::SetSubgrid(wxCommandEvent &e) {
	settings->subGrid = e.GetId() - ID_SUBGRID_OFF;
}

void LeftPanel::SetGridStyle(wxCommandEvent &e) {
	settings->gridStyle = e.GetId() - ID_GRID_LINES;
}

void LeftPanel::ToggleGrid(wxCommandEvent &e) {
	settings->showGrid = e.IsChecked();
}

void LeftPanel::ShowGridBinder(wxCommandEvent&) {
	GridBinderDialog::Show(this, settings);
}

void LeftPanel::ShowGridMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();

	for(int i = 0; i < normalGridsCount; i++) 
		menu->AppendCheckItem(ID_GRID_NORMAL + i, settings->GetGridStr(normalGrids[i]))
				->Check(pcb->GetSelectedBoard()->GetGrid() == normalGrids[i]);

	menu->AppendSeparator();
	{
		wxMenu *metric = new wxMenu();
		for(int i = 0; i < metricGridsCount; i++) 
			metric->AppendCheckItem(ID_GRID_METRIC + i, settings->GetGridStr(metricGrids[i]))
					->Check(pcb->GetSelectedBoard()->GetGrid() == metricGrids[i]);
		menu->Append(wxID_ANY, _("Metric grids"), metric);
	}
	menu->AppendSeparator();
	{
		wxMenu *user = new wxMenu();
		for(int i = 0; i < settings->grids.Size(); i++)
			user->AppendCheckItem(ID_GRID_USER + i, settings->GetGridStr(settings->grids[i]))
					->Check(pcb->GetSelectedBoard()->GetGrid() == settings->grids[i]);
		if(!settings->grids.Empty())
			user->AppendSeparator();
		user->Append(ID_GRID_USER_NEW, _("Add new grid value..."));
		if(!settings->grids.Empty()) {
			wxMenu *remove = new wxMenu();
			for(int i = 0; i < settings->grids.Size(); i++)
				remove->Append(ID_GRID_USER_DEL + i, settings->GetGridStr(settings->grids[i]));
			user->Append(wxID_ANY, _("Remove"), remove);
		}
		menu->Append(wxID_ANY, _("User grids"), user);
	}
	menu->AppendSeparator();
	menu->Append(ID_GRID_HOTKEYS, _("Hotkeys..."));
	{
		wxMenu *style = new wxMenu;
		style->AppendRadioItem(ID_GRID_LINES, _("Lines"));
		style->AppendRadioItem(ID_GRID_DOTS, _("Dots"));
		style->Check(ID_GRID_LINES + settings->gridStyle, true);
		menu->Append(wxID_ANY, _("Grid style"), style);
	}
	{
		wxMenu *sub = new wxMenu;
		sub->AppendRadioItem(ID_SUBGRID_OFF, _("Off"));
		sub->AppendRadioItem(ID_SUBGRID_2, "2");
		sub->AppendRadioItem(ID_SUBGRID_4, "4");
		sub->AppendRadioItem(ID_SUBGRID_5, "5");
		sub->AppendRadioItem(ID_SUBGRID_10, "10");

		sub->Check(ID_SUBGRID_OFF + settings->subGrid, true); //enable item
		menu->Append(wxID_ANY, _("Subdivisions"), sub);
	}
	menu->AppendCheckItem(ID_GRID_SHOW, _("Show grid"))->Check(settings->showGrid);
	PopupMenu(menu);
	delete menu;
}

void LeftPanel::UpdateSizes(wxUpdateUIEvent &e) {
	float track = settings->trackSize;
	PadSize	pad = settings->padSize;
	Vec2	smd = settings->smdSize;

	Object *selected = pcb->GetSelectedBoard()->GetFirstSelected();
	if(selected) {
		switch(selected->GetType()) {
		case Object::TRACK: case Object::POLY: case Object::ARC:
			track = ((LineObject*) selected)->GetWidth();
			break;
		case Object::THT_PAD:
			pad = ((THTPad*) selected)->GetSize();
			break;
		case Object::SMD_PAD:
			smd = ((SMDPad*) selected)->GetSize();
			break;
		}
	}

	trackSize->SetValue(settings->ConvertToUnits(track));
	padSizeOuter->SetValue(settings->ConvertToUnits(pad.out));
	padSizeInner->SetValue(settings->ConvertToUnits(pad.in));
	smdWidth->SetValue(settings->ConvertToUnits(smd.x));
	smdHeight->SetValue(settings->ConvertToUnits(smd.y));
}

void LeftPanel::SetTrackSize(float size) {
	settings->trackSize = size;
	for(Object *object = pcb->GetSelectedBoard()->GetObjects(); object; object = object->GetNext())
		if(object->IsSelected()) {
			uint8_t type = object->GetType();
			if(type == Object::TRACK || type == Object::POLY || type == Object::ARC)
				((LineObject*) object)->SetWidth(size);
		}
}

void LeftPanel::SetTrackSize(wxSpinDoubleEvent&) {
	SetTrackSize(settings->ConvertFromUnits(trackSize->GetValue()));
}
void LeftPanel::SelectTrackSize(wxCommandEvent &e) {
	SetTrackSize(settings->trackSizes[e.GetId() - ID_TRACK_SEL]);
}
void LeftPanel::RemoveTrackSize(wxCommandEvent &e) {
	settings->trackSizes.RemoveIndex(e.GetId() - ID_TRACK_DEL);
}
void LeftPanel::AddTrackSize(wxCommandEvent &e) {
	settings->trackSizes.Add(settings->trackSize);
}

void LeftPanel::SetPadSize(const PadSize &size) {
	settings->padSize = size;
	for(Object *object = pcb->GetSelectedBoard()->GetObjects(); object; object = object->GetNext())
		if(object->IsSelected() && object->GetType() == Object::THT_PAD)
			((THTPad*) object)->SetSize(size);
}

void LeftPanel::SetPadSize(wxSpinDoubleEvent&) {
	SetPadSize(PadSize(settings->ConvertFromUnits(padSizeOuter->GetValue()),
						settings->ConvertFromUnits(padSizeInner->GetValue())));
}
void LeftPanel::SelectPadSize(wxCommandEvent &e) {
	SetPadSize(settings->padSizes[e.GetId() - ID_PAD_SEL]);
}
void LeftPanel::RemovePadSize(wxCommandEvent &e) {
	settings->padSizes.RemoveIndex(e.GetId() - ID_PAD_DEL);
}
void LeftPanel::AddPadSize(wxCommandEvent &e) {
	settings->padSizes.Add(settings->padSize);
}

void LeftPanel::SetSmdSize(const Vec2 &size) {
	settings->smdSize = size;
	for(Object *object = pcb->GetSelectedBoard()->GetObjects(); object; object = object->GetNext())
		if(object->IsSelected() && object->GetType() == Object::SMD_PAD)
			((SMDPad*) object)->SetSize(size);
}

void LeftPanel::SetSmdSize(wxSpinDoubleEvent&) {
	SetSmdSize(Vec2(settings->ConvertFromUnits(smdWidth->GetValue()),
					settings->ConvertFromUnits(smdHeight->GetValue())));
}
void LeftPanel::SwapSmdSize(wxCommandEvent&) {
	SetSmdSize(Vec2(settings->ConvertFromUnits(smdHeight->GetValue()),
					settings->ConvertFromUnits(smdWidth->GetValue())));
}
void LeftPanel::SelectSmdSize(wxCommandEvent &e) {
	SetSmdSize(settings->smdSizes[e.GetId() - ID_SMD_SEL]);
}
void LeftPanel::RemoveSmdSize(wxCommandEvent &e) {
	settings->smdSizes.RemoveIndex(e.GetId() - ID_SMD_DEL);
}
void LeftPanel::AddSmdSize(wxCommandEvent &e) {
	settings->smdSizes.Add(settings->smdSize);
}

void LeftPanel::ShowTrackMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	bool selectedAny = false;
	if(!settings->trackSizes.Empty()) {
		for(int i = 0; i < settings->trackSizes.Size(); i++) {
			bool selected = settings->trackSizes[i] == settings->trackSize;
			selectedAny |= selected;
			menu->AppendCheckItem(ID_TRACK_SEL + i, settings->GetStr(settings->trackSizes[i]))->Check(selected);
		}
		menu->AppendSeparator();
	}
	AddItem(menu, ID_TRACK_ADD, settings->GetStr(settings->trackSize), plus_xpm, !selectedAny);
	if(!settings->trackSizes.Empty()) {
		menu->AppendSeparator();
		wxMenu *remove = new wxMenu();
		for(int i = 0; i < settings->trackSizes.Size(); i++)
			AddItem(remove, ID_TRACK_DEL + i, settings->GetStr(settings->trackSizes[i]), cross_xpm);
		AddSubmenu(menu, remove, _("Remove"), cross_xpm);
	}
	PopupMenu(menu);
}

void LeftPanel::ShowPadMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	bool selectedAny = false;
	if(!settings->padSizes.Empty()) {
		for(int i = 0; i < settings->padSizes.Size(); i++) {
			bool selected = settings->padSizes[i] == settings->padSize;
			selectedAny |= selected;
			menu->AppendCheckItem(ID_PAD_SEL + i, settings->GetStr(settings->padSizes[i]))->Check(selected);
		}
		menu->AppendSeparator();
	}
	AddItem(menu, ID_PAD_ADD, settings->GetStr(settings->padSize), plus_xpm, !selectedAny);
	if(!settings->padSizes.Empty()) {
		menu->AppendSeparator();
		wxMenu *remove = new wxMenu();
		for(int i = 0; i < settings->padSizes.Size(); i++)
			AddItem(remove, ID_PAD_DEL + i, settings->GetStr(settings->padSizes[i]), cross_xpm);
		AddSubmenu(menu, remove, _("Remove"), cross_xpm);
	}
	PopupMenu(menu);
}

void LeftPanel::ShowSmdMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	bool selectedAny = false;
	if(!settings->smdSizes.Empty()) {
		for(int i = 0; i < settings->smdSizes.Size(); i++) {
			bool selected = settings->smdSizes[i] == settings->smdSize;
			selectedAny |= selected;
			menu->AppendCheckItem(ID_SMD_SEL + i, settings->GetStr(settings->smdSizes[i]))->Check(selected);
		}
		menu->AppendSeparator();
	}
	AddItem(menu, ID_SMD_ADD, settings->GetStr(settings->smdSize), plus_xpm, !selectedAny);
	if(!settings->smdSizes.Empty()) {
		menu->AppendSeparator();
		wxMenu *remove = new wxMenu();
		for(int i = 0; i < settings->smdSizes.Size(); i++)
			AddItem(remove, ID_SMD_DEL + i, settings->GetStr(settings->smdSizes[i]), cross_xpm);
		AddSubmenu(menu, remove, _("Remove"), cross_xpm);
	}
	PopupMenu(menu);
}

