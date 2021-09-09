#include "ScannedCopyDialog.h"
#include "InputNumberDialog.h"
#include "OpenLayoutMain.h"
#include "NewBoardDialog.h"
#include "LayerInfoDialog.h"
#include "ProjectInfoDialog.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"
#include "LeftPanel.h"
#include "GLCanvas.h"
#include "OpenLayoutApp.h"
#include "BottomPanel.h"
#include <wx/msgdlg.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/filedlg.h>

#define MAIN_XPM
#include "images.h"
#undef MAIN_XPM

#include <string>

#include "Utils.h"
using namespace std;

enum {
	ID_SAVE_MACRO=1,
	ID_AUTOSAVE,
	ID_GERBER_IMPORT,
	ID_GERBER_EXPORT,
	ID_DRILL_DATA,
	ID_ISOLATION,
	ID_SAVE_BMP,
	ID_SAVE_JPG,
	ID_SAVE_GIF,
	ID_SAVE_EMP,
	ID_SAVE_PNG,
	ID_DIRECTORIES,
	ID_BOARD_NEW,
	ID_BOARD_PROPERTIES,
	ID_BOARD_COPY,
	ID_BOARD_DELETE,
	ID_BOARD_SET_RIGHT,
	ID_BOARD_SET_LEFT,
	ID_BOARD_MOVE_RIGHT,
	ID_BOARD_MOVE_LEFT,
	ID_BOARD_SAVE,
	ID_BOARD_IMPORT,
	ID_ROTATE,
	ID_VMIRROR,
	ID_HMIRROR,
	ID_GROUP,
	ID_UNGROUP,
	ID_CHANGE_SIDE,
	ID_CHANGE_LAYER,
	ID_LAYER_C1,
	ID_LAYER_S1,
	ID_LAYER_C2,
	ID_LAYER_S2,
	ID_LAYER_I1,
	ID_LAYER_I2,
	ID_LAYER_O,
	ID_SNAP_GRID,
	ID_MASSIVE,
	ID_LIST_DRILLINGS,
	ID_SCANNED_COPY,
	ID_FOOTPRINT,
	ID_RESET_MASK,
	ID_REMOVE_CONNECTIONS,
	ID_DELETE_OUTSIDE,
	ID_ELEMENT_IMPORT,
	ID_ELEMENT_EXPORT,
	ID_DEFINE_PLUGIN,
	ID_RUN_PLUGIN,
	ID_PANEL_MACRO,
	ID_PANEL_PROPERTIES,
	ID_PANEL_DRC,
	ID_PANEL_COMPONENTS,
	ID_PANEL_SELECTOR,
	ID_ALIGN,
	ID_TRANSPARENT,
	ID_ZOOM,
	ID_ZOOM_BOARD,
	ID_ZOOM_OBJECTS,
	ID_ZOOM_SELECTION,
	ID_ZOOM_PREVIOUS,

	ID_ANGLE_90,
	ID_ANGLE_45,
	ID_ANGLE_10,
	ID_ANGLE_5,
	ID_ANGLE_CUSTOM,
	ID_ANGLE_CUSTOM_CH,
};
wxBEGIN_EVENT_TABLE(OpenLayoutFrame, wxFrame)
	EVT_MENU(wxID_EXIT, OpenLayoutFrame::close)
	EVT_MENU(wxID_PROPERTIES, OpenLayoutFrame::show_settings)
	EVT_MENU(wxID_ABOUT, OpenLayoutFrame::show_about)
	EVT_MENU(wxID_INFO, OpenLayoutFrame::show_project_info)
	EVT_MENU(ID_BOARD_NEW, OpenLayoutFrame::new_board)
	EVT_MENU(ID_SCANNED_COPY, OpenLayoutFrame::show_scan_properties)
	EVT_MENU(ID_GROUP, OpenLayoutFrame::group)
	EVT_MENU(ID_UNGROUP, OpenLayoutFrame::ungroup)
	EVT_MENU(wxID_SELECTALL, OpenLayoutFrame::select_all)
	EVT_MENU(wxID_SAVE, OpenLayoutFrame::savefile)
	EVT_MENU(wxID_SAVEAS, OpenLayoutFrame::savefile)
	EVT_MENU(wxID_OPEN, OpenLayoutFrame::openfile)
	EVT_MENU_RANGE(ID_LAYER_C1, ID_LAYER_O, OpenLayoutFrame::set_sel_layer)
	EVT_UPDATE_UI(wxID_COPY,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(wxID_CUT,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(wxID_DELETE,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(wxID_DUPLICATE,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(ID_ROTATE,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(ID_HMIRROR,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(ID_VMIRROR,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(ID_SNAP_GRID,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI(ID_GROUP,OpenLayoutFrame::updateui_group)
	EVT_UPDATE_UI(ID_UNGROUP,OpenLayoutFrame::updateui_ungroup)
	EVT_UPDATE_UI_RANGE(ID_LAYER_C1,ID_LAYER_S2,OpenLayoutFrame::updateui_edit)
	EVT_UPDATE_UI_RANGE(ID_LAYER_I1,ID_LAYER_I2,OpenLayoutFrame::updateui_multilayer)
	EVT_UPDATE_UI(ID_LAYER_O,OpenLayoutFrame::updateui_edit)
wxEND_EVENT_TABLE()

OpenLayoutFrame::OpenLayoutFrame(const char *filename)
	:wxFrame(0,wxID_ANY,"OpenLayout",wxDefaultPosition,{800,600}) {
	wxBoxSizer *all_box=new wxBoxSizer(wxVERTICAL);
	init_menu_bar();
	init_tool_bar(all_box);
	{
		wxBoxSizer *content=new wxBoxSizer(wxHORIZONTAL);
		wxScrolledWindow *left_panel=new LeftPanel(this);
		content->Add(left_panel,0,wxEXPAND);
		{
			canvas=new GLCanvas(this);
			content->Add(canvas,1,wxEXPAND|wxALL,2);
		}
		all_box->Add(content,1,wxEXPAND);

		bottom_panel=new BottomPanel(this);
		all_box->Add(bottom_panel,0,wxEXPAND);
	}
	SetSizer(all_box);
	SetAutoLayout(true);

	if(filename)
		PCB.load(filename);
	wxUpdateUIEvent::SetUpdateInterval(40);
}

void OpenLayoutFrame::close(wxCommandEvent&) {
	Close(true);
}
void OpenLayoutFrame::show_settings(wxCommandEvent&) {
	SettingsDialog settings(this,SETTINGS);
	if(settings.ShowModal()==wxID_OK){
		settings.Get(SETTINGS);
		static_cast<BottomPanel*>(bottom_panel)->UpdateColors();
	}
}
void OpenLayoutFrame::show_about(wxCommandEvent&) {
	AboutDialog(this).ShowModal();
}
void OpenLayoutFrame::show_project_info(wxCommandEvent&) {
	ProjectInfoDialog dialog(this,PCB.info);
	if(dialog.ShowModal()==wxID_OK)
		dialog.Get(PCB.info);
}
void OpenLayoutFrame::show_scan_properties(wxCommandEvent&) {
	ScannedCopyDialog dialog(this,PCB.GetSelectedBoard().images,SETTINGS);
	if(dialog.ShowModal()==wxID_OK)
		dialog.Get(PCB.GetSelectedBoard().images);
}
void OpenLayoutFrame::new_board(wxCommandEvent&) {
	NewBoardDialog dialog(this);
	while(dialog.ShowModal()==wxID_OK) {
		if(dialog.isValid()) {
			PCB.AddBoard(dialog.build());
			wxMessageBox(_("A new board was added"),GetTitle(),wxICON_INFORMATION);
			break;
		} else
			wxMessageBox(_("The board is too big!"),GetTitle(),wxICON_ERROR);
	}
}
void OpenLayoutFrame::init_menu_bar() {
	wxMenuBar* menu_bar = new wxMenuBar();
	{
		//File
		wxMenu* menu= new wxMenu();
		menu->Append(wxID_NEW,_("New\tCtrl+N"));
		menu->Append(wxID_OPEN,_("Open\tCtrl+O"));
		menu->Append(wxID_SAVE,_("Save\tCtrl+S"));
		menu->Append(wxID_SAVEAS,_("Save as\tCtrl+Shift+S"));
		menu->AppendSeparator();
		menu->Append(ID_SAVE_MACRO,_("Save as macro"));
		menu->AppendSeparator();
		menu->Append(ID_AUTOSAVE,_("AutoSave"));
		menu->AppendSeparator();
		menu->Append(ID_GERBER_IMPORT,_("Gerber-Import"));
		menu->AppendSeparator();
		{
			//File->Export
			wxMenu *submenu = new wxMenu();
			submenu->Append(ID_GERBER_EXPORT,_("Gerber Export"));
			submenu->Append(ID_DRILL_DATA,_("Drill data (Excellon)"));
			submenu->AppendSeparator();
			submenu->Append(ID_ISOLATION,_("Isolation milling (HPGL, *.plt)"));
			submenu->AppendSeparator();
			submenu->Append(ID_SAVE_BMP,_("Bitmap (*.bmp)"));
			submenu->Append(ID_SAVE_JPG,_("JPG (*.jpg)"));
			submenu->Append(ID_SAVE_GIF,_("GIF (*.gif)"));
			submenu->Append(ID_SAVE_EMP,_("EMP (*.emp)"));
			submenu->Append(ID_SAVE_PNG,_("PNG (*.png)"));
			menu->Append(wxID_ANY, _("Export"), submenu);
		}
		menu->AppendSeparator();
		menu->Append(ID_DIRECTORIES,_("Directories"));
		menu->AppendSeparator();
		menu->Append(wxID_PRINT_SETUP,_("Printer setup"));
		menu->Append(wxID_PRINT,_("Print\tCtrl+P"));
		menu->AppendSeparator();
		menu->Append(wxID_EXIT,_("Exit\tCtrl+Q"));
		menu_bar->Append(menu, _("&File"));
	}
	{
		//Edit
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_UNDO,_("&Undo\tCtrl+Z"));
		menu->Append(wxID_REDO,_("&Redo\tCtrl+Y"));
		menu->AppendSeparator();
		menu->Append(wxID_COPY,_("C&opy\tCtrl+C"));
		menu->Append(wxID_CUT,_("&Cut\tCtrl+X"));
		menu->Append(wxID_PASTE,_("&Paste\tCtrl+V"));
		menu->Append(wxID_DUPLICATE,_("Dup&licate\tCtrl+D"));
		menu->Append(wxID_DELETE,_("&Delete\tDelete"));
		menu->AppendSeparator();
		menu->Append(wxID_SELECTALL,_("Select &all\tCtrl+A"));
		menu_bar->Append(menu, _("&Edit"));
	}
	{
		//Board
		wxMenu *menu = new wxMenu();
		menu->Append(ID_BOARD_NEW,_("Add new board"));
		menu->Append(ID_BOARD_PROPERTIES,_("Properties"));
		menu->Append(ID_BOARD_COPY,_("Copy board"));
		menu->Append(ID_BOARD_DELETE,_("Delete board"));
		menu->AppendSeparator();
		menu->Append(ID_BOARD_SET_RIGHT,_("Set board to &right"));
		menu->Append(ID_BOARD_SET_LEFT,_("Set board to &left"));
		menu->AppendSeparator();
		menu->Append(ID_BOARD_MOVE_RIGHT,_("&Move board to right"));
		menu->Append(ID_BOARD_MOVE_LEFT,_("M&ove board to left"));
		menu->Append(ID_BOARD_IMPORT,_("&Import boards from file"));
		menu->Append(ID_BOARD_SAVE,_("&Save boards to file"));
		menu_bar->Append(menu, _("&Board"));
	}
	{
		//Functions
		wxMenu *menu = new wxMenu();
		menu->Append(ID_ROTATE,_("&Rotate\tCtrl+R"));
		menu->AppendSeparator();
		menu->Append(ID_HMIRROR,_("Mirror &horisontal\tCtrl+H"));
		menu->Append(ID_VMIRROR,_("Mirror &vertical\tCtrl+T"));
		menu->AppendSeparator();
		menu->Append(ID_GROUP,_("Build &group\tCtrl+G"));
		menu->Append(ID_UNGROUP,_("Split gro&up\tCtrl+U"));
		menu->AppendSeparator();
		menu->Append(ID_CHANGE_SIDE,_("&Change board side\tCtrl+W"));
		{
			//Functions->Set to layer
			wxMenu *submenu = new wxMenu();
			submenu->Append(ID_LAYER_C1,_("&C1"));
			submenu->Append(ID_LAYER_S1,_("S&1"));
			submenu->Append(ID_LAYER_C2,_("C&2"));
			submenu->Append(ID_LAYER_S2,_("&S2"));
			submenu->Append(ID_LAYER_I1,_("I1"));
			submenu->Append(ID_LAYER_I2,_("&I2"));
			submenu->Append(ID_LAYER_O, _("&O"));
			menu->Append(ID_CHANGE_LAYER, _("&Set to layer"), submenu);
		}
		menu->AppendSeparator();
		menu->Append(ID_SNAP_GRID,_("S&nap to grid"));
		menu->Append(ID_MASSIVE,_("&Tile / Arrange circular"));
		menu_bar->Append(menu, _("F&unctions"));
	}
	{
		//Extras
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_INFO,_("&Project info"));
		menu->Append(ID_LIST_DRILLINGS,_("&List drillings"));
		menu->AppendSeparator();
		menu->Append(ID_SCANNED_COPY,_("&Scanned copy"));
		menu->AppendSeparator();
		menu->Append(ID_FOOTPRINT,_("&Footprint-Wizard"));
		menu->AppendSeparator();
		menu->Append(ID_RESET_MASK,_("R&eset solder mask"));
		menu->Append(ID_REMOVE_CONNECTIONS,_("&Remove connections (rubberbands)"));
		menu->Append(ID_DELETE_OUTSIDE,_("&Delete elements outside the board"));
		menu->AppendSeparator();
		menu->Append(ID_ELEMENT_IMPORT,_("&Text-IO: Import elements"));
		menu->Append(ID_ELEMENT_EXPORT,_("Te&xt-IO: Export elements"));
		menu->AppendSeparator();
		menu->Append(ID_DEFINE_PLUGIN,_("Def&ine plugin"));
		menu->Append(ID_RUN_PLUGIN,_("R&un plugin"));
		menu_bar->Append(menu, _("E&xtras"));
	}
	{
		//Options
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_PROPERTIES,_("&General settings"));
		menu->AppendSeparator();
		menu->Append(ID_PANEL_MACRO,_("&Macro-Library"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_PROPERTIES,_("&Properties-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_DRC,_("&DRC-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_COMPONENTS,_("&Components-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_SELECTOR,_("S&elector-Panel"));
		menu->AppendSeparator();
		menu->Append(ID_ZOOM_BOARD,_("Zoom &board"));
		menu->Append(ID_ZOOM_OBJECTS,_("Zoom &objects"));
		menu->Append(ID_ZOOM_SELECTION,_("Zoom &selection"));
		menu->Append(ID_ZOOM_PREVIOUS,_("&Zoom previous"));
		menu_bar->Append(menu, _("&Options"));
	}
	{
		//Help
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_ABOUT,_("&About"));
		menu_bar->Append(menu, _("&Help"));
	}
	SetMenuBar(menu_bar);
}
void OpenLayoutFrame::init_tool_bar(wxBoxSizer *sizer) {
	wxToolBar *tool_bar = new wxToolBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTB_HORIZONTAL|wxTB_FLAT);
	tool_bar->AddTool(wxID_NEW, _("New file"),new_xpm);
	tool_bar->AddTool(wxID_OPEN, _("Open file"),open_xpm);
	tool_bar->AddTool(wxID_SAVE, _("Save file"),save_xpm);
	tool_bar->AddTool(wxID_PRINT, _("Print"), print_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(wxID_UNDO, _("Undo"), undo_xpm);
	tool_bar->AddTool(wxID_REDO, _("Redo"), redo_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(wxID_CUT, _("Cut"), cut_xpm);
	tool_bar->AddTool(wxID_COPY, _("Copy"), copy_xpm);
	tool_bar->AddTool(wxID_PASTE, _("Paste"), paste_xpm);
	tool_bar->AddTool(wxID_DELETE, _("Delete"), delete_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(wxID_DUPLICATE, _("Duplicate"), duplicate_xpm);
	tool_bar->AddTool(ID_ROTATE, _("Rotate"), rotate_xpm,wxNullBitmap,wxITEM_DROPDOWN);
	{
		//create menu for rotate button
		wxMenu *menu=new wxMenu();
		menu->AppendRadioItem(ID_ANGLE_90,"90");
		menu->AppendRadioItem(ID_ANGLE_45,"45");
		menu->AppendRadioItem(ID_ANGLE_10,"10");
		menu->AppendRadioItem(ID_ANGLE_5,"5");
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			switch(e.GetId()) {
			case ID_ANGLE_90:
				SETTINGS.rotation_angle=90.0f;
				break;
			case ID_ANGLE_45:
				SETTINGS.rotation_angle=45.0f;
				break;
			case ID_ANGLE_10:
				SETTINGS.rotation_angle=10.0f;
				break;
			case ID_ANGLE_5:
				SETTINGS.rotation_angle=5.0f;
				break;
			}
		},ID_ANGLE_90,ID_ANGLE_CUSTOM);
		tool_bar->SetDropdownMenu(ID_ROTATE,menu);
	}
	tool_bar->AddTool(ID_HMIRROR,_("Mirror horizontal"), mirror_h_xpm);
	tool_bar->AddTool(ID_VMIRROR, _("Mirror vertical"), mirror_v_xpm);
	tool_bar->AddTool(ID_ALIGN, _("Align elements"), align_xpm);
	tool_bar->AddTool(ID_SNAP_GRID, _("Snap to grid"), to_grid_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(ID_GROUP, _("Build group"), group_on_xpm);
	tool_bar->AddTool(ID_UNGROUP, _("Split group"), group_off_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(ID_ZOOM, _("Zoom"), zoom_any_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(ID_TRANSPARENT, _("Transparent"),transparent_xpm);
	tool_bar->AddTool(ID_PANEL_PROPERTIES, _("Info"), info_xpm);
	tool_bar->AddSeparator();
	tool_bar->AddTool(ID_SCANNED_COPY, _("Scanned copy"), bitmap_xpm);
	tool_bar->Realize();
	SetToolBar(tool_bar);
}
void OpenLayoutFrame::updateui_edit(wxUpdateUIEvent &e) {
	e.Enable(PCB.GetSelectedBoard().is_selected());
}
void OpenLayoutFrame::updateui_group(wxUpdateUIEvent &e) {
	e.Enable(PCB.GetSelectedBoard().can_group());
}
void OpenLayoutFrame::updateui_ungroup(wxUpdateUIEvent &e) {
	e.Enable(PCB.GetSelectedBoard().can_ungroup());
}
void OpenLayoutFrame::updateui_multilayer(wxUpdateUIEvent &e) {
	e.Enable(PCB.GetSelectedBoard().is_selected() && PCB.GetSelectedBoard().is_multilayer);
}

void OpenLayoutFrame::group(wxCommandEvent &e) {
	PCB.GetSelectedBoard().group();
	canvas->Refresh();
}
void OpenLayoutFrame::ungroup(wxCommandEvent &e) {
	PCB.GetSelectedBoard().ungroup();
	canvas->Refresh();
}
void OpenLayoutFrame::del(wxCommandEvent &e) {
	//PCB.GetSelectedBoard().();
}
void OpenLayoutFrame::set_sel_layer(wxCommandEvent &e) {
	int id=e.GetId()-ID_LAYER_C1;
	PCB.GetSelectedBoard().set_sel_layer(id+1);
	canvas->Refresh();
}
void OpenLayoutFrame::select_all(wxCommandEvent &e) {
	PCB.GetSelectedBoard().select_all();
	canvas->Refresh();
}
void OpenLayoutFrame::openfile(wxCommandEvent &e) {
	wxFileDialog dialog(this, _("Save layout file"), "", "",
						_("Layout files (*.lay*)|*.lay*|All files (*.*)|*.*"), wxFD_OPEN);

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	wxString path=dialog.GetPath();
	PCB.load(path.c_str());
	canvas->Refresh();
}
void OpenLayoutFrame::savefile(wxCommandEvent &e) {
	wxFileDialog dialog(this, _("Save layout file"), "", "",
						_("Layout files (*.lay*)|*.lay*|All files (*.*)|*.*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	wxString path=dialog.GetPath();
	PCB.save(path.c_str());
}
void OpenLayoutFrame::RefreshCanvas(){
	canvas->Refresh();
}
