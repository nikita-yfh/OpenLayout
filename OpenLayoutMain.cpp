/***************************************************************
 * Name:      OpenLayoutMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:
 **************************************************************/

#include "OpenLayoutMain.h"
#include "NewBoardDialog.h"
#include "LayerInfoDialog.h"
#include "ProjectInfoDialog.h"
#include "SettingsDialog.h"
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>

#define MAIN_XPM
#include "images.h"
#undef MAIN_XPM

#include <string>
#include <sstream>
using namespace std;

OpenLayoutFrame::OpenLayoutFrame()
		:wxFrame(0,wxID_ANY,"OpenLayout"){
	wxBoxSizer *all_box=new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer *content=new wxBoxSizer(wxHORIZONTAL);
		init_left_panel(content);
		all_box->Add(content);
	}
	SetSizerAndFit(all_box);
	Bind(wxEVT_MENU,&OpenLayoutFrame::close,this, wxID_EXIT);
	Bind(wxEVT_MENU,&OpenLayoutFrame::open_settings,this,wxID_PROPERTIES);
    init_menu_bar();
    init_tool_bar();

    build_pad_menu();
    build_smd_menu();
}

void OpenLayoutFrame::close(wxCommandEvent&){
	Close(true);
}
void OpenLayoutFrame::open_settings(wxCommandEvent&){
	SettingsDialog *settings=new SettingsDialog(this,s);
	if(settings->ShowModal()==wxID_OK)
		settings->Get(s);
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
        menu_bar->Append(menu, _("File"));
    }

    {
        //Edit
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_UNDO,_("Undo\tCtrl+Z"));
        menu->Append(wxID_REDO,_("Redo\tCtrl+Y"));
        menu->AppendSeparator();
        menu->Append(wxID_COPY,_("Copy\tCtrl+C"));
        menu->Append(wxID_CUT,_("Cut\tCtrl+X"));
        menu->Append(wxID_PASTE,_("Paste\tCtrl+V"));
        menu->Append(wxID_DUPLICATE,_("Duplicate\tCtrl+D"));
        menu->Append(wxID_DELETE,_("Delete\tDelete"));
        menu->AppendSeparator();
        menu->Append(wxID_SELECTALL,_("Select all\tCtrl+A"));
        menu_bar->Append(menu, _("Edit"));
    }
    {
        //Board
        wxMenu *menu = new wxMenu();
        menu->Append(ID_BOARD_NEW,_("Add new board"));
        menu->Append(ID_BOARD_PROPERTIES,_("Properties"));
        menu->Append(ID_BOARD_COPY,_("Copy board"));
        menu->Append(ID_BOARD_DELETE,_("Delete board"));
        menu->AppendSeparator();
        menu->Append(ID_BOARD_SET_RIGHT,_("Set board to right"));
        menu->Append(ID_BOARD_SET_LEFT,_("Set board to left"));
        menu->AppendSeparator();
        menu->Append(ID_BOARD_MOVE_RIGHT,_("Move board to right"));
        menu->Append(ID_BOARD_MOVE_LEFT,_("Move board to left"));
        menu->Append(ID_BOARD_IMPORT,_("Import boards from file"));
        menu->Append(ID_BOARD_SAVE,_("Save boards to file"));
        menu_bar->Append(menu, _("Board"));
    }
    {
        //Functions
        wxMenu *menu = new wxMenu();
        menu->Append(ID_ROTATE,_("Rotate\tCtrl+R"));
        menu->AppendSeparator();
        menu->Append(ID_HMIRROR,_("Mirror horisontal\tCtrl+H"));
        menu->Append(ID_VMIRROR,_("Mirror vertical\tCtrl+T"));
        menu->AppendSeparator();
        menu->Append(ID_GROUP,_("Build group\tCtrl+G"));
        menu->Append(ID_UNGROUP,_("Split group\tCtrl+U"));
        menu->AppendSeparator();
        menu->Append(ID_CHANGE_SIDE,_("Change board side\tCtrl+W"));
        menu->Append(wxID_ANY,_("Set to layer"));
        menu->AppendSeparator();
        menu->Append(ID_SNAP_GRID,_("Snap to grid"));
        menu->Append(ID_MASSIVE,_("Tile / Arrange circular"));
        menu_bar->Append(menu, _("Functions"));
    }
    {
        //Extras
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_INFO,_("Project info"));
        menu->Append(ID_LIST_DRILLINGS,_("List drillings"));
        menu->AppendSeparator();
        menu->Append(ID_SCANNED_COPY,_("Scanned copy"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Footprint-Wizard"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Reset solder mask"));
        menu->Append(wxID_ANY,_("Remove connections (rubberbands)"));
        menu->Append(wxID_ANY,_("Delete elements outside the board"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Text-IO: Import elements"));
        menu->Append(wxID_ANY,_("Text-IO: Export elements"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Define plugin"));
        menu->Append(wxID_ANY,_("Run plugin"));
        menu_bar->Append(menu, _("Extras"));
    }
    {
        //Options
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_PROPERTIES,_("General settings"));
        menu->AppendSeparator();
        menu->Append(wxID_ANY,_("Macro-Library"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("Properties-Panel"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("DRC-Panel"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("Components-Panel"), wxEmptyString, wxITEM_CHECK);
        menu->Append(wxID_ANY,_("Selector-Panel"));
        menu->AppendSeparator();
        menu->Append(wxID_ZOOM_FIT,_("Zoom board"));
        menu->Append(wxID_ANY,_("Zoom objects"));
        menu->Append(wxID_ANY,_("Zoom selection"));
        menu->Append(wxID_ANY,_("Zoom previous"));
        menu_bar->Append(menu, _("Options"));
    }
    {
        //Help
        wxMenu *menu = new wxMenu();
        menu->Append(wxID_ABOUT,_("About"));
        menu_bar->Append(menu, _("Help"));
    }
    SetMenuBar(menu_bar);
}
void OpenLayoutFrame::init_tool_bar(){
	wxToolBar *tool_bar = new wxToolBar(this,wxID_ANY);
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
    tool_bar->AddTool(ID_ROTATE, _("Rotate"), rotate_xpm);
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
void OpenLayoutFrame::init_left_panel(wxBoxSizer *content){
	wxScrolledWindow *left_panel = new wxScrolledWindow(this,wxID_ANY,
		wxDefaultPosition,wxDefaultSize,wxVSCROLL);
	{
		wxBoxSizer *left_box=new wxBoxSizer(wxVERTICAL);
		{
			wxBoxSizer *tools=new wxBoxSizer(wxVERTICAL);
			const char**images[]={
				tool_edit_xpm,
				tool_zoom_xpm,
				tool_track_xpm,
				tool_pad_xpm,
				tool_pad_smd_xpm,
				tool_circle_xpm,
				tool_rectangle_xpm,
				tool_polygon_xpm,
				tool_special_xpm,
				tool_text_xpm,
				tool_mask_xpm,
				tool_connections_xpm,
				tool_autoroute_xpm,
				tool_test_xpm,
				tool_measure_xpm,
				tool_photoview_xpm
			};
			for(int q=0;q<TOOL_COUNT;q++){
				wxButton *button=new wxButton(left_panel,ID_TOOL_EDIT+q,
					Settings::tool_names[q],wxDefaultPosition,wxDefaultSize,wxNO_BORDER|wxBU_LEFT);
				button->SetBitmap(images[q],wxLEFT);
				button->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e){
					tool=e.GetId()-ID_TOOL_EDIT;
				});
				tools->Add(button,1,wxEXPAND);
			}
			left_box->Add(tools);
		}
		{
			grid_button=new wxButton(left_panel,wxID_ANY,
				"1.27 mm",wxDefaultPosition,wxDefaultSize,wxNO_BORDER|wxBU_LEFT);
			grid_button->SetBitmap(grid_xpm,wxLEFT);
			left_box->Add(grid_button,0,wxEXPAND,5);
		}
		{
			wxFlexGridSizer *sizer=new wxFlexGridSizer(3,2,1,1);
			wxSize size(65,-1);
			{
				wxBitmapButton *track=new wxBitmapButton(left_panel,wxID_ANY,
					track_xpm,wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
				sizer->Add(track,0,wxEXPAND);
				w_track_size=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
					wxDefaultPosition,size,wxSP_ARROW_KEYS,0,99.99,track_size,0.05);
				sizer->Add(w_track_size,0,wxSHAPED);
			}
			{
				wxBitmapButton *pad=new wxBitmapButton(left_panel,wxID_ANY,
					pad_xpm,wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
				pad->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e){PopupMenu(pad_menu);});
				sizer->Add(pad,0,wxEXPAND);
				{
					wxBoxSizer *sizer1=new wxBoxSizer(wxVERTICAL);
					w_pad_size1=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
						wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,pad_size.radius1,0.05);
					w_pad_size2=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
						wxDefaultPosition,size,wxSP_ARROW_KEYS,0,99.99,pad_size.radius2,0.05);
					sizer1->Add(w_pad_size1, 0,wxSHAPED);
					sizer1->Add(w_pad_size2, 0,wxSHAPED);
					sizer->Add(sizer1);
				}
			}
			{
				wxBitmapButton *smd=new wxBitmapButton(left_panel,wxID_ANY,
					smd_xpm,wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
				smd->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e){PopupMenu(smd_menu);});
				sizer->Add(smd,0,wxEXPAND);
				{
					wxBoxSizer *sizer1=new wxBoxSizer(wxVERTICAL);
					w_smd_w=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
						wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,smd_size.width,0.05);
					w_smd_h=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
						wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,smd_size.height,0.05);
					wxBitmapButton *swap_smd=new wxBitmapButton(left_panel,wxID_ANY,
						swap_smd_xpm,wxDefaultPosition,wxDefaultSize,wxNO_BORDER);
					swap_smd->Bind(wxEVT_BUTTON,&OpenLayoutFrame::swap_smd_size,this);
					sizer1->Add(w_smd_w,0,wxSHAPED);
					sizer1->Add(w_smd_h,0,wxSHAPED);
					sizer1->Add(swap_smd,0,wxEXPAND);
					sizer->Add(sizer1);
				}
			}

			left_box->Add(sizer);

		}
		left_panel->SetSizerAndFit(left_box);
		left_panel->SetScrollRate(10, 10);
		left_panel->SetVirtualSize(left_box->GetSize());
	}
	content->Add(left_panel);
}
void OpenLayoutFrame::swap_smd_size(wxCommandEvent&e){
	swap(smd_size.width,smd_size.height);
	w_smd_w->SetValue(smd_size.width);
	w_smd_h->SetValue(smd_size.height);
	build_smd_menu();
}
void OpenLayoutFrame::build_pad_menu(){
	if(pad_menu)delete pad_menu;
	pad_menu=new wxMenu();
	/*for(int q=0;q<s.pad_sizes.size();q++){
		PadSize size=s.pad_sizes[q];
		char text[128];
		sprintf(text,"%.2f X %.2f %s",size.radius1,size.radius2,"mm");
		wxMenuItem *item=new wxMenuItem(pad_menu,wxID_ANY,text);
		if(pad_size.radius1==size.radius1&&pad_size.radius2==size.radius2)
			item->SetBitmap(check_xpm);
		pad_menu->Append(item);
	}*/
}
void add_submenu(wxMenu *parent,wxMenu *child,const wxString &name,const wxBitmap &bitmap){
	wxMenuItem *item=new wxMenuItem(parent,wxID_ANY,_("Remove..."));
	item->SetBitmap(bitmap);
	item->SetSubMenu(child);
	parent->Append(item);
}
void OpenLayoutFrame::build_smd_menu(){
	if(smd_menu)delete smd_menu;
	smd_menu=new wxMenu();
	for(int q=0;q<s.smd_sizes.size();q++){
		SMDSize size=s.smd_sizes[q];
		char text[128];
		sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
		wxMenuItem *item=new wxMenuItem(smd_menu,wxID_ANY,text);
		if(smd_size.normal()==size)
			item->SetBitmap(check_xpm);
		smd_menu->Append(item);
	}
	smd_menu->AppendSeparator();
	{
		char text[128];
		SMDSize size=smd_size.normal();
		sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
		wxMenuItem *item=new wxMenuItem(smd_menu,wxID_ANY,text);
		item->SetBitmap(plus_xpm);
		smd_menu->Append(item);
	}
	{
		smd_menu->AppendSeparator();
		wxMenu *del_menu=new wxMenu();
		{
			for(int q=0;q<s.smd_sizes.size();q++){
				SMDSize size=s.smd_sizes[q];
				char text[128];
				sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
				wxMenuItem *item=new wxMenuItem(del_menu,wxID_ANY,text);

				struct DelUserData : wxObject{DelUserData(uint8_t d){data=d;}; uint8_t data;};

				/*item->Bind(wxEVT_MENU,[&](wxCommandEvent&e){

					build_smd_menu();
				},wxID_ANY,wxID_ANY);*/
				item->SetBitmap(cross_xpm);
				del_menu->Append(item);
			}
		}
		add_submenu(smd_menu,del_menu,_("Remove..."),cross_xpm);
	}
}
