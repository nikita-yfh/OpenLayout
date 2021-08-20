/***************************************************************
 * Name:      OpenLayoutMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:
 **************************************************************/
#include "ScannedCopyDialog.h"
#include "InputNumberDialog.h"
#include "OpenLayoutMain.h"
#include "NewBoardDialog.h"
#include "LayerInfoDialog.h"
#include "ProjectInfoDialog.h"
#include "InputGridDialog.h"
#include "GridBinderDialog.h"
#include "SettingsDialog.h"
#include "AboutDialog.h"
#include "GLDraw.h"
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/artprov.h>

#define MAIN_XPM
#include "images.h"
#undef MAIN_XPM

#include <string>
#include <sstream>
#include <algorithm>
#include <wx/time.h>

#include "Utils.h"
using namespace std;

template<typename T>
struct Userdata :wxObject {
    Userdata(T v) {
        value=v;
    }
    T value;
};
OpenLayoutFrame::OpenLayoutFrame()
    :wxFrame(0,wxID_ANY,"OpenLayout") {
    wxBoxSizer *all_box=new wxBoxSizer(wxVERTICAL);
    init_menu_bar();
    init_tool_bar(all_box);
    {
        wxBoxSizer *content=new wxBoxSizer(wxHORIZONTAL);
        init_left_panel(content);
        {
        	GLCanvas *canvas=new GLCanvas(this);
        	content->Add(canvas,1,wxEXPAND|wxALL,5);
        }
        all_box->Add(content,1,wxEXPAND);
        wxBoxSizer *bottom_panel=new wxBoxSizer(wxHORIZONTAL);
    }
    SetSizer(all_box);
    SetAutoLayout(true);
    Bind(wxEVT_MENU,&OpenLayoutFrame::close,this, wxID_EXIT);
    Bind(wxEVT_MENU,&OpenLayoutFrame::show_settings,this,wxID_PROPERTIES);
    Bind(wxEVT_MENU,&OpenLayoutFrame::show_about,this,wxID_ABOUT);
    Bind(wxEVT_MENU,&OpenLayoutFrame::show_project_info,this,wxID_INFO);
    Bind(wxEVT_MENU,&OpenLayoutFrame::new_board,this,ID_BOARD_NEW);
    Bind(wxEVT_MENU,&OpenLayoutFrame::show_scan_properties,this,ID_SCANNED_COPY);

    file.load("./1.lay6");
    file.save("./2.lay6");
}

void OpenLayoutFrame::close(wxCommandEvent&) {
    Close(true);
}
void OpenLayoutFrame::show_settings(wxCommandEvent&) {
    SettingsDialog settings(this,s);
    if(settings.ShowModal()==wxID_OK)
        settings.Get(s);
}
void OpenLayoutFrame::show_about(wxCommandEvent&) {
    AboutDialog(this).ShowModal();
}
void OpenLayoutFrame::show_project_info(wxCommandEvent&) {
    ProjectInfoDialog dialog(this,file.info);
    if(dialog.ShowModal()==wxID_OK)
        dialog.Get(file.info);
}
void OpenLayoutFrame::show_scan_properties(wxCommandEvent&) {
    ScannedCopyDialog dialog(this,file.GetSelectedBoard().images,s);
    if(dialog.ShowModal()==wxID_OK)
        dialog.Get(file.GetSelectedBoard().images);
}
void OpenLayoutFrame::new_board(wxCommandEvent&) {
    NewBoardDialog dialog(this);
    while(dialog.ShowModal()==wxID_OK) {
        if(dialog.isValid()) {
            file.AddBoard(dialog.build());
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
        menu->Append(wxID_ANY,_("&Set to layer"));
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
    {//create menu for rotate button
    	wxMenu *menu=new wxMenu();
		menu->AppendRadioItem(ID_ANGLE_90,"90");
		menu->AppendRadioItem(ID_ANGLE_45,"45");
		menu->AppendRadioItem(ID_ANGLE_10,"10");
		menu->AppendRadioItem(ID_ANGLE_5,"5");
		Bind(wxEVT_MENU,[&](wxCommandEvent&e){
			switch(e.GetId()){
				case ID_ANGLE_90: s.rotation_angle=90.0f; break;
				case ID_ANGLE_45: s.rotation_angle=45.0f; break;
				case ID_ANGLE_10: s.rotation_angle=10.0f; break;
				case ID_ANGLE_5:  s.rotation_angle=5.0f;  break;
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
void OpenLayoutFrame::init_left_panel(wxBoxSizer *content) {
    wxScrolledWindow *left_panel = new wxScrolledWindow(this,wxID_ANY,
            wxDefaultPosition,wxDefaultSize,wxVSCROLL);
    {
        wxBoxSizer *left_box=new wxBoxSizer(wxVERTICAL);
		{
			wxBoxSizer *hsizer=new wxBoxSizer(wxHORIZONTAL);
			{
				wxToolBar *tools=new wxToolBar(left_panel,wxID_ANY,wxDefaultPosition,wxDefaultSize,
							wxTB_VERTICAL|wxTB_FLAT|wxTB_TEXT|wxTB_HORZ_TEXT|wxTB_HORZ_LAYOUT|wxTB_NOALIGN|wxTB_NODIVIDER);
				const char**images[]= {
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
				for(int q=0; q<TOOL_COUNT; q++) {
					tools->AddRadioTool(ID_TOOL_EDIT+q,wxEmptyString,images[q]);
				}
				tools->Realize();
				hsizer->Add(tools,0,wxEXPAND|wxALL);
			}
			{
				wxBoxSizer *vsizer=new wxBoxSizer(wxVERTICAL);
				for(int q=0; q<TOOL_COUNT; q++)
					vsizer->Add(new wxStaticText(left_panel,wxID_ANY,Settings::tool_names[q]),1,wxEXPAND);
				hsizer->Add(vsizer,1,wxEXPAND|wxUP,8);
			}
			left_box->Add(hsizer,0,wxEXPAND);
		}
        {
            grid_button=new wxButton(left_panel,wxID_ANY,
                                     "1.27 mm",wxDefaultPosition,wxDefaultSize,wxBU_LEFT);
            grid_button->SetBitmap(grid_xpm,wxLEFT);
            grid_button->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
                build_grid_menu();
            });
            left_box->Add(grid_button,0,wxEXPAND|wxLEFT|wxRIGHT,5);
        }
        {
            wxFlexGridSizer *sizer=new wxFlexGridSizer(3,2,1,1);
            wxSize size(70,-1);
            {
                wxBitmapButton *track=new wxBitmapButton(left_panel,wxID_ANY,
                        track_xpm);
                track->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
                    build_track_menu();
                });
                sizer->Add(track,0,wxEXPAND|wxLEFT|wxRIGHT,5);
                w_track_size=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
                                                  wxDefaultPosition,size,wxSP_ARROW_KEYS,0,99.99,track_size,0.05);
                w_track_size->Bind(wxEVT_SPINCTRLDOUBLE,[&](wxSpinDoubleEvent&e) {
                    set_track_size(e.GetValue());
                });
                sizer->Add(w_track_size,0,wxEXPAND|wxLEFT|wxRIGHT,5);
            }
            {
                wxBitmapButton *pad=new wxBitmapButton(left_panel,wxID_ANY,
                                                       pad_xpm);
                pad->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
                    build_pad_menu();
                });
                sizer->Add(pad,0,wxEXPAND|wxLEFT|wxRIGHT,5);
                {
                    wxBoxSizer *sizer1=new wxBoxSizer(wxVERTICAL);
                    auto func=[&](wxSpinDoubleEvent&) {
                        set_pad_size({w_pad_outer->GetValue(),w_pad_inner->GetValue()});
                    };
                    w_pad_outer=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
                                                     wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,pad_size.outer,0.05);
                    w_pad_inner=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
                                                     wxDefaultPosition,size,wxSP_ARROW_KEYS,0,99.99,pad_size.inner,0.05);
                    w_pad_outer->Bind(wxEVT_SPINCTRLDOUBLE,func);
                    w_pad_inner->Bind(wxEVT_SPINCTRLDOUBLE,func);
                    sizer1->Add(w_pad_outer, 0,wxEXPAND|wxLEFT|wxRIGHT,5);
                    sizer1->Add(w_pad_inner, 0,wxEXPAND|wxLEFT|wxRIGHT,5);
                    sizer->Add(sizer1);
                }
            }
            {
                wxBitmapButton *smd=new wxBitmapButton(left_panel,wxID_ANY,
                                                       smd_xpm);
                smd->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
                    build_smd_menu();
                });
                sizer->Add(smd,0,wxEXPAND|wxLEFT|wxRIGHT,5);
                {
                    wxBoxSizer *sizer1=new wxBoxSizer(wxVERTICAL);
                    auto func=[&](wxSpinDoubleEvent&) {
                        set_smd_size({w_smd_w->GetValue(),w_smd_h->GetValue()});
                    };
                    w_smd_w=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
                                                 wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,smd_size.width,0.05);
                    w_smd_h=new wxSpinCtrlDouble(left_panel,wxID_ANY,wxEmptyString,
                                                 wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,smd_size.height,0.05);
                    w_smd_w->Bind(wxEVT_SPINCTRLDOUBLE,func);
                    w_smd_h->Bind(wxEVT_SPINCTRLDOUBLE,func);
                    wxBitmapButton *swap_smd=new wxBitmapButton(left_panel,wxID_ANY,
                            swap_smd_xpm);
                    swap_smd->Bind(wxEVT_BUTTON,&OpenLayoutFrame::swap_smd_size,this);
                    sizer1->Add(w_smd_w,0,wxEXPAND|wxLEFT|wxRIGHT,5);
                    sizer1->Add(w_smd_h,0,wxEXPAND|wxLEFT|wxRIGHT,5);
                    sizer1->Add(swap_smd,0,wxEXPAND|wxLEFT|wxRIGHT,5);
                    sizer->Add(sizer1);
                }
            }

            left_box->Add(sizer,0,wxEXPAND);

        }
        left_panel->SetSizerAndFit(left_box);
        left_panel->SetScrollRate(10, 10);
        left_panel->SetVirtualSize(left_box->GetSize());
    }
    content->Add(left_panel,0,wxEXPAND|wxALL);
}
void OpenLayoutFrame::swap_smd_size(wxCommandEvent&e) {
    swap(smd_size.width,smd_size.height);
    w_smd_w->SetValue(smd_size.width);
    w_smd_h->SetValue(smd_size.height);
}
void OpenLayoutFrame::set_pad_size(Vec2 size) {
    w_pad_outer->SetValue(size.outer);
    w_pad_inner->SetValue(size.inner);
    pad_size=size;
}
void OpenLayoutFrame::set_smd_size(Vec2 size) {
    w_smd_w->SetValue(size.width);
    w_smd_h->SetValue(size.height);
    smd_size=size;
}
void OpenLayoutFrame::set_track_size(float size) {
    w_track_size->SetValue(size);
    track_size=size;
}
void add_submenu(wxMenu *parent,wxMenu *child,const wxString &name,const wxBitmap &bitmap=wxNullBitmap,bool enabled=true) {
    wxMenuItem *item=new wxMenuItem(parent,wxID_ANY,name);
    item->SetBitmap(bitmap);
    item->SetSubMenu(child);
    parent->Append(item);
    item->Enable(enabled);
}
void add_item(wxMenu *parent,const wxString &text, int id, const wxBitmap &bitmap=wxNullBitmap,bool enabled=true) {
    wxMenuItem *item=new wxMenuItem(parent,id,text);
    item->SetBitmap(bitmap);
    parent->Append(item);
    item->Enable(enabled);
}
void add_check_item(wxMenu *parent,const wxString &text, int id,bool enabled=true) {
    wxMenuItem *item=new wxMenuItem(parent,id,text,wxEmptyString,wxITEM_CHECK);
    parent->Append(item);
    item->Check(enabled);
}
void OpenLayoutFrame::build_smd_menu() {
    wxMenu *smd_menu=new wxMenu();
    sort(s.smd_sizes.begin(),s.smd_sizes.end());
    bool sel=false;
    {
        for(int q=0; q<s.smd_sizes.size(); q++) {
            Vec2 size=s.smd_sizes[q];
            char text[128];
            sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
            if(smd_size==size)sel=true;
			smd_menu->AppendCheckItem(ID_SMD_SEL+q,text,wxEmptyString)->Check(smd_size==size);
        }
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            int id=e.GetId()-ID_SMD_SEL;
            set_smd_size(s.smd_sizes[id]);
        },ID_SMD_SEL,ID_SMD_SEL+99);
    }
    smd_menu->AppendSeparator();
    {
        char text[128];
        sprintf(text,"%.2f X %.2f %s",smd_size.width,smd_size.height,"mm");
        add_item(smd_menu,text,ID_SMD_ADD,plus_xpm,!sel);
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            s.smd_sizes.push_back(smd_size);
        },ID_SMD_ADD);
    }
    {
        smd_menu->AppendSeparator();
        wxMenu *del_menu=new wxMenu();
        {
            for(int q=0; q<s.smd_sizes.size(); q++) {
                Vec2 size=s.smd_sizes[q];
                char text[128];
                sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
                del_menu->Append(ID_SMD_DEL+q,text);
            }
            Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
                int id=e.GetId()-ID_SMD_DEL;
                s.smd_sizes.erase(s.smd_sizes.begin()+id);
            },ID_SMD_DEL,ID_SMD_DEL+99);
        }
        add_submenu(smd_menu,del_menu,_("Remove..."),cross_xpm,s.smd_sizes.size()!=0);
    }
    PopupMenu(smd_menu);
    delete smd_menu;
}
void OpenLayoutFrame::build_pad_menu() {
    wxMenu *pad_menu=new wxMenu();
    sort(s.pad_sizes.begin(),s.pad_sizes.end());
    bool sel=false;
    {
        for(int q=0; q<s.pad_sizes.size(); q++) {
            Vec2 size=s.pad_sizes[q];
            char text[128];
            sprintf(text,"%.2f X %.2f %s",size.outer,size.inner,"mm");
            if(pad_size==size) {
                add_check_item(pad_menu,text,ID_PAD_SEL+q,true);
                sel=true;
            } else
                add_check_item(pad_menu,text,ID_PAD_SEL+q,false);
        }
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            int id=e.GetId()-ID_PAD_SEL;
            set_pad_size(s.pad_sizes[id]);
        },ID_PAD_SEL,ID_PAD_SEL+99);
    }
    pad_menu->AppendSeparator();
    {
        char text[128];
        sprintf(text,"%.2f X %.2f %s",pad_size.outer,pad_size.inner,"mm");
        add_item(pad_menu,text,ID_PAD_ADD,plus_xpm,!sel);
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            s.pad_sizes.push_back(pad_size);
        },ID_PAD_ADD);
    }
    {
        pad_menu->AppendSeparator();
        wxMenu *del_menu=new wxMenu();
        {
            for(int q=0; q<s.pad_sizes.size(); q++) {
                Vec2 size=s.pad_sizes[q];
                char text[128];
                sprintf(text,"%.2f X %.2f %s",size.outer,size.inner,"mm");
                add_item(del_menu,text,ID_PAD_DEL+q,cross_xpm);
            }
            Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
                int id=e.GetId()-ID_PAD_DEL;
                s.pad_sizes.erase(s.pad_sizes.begin()+id);
            },ID_PAD_DEL,ID_PAD_DEL+99);
        }
        add_submenu(pad_menu,del_menu,_("Remove..."),cross_xpm,s.pad_sizes.size()!=0);
    }
    PopupMenu(pad_menu);
    delete pad_menu;
}
void OpenLayoutFrame::build_track_menu() {
    wxMenu *track_menu=new wxMenu();
    sort(s.track_sizes.begin(),s.track_sizes.end());
    bool sel=false;
    {
        for(int q=0; q<s.track_sizes.size(); q++) {
            float size=s.track_sizes[q];
            char text[128];
            sprintf(text,"%.2f %s",size,"mm");
            if(track_size==size) {
                add_check_item(track_menu,text,ID_TRACK_SEL+q,true);
                sel=true;
            } else
                add_check_item(track_menu,text,ID_TRACK_SEL+q,false);
        }
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            int id=e.GetId()-ID_TRACK_SEL;
            set_track_size(s.track_sizes[id]);
        },ID_TRACK_SEL,ID_TRACK_SEL+99);
    }
    track_menu->AppendSeparator();
    {
        char text[128];
        sprintf(text,"%.2f %s",track_size,"mm");
        add_item(track_menu,text,ID_TRACK_ADD,plus_xpm,!sel);
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            s.track_sizes.push_back(track_size);
        },ID_TRACK_ADD);
    }
    {
        track_menu->AppendSeparator();
        wxMenu *del_menu=new wxMenu();
        {
            for(int q=0; q<s.track_sizes.size(); q++) {
                float size=s.track_sizes[q];
                char text[128];
                sprintf(text,"%.2f %s",size,"mm");
                add_item(del_menu,text,ID_TRACK_DEL+q,cross_xpm);
            }
            Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
                int id=e.GetId()-ID_TRACK_DEL;
                s.track_sizes.erase(s.track_sizes.begin()+id);
            },ID_TRACK_DEL,ID_TRACK_DEL+99);
        }
        add_submenu(track_menu,del_menu,_("Remove..."),cross_xpm,s.track_sizes.size()!=0);
    }
    PopupMenu(track_menu);
    delete track_menu;
}
float get_normal_grid(int n) {
    return 0.0396875f*pow(2,n);
}
float get_metric_grid(int n) {
    float grids[]= {
        0.01f,	0.02f,	0.025f,	0.05f,
        0.1f,	0.2f,	0.25f,	0.5f,
        1.0f,	2.0f,	2.5f
    };
    return grids[n];
}
void OpenLayoutFrame::build_grid_menu() {
    wxMenu *grid_menu=new wxMenu();
    for(int item=0; item<8; item++) {
        float grid=get_normal_grid(item);
        add_check_item(grid_menu,get_grid_str(grid),ID_GRID_NORMAL+item,file.GetSelectedBoard().active_grid_val==grid);
    }
    grid_menu->AppendSeparator();
    {
        wxMenu *metric=new wxMenu();
        for(int item=0; item<11; item++) {
            float grid=get_metric_grid(item);
			add_check_item(metric,get_grid_str(grid),ID_GRID_METRIC+item,file.GetSelectedBoard().active_grid_val==grid);
        }
        add_submenu(grid_menu,metric,_("Metric grids:"));
    }
    grid_menu->AppendSeparator();
    {
        wxMenu *user=new wxMenu();
        for(int item=0; item<s.grids.size(); item++) {
            float grid=s.grids[item];
			add_check_item(user,get_grid_str(grid),ID_GRID_USER+item,file.GetSelectedBoard().active_grid_val==grid);
        }
        if(s.grids.size())
            user->AppendSeparator();
        user->Append(ID_GRID_USER_NEW,_("Add new grid value"));
        Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
            InputGridDialog dialog(this);
            if(dialog.ShowModal()==wxID_OK) {
                float grid=dialog.Get();
                s.grids.push_back(grid);
                sort(s.grids.begin(),s.grids.end());
                set_grid(grid);
            }
        },ID_GRID_USER_NEW);
        if(s.grids.size()) {
            wxMenu *user_del=new wxMenu();
            for(int item=0; item<s.grids.size(); item++) {
                float grid=s.grids[item];
                add_item(user_del,get_grid_str(grid),ID_GRID_USER_DEL+item,wxNullBitmap);
            }
            add_submenu(user,user_del,_("Remove"),wxNullBitmap);
        }
        add_submenu(grid_menu,user,_("User grids:"));
    }
    Bind(wxEVT_MENU,[&](wxCommandEvent &e) {
        int id=e.GetId()-ID_GRID_NORMAL;
        float grid;
        if(id<8) //normal
            grid=get_normal_grid(id);
        else if(id<19) //metric
            grid=get_metric_grid(id-8);
        else //user grid
            grid=s.grids[id-19];
        set_grid(grid,id<8);
    },ID_GRID_NORMAL,ID_GRID_NORMAL+118);
    Bind(wxEVT_MENU,[&](wxCommandEvent &e) {
        int id=e.GetId()-ID_GRID_USER_DEL;
        s.grids.erase(s.grids.begin()+id);
    },ID_GRID_USER_DEL,ID_GRID_USER_DEL+99);
    grid_menu->AppendSeparator();
    grid_menu->Append(ID_GRID_HOTKEYS,_("Hotkeys..."));
    Bind(wxEVT_MENU,[&](wxCommandEvent&e){
		ShowGridBinderDialog(this,s.grids_bind);
	},ID_GRID_HOTKEYS);
    {
        wxMenu *style=new wxMenu;
        style->AppendRadioItem(ID_GRID_LINES,_("Lines"));
        style->AppendRadioItem(ID_GRID_DOTS,_("Dots"));
        if(s.grid_style==GRID_DOTS)
			style->Check(ID_GRID_DOTS,true);
		else
			style->Check(ID_GRID_LINES,true);
		Bind(wxEVT_MENU,[&](wxCommandEvent &e){
			if(e.GetId()==ID_GRID_DOTS)
				s.grid_style=GRID_DOTS;
			else
				s.grid_style=GRID_LINES;
		},ID_GRID_LINES,ID_GRID_DOTS);
        grid_menu->Append(wxID_ANY,_("Grid style"),style);
    }
    {
        wxMenu *sub=new wxMenu;
        sub->AppendRadioItem(ID_SUBGRID_OFF,_("Off"));
        sub->AppendRadioItem(ID_SUBGRID_2,_("2"));
        sub->AppendRadioItem(ID_SUBGRID_4,_("4"));
        sub->AppendRadioItem(ID_SUBGRID_5,_("5"));
        sub->AppendRadioItem(ID_SUBGRID_10,_("10"));

        sub->Check(ID_SUBGRID_OFF+s.sub_grid,true); //enable item
        sub->Bind(wxEVT_MENU,[&](wxCommandEvent &e){
			s.sub_grid=e.GetId()-ID_SUBGRID_OFF;
		},ID_SUBGRID_OFF,ID_SUBGRID_10);
        grid_menu->Append(wxID_ANY,_("Subdivisions"),sub);
    }
    grid_menu->AppendCheckItem(ID_SHOW_GRID,_("Show grid"))->Check(s.show_grid);
    Bind(wxEVT_MENU,[&](wxCommandEvent&e){
		s.show_grid=!s.show_grid;
	},ID_SHOW_GRID);
    PopupMenu(grid_menu);
    delete grid_menu;
}
void OpenLayoutFrame::set_grid(float val,bool metric) {
    file.GetSelectedBoard().active_grid_val=val;
    grid_button->SetLabel(get_grid_str(val));
}
