#include "ToolBar.h"
#include "OpenLayoutMain.h"
#include <wx/menu.h>

extern "C" {
#include "toolbar/align_bottom.xpm"
#include "toolbar/align_hcenter.xpm"
#include "toolbar/align_left.xpm"
#include "toolbar/align_right.xpm"
#include "toolbar/align_top.xpm"
#include "toolbar/align_vcenter.xpm"
#include "toolbar/align.xpm"
#include "toolbar/bitmap.xpm"
#include "toolbar/components.xpm"
#include "toolbar/copy.xpm"
#include "toolbar/cut.xpm"
#include "toolbar/delete.xpm"
#include "toolbar/drc.xpm"
#include "toolbar/duplicate.xpm"
#include "toolbar/group_off.xpm"
#include "toolbar/group_on.xpm"
#include "toolbar/info.xpm"
#include "toolbar/macro.xpm"
#include "toolbar/mirror_h.xpm"
#include "toolbar/mirror_v.xpm"
#include "toolbar/new.xpm"
#include "toolbar/open.xpm"
#include "toolbar/paste.xpm"
#include "toolbar/print.xpm"
#include "toolbar/properties.xpm"
#include "toolbar/redo.xpm"
#include "toolbar/rotate.xpm"
#include "toolbar/save.xpm"
#include "toolbar/selector.xpm"
#include "toolbar/to_grid.xpm"
#include "toolbar/transparent.xpm"
#include "toolbar/undo.xpm"
#include "toolbar/zoom_any.xpm"
#include "toolbar/zoom_board.xpm"
#include "toolbar/zoom_objects.xpm"
#include "toolbar/zoom_prev.xpm"
#include "toolbar/zoom_selection.xpm"
}

wxBEGIN_EVENT_TABLE(ToolBar, wxToolBar)
	EVT_MENU(ID_ALIGN,	ToolBar::Align)
	EVT_MENU(ID_ZOOM,	ToolBar::Zoom)
wxEND_EVENT_TABLE()

ToolBar::ToolBar(wxWindow *parent)
		:wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxTB_HORIZONTAL|wxTB_FLAT){
	AddTool(wxID_NEW, _("New"), new_xpm, _("New "));
	AddTool(wxID_OPEN, _("Open"), open_xpm, _("Open "));
	AddTool(wxID_SAVE, _("Save"), save_xpm, _("Save "));
	AddTool(wxID_PRINT, _("Print"), print_xpm, _("Print"));
	AddSeparator();
	AddTool(wxID_UNDO, _("Undo"), undo_xpm, _("Undo"));
	AddTool(wxID_REDO, _("Redo"), redo_xpm, _("Redo"));
	AddSeparator();
	AddTool(wxID_CUT, _("Cut"), cut_xpm, _("Cut to clipboard"));
	AddTool(wxID_COPY, _("Copy"), copy_xpm, _("Copy to clipboard"));
	AddTool(wxID_PASTE, _("Paste"), paste_xpm, _("Paste from clipboard"));
	AddTool(wxID_DELETE, _("Delete"), delete_xpm, _("Delete"));
	AddSeparator();
	AddTool(wxID_DUPLICATE, _("Duplicate"), duplicate_xpm, _("Duplicate"));
	AddTool(ID_ROTATE, _("Rotate"), rotate_xpm, _("Rotate"), wxITEM_DROPDOWN);
	{
		wxMenu *menu = new wxMenu();
		menu->AppendRadioItem(ID_ROTATE_90,	L"90\x00b0");
		menu->AppendRadioItem(ID_ROTATE_45,	L"45\x00b0");
		menu->AppendRadioItem(ID_ROTATE_10,	L"10\x00b0");
		menu->AppendRadioItem(ID_ROTATE_5,	L"5\x00b0");
		SetDropdownMenu(ID_ROTATE, menu);
	}
	AddTool(ID_HMIRROR, _("Mirror horizontal"), mirror_h_xpm, _("Mirror horisontal"));
	AddTool(ID_VMIRROR, _("Mirror vertical"), mirror_v_xpm, _("Mirror vertical"));
	AddTool(ID_ALIGN, _("Align elements..."), align_xpm, _("Align elements..."));
	AddTool(ID_SNAP_GRID, _("Snap to grid"), to_grid_xpm, _("Snap to grid"));
	AddSeparator();
	AddTool(ID_GROUP, _("Build group"), group_on_xpm, _("Build group"));
	AddTool(ID_UNGROUP, _("Split group"), group_off_xpm, _("Split group"));
	AddSeparator();
	AddTool(ID_ZOOM, _("Zoom"), zoom_any_xpm, _("Zoom functions"));
	AddSeparator();
	AddTool(ID_TRANSPARENT, _("Transparent"), transparent_xpm, _("Toggle transparent mode"));
	AddTool(ID_PANEL_PROPERTIES, _("Info"), info_xpm, _("Show project info"));
	AddSeparator();
	AddTool(ID_SCANNED_COPY, _("Scanned copy"), bitmap_xpm, _("Scanned copy"));
	AddStretchableSpace();
	AddTool(ID_PANEL_SELECTOR, _("Selector-Panel"), selector_xpm, _("Show/hide Selector-Panel"));
	AddTool(ID_PANEL_COMPONENTS, _("Component-Panel"), components_xpm, _("Show/hide Component-Panel"));
	AddTool(ID_PANEL_SELECTOR, _("Properties-Panel"), properties_xpm, _("Show/hide Properties-Panel"));
	AddTool(ID_PANEL_DRC, _("DRC-Panel"), drc_xpm, _("Show/hide DRC-Panel"));
	AddTool(ID_PANEL_MACRO, _("Macro-Library"), macro_xpm, _("Show/hide Macro-Library"));
	Realize();
}

static void AddMenuItem(wxMenu *parent, const wxString &text, int id, const wxBitmap &bitmap){
	wxMenuItem *item = new wxMenuItem(parent, id, text);
	item->SetBitmap(bitmap);
	parent->Append(item);
}

void ToolBar::Align(wxCommandEvent&){
	wxMenu *menu = new wxMenu();
	AddMenuItem(menu, _("Align top"),				ID_ALIGN_TOP,		align_top_xpm);
	AddMenuItem(menu, _("Align bottom"),			ID_ALIGN_BOTTOM,	align_bottom_xpm);
	AddMenuItem(menu, _("Align left"),				ID_ALIGN_LEFT,		align_left_xpm);
	AddMenuItem(menu, _("Align right"),				ID_ALIGN_RIGHT,		align_right_xpm);
	AddMenuItem(menu, _("Align horizontal-center"),	ID_ALIGN_HCENTER, 	align_hcenter_xpm);
	AddMenuItem(menu, _("Align vertical-center"),	ID_ALIGN_VCENTER, 	align_vcenter_xpm);
	PopupMenu(menu);
}
void ToolBar::Zoom(wxCommandEvent&){
	wxMenu *menu = new wxMenu();
	AddMenuItem(menu, _("Zoom previous"),	ID_ZOOM_PREVIOUS,	zoom_prev_xpm);
	menu->AppendSeparator();
	AddMenuItem(menu, _("Zoom board"),		ID_ZOOM_BOARD,		zoom_board_xpm);
	AddMenuItem(menu, _("Zoom objects"),	ID_ZOOM_OBJECTS,	zoom_objects_xpm);
	AddMenuItem(menu, _("Zoom selection"),	ID_ZOOM_SELECTION,	zoom_selection_xpm);
	PopupMenu(menu);
}
