#include "LeftPanel.h"
#include <wx/toolbar.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <algorithm>
#define LEFT_PANEL_XPM
#include "images.h"
#undef LEFT_PANEL_XPM
#include "InputGridDialog.h"
#include "GridBinderDialog.h"
#include "OpenLayoutApp.h"

enum class Tool {
	Edit,
	Zoom,
	Track,
	Pad,
	SMDPad,
	Circle,
	Rectangle,
	Zone,
	SpecialForm,
	Text,
	Connection,
	Autoroute,
	Test,
	Measure,
	Photoview,
	SolderMask
};

enum {
	ID_TOOL_EDIT,
	ID_TOOL_ZOOM,
	ID_TOOL_TRACK,
	ID_TOOL_PAD,
	ID_TOOL_SMD_PAD,
	ID_TOOL_CIRCLE,
	ID_TOOL_RECTANGLE,
	ID_TOOL_ZONE,
	ID_TOOL_FORM,
	ID_TOOL_TEXT,
	ID_TOOL_SOLDER_MASK,
	ID_TOOL_CONNECTIONS,
	ID_TOOL_AUTOROUTE,
	ID_TOOL_TEST,
	ID_TOOL_MEASURE,
	ID_TOOL_PHOTOVIEW,

	ID_TRACK_SEL,
	ID_TRACK_ADD=ID_TRACK_SEL+100,
	ID_TRACK_DEL,
	ID_PAD_SEL=ID_TRACK_DEL+100,
	ID_PAD_ADD=ID_PAD_SEL+100,
	ID_PAD_DEL,
	ID_SMD_SEL=ID_PAD_DEL+100,
	ID_SMD_ADD=ID_SMD_SEL+100,
	ID_SMD_DEL,

	ID_GRID_NORMAL,
	ID_GRID_METRIC=ID_GRID_NORMAL+8,
	ID_GRID_USER=ID_GRID_METRIC+11,
	ID_GRID_USER_DEL=ID_GRID_USER+100,
	ID_GRID_USER_NEW=ID_GRID_USER_DEL+100,

	ID_GRID_HOTKEYS,

	ID_GRID_LINES,
	ID_GRID_DOTS,

	ID_SUBGRID_OFF,
	ID_SUBGRID_2,
	ID_SUBGRID_4,
	ID_SUBGRID_5,
	ID_SUBGRID_10,

	ID_SHOW_GRID,
};

LeftPanel::LeftPanel(wxWindow *parent):
	wxScrolledWindow(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxVSCROLL) {
	wxBoxSizer *left_box=new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer *hsizer=new wxBoxSizer(wxHORIZONTAL);
		{
			wxToolBar *tools=new wxToolBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
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
			const char *tooltips[]= {
				_("Edit mode to select. move. copy. cut, paste or delete elements"),
				_("Zoom mode"),
				_("Draw tracks"),
				_("Add pads, click on the arrow to select a pad shape"),
				_("Add SMD-Pads"),
				_("Draw circles or arcs"),
				_("Draw rectangles"),
				_("Draw filled areas"),
				_("Wizard for equilateral polygons, apirals, etc."),
				_("Add text label"),
				_("Edit solder mask, include or exclude elements from solder mask"),
				_("Draw rubberbands to define connections"),
				_("Autorouter for routing rubberband connections"),
				_("Test mode to check connected elements"),
				_("Measure mode to distance and angles"),
				_("Display the board as WYSIWYG"),
			};
			for(int q=0; q<TOOL_COUNT; q++)
				tools->AddRadioTool(ID_TOOL_EDIT+q,wxEmptyString,images[q],wxNullBitmap,tooltips[q]);
			Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
				APP.selected_tool=e.GetId()-ID_TOOL_EDIT;
			},ID_TOOL_EDIT,ID_TOOL_PHOTOVIEW);
			tools->Realize();
			hsizer->Add(tools,0,wxEXPAND|wxALL);
		}
		{
			wxBoxSizer *vsizer=new wxBoxSizer(wxVERTICAL);
			for(int q=0; q<TOOL_COUNT; q++)
				vsizer->Add(new wxStaticText(this,wxID_ANY,Settings::tool_names[q]),1,wxEXPAND);
			hsizer->Add(vsizer,1,wxEXPAND|wxUP,8);
		}
		left_box->Add(hsizer,0,wxEXPAND);
	}
	{
		grid_button=new wxButton(this,wxID_ANY,
								 "1.27 mm",wxDefaultPosition,wxDefaultSize,wxBU_LEFT);
		grid_button->SetBitmap(grid_xpm,wxLEFT);
		grid_button->SetToolTip(_("Snap to grid"));
		grid_button->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
			build_grid_menu();
		});
		left_box->Add(grid_button,0,wxEXPAND|wxLEFT|wxRIGHT,5);
	}
	{
		wxFlexGridSizer *sizer=new wxFlexGridSizer(3,2,1,1);
		wxSize size(70,-1);
		{
			wxBitmapButton *track=new wxBitmapButton(this,wxID_ANY,
					track_xpm);
			track->SetToolTip(_("Favorite list for Tracks"));
			track->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
				build_track_menu();
			});
			sizer->Add(track,0,wxEXPAND|wxLEFT|wxRIGHT,5);
			w_track_size=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,
											  wxDefaultPosition,size,wxSP_ARROW_KEYS,0,99.99,APP.track_size,0.05);
			w_track_size->SetToolTip(_("Track width"));
			w_track_size->Bind(wxEVT_SPINCTRLDOUBLE,[&](wxSpinDoubleEvent&e) {
				set_track_size(e.GetValue());
			});
			sizer->Add(w_track_size,0,wxEXPAND|wxLEFT|wxRIGHT,5);
		}
		{
			wxBitmapButton *pad=new wxBitmapButton(this,wxID_ANY,
												   pad_xpm);
			pad->SetToolTip(_("Favorite list for Pads"));
			pad->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
				build_pad_menu();
			});
			sizer->Add(pad,0,wxEXPAND|wxLEFT|wxRIGHT,5);
			{
				wxBoxSizer *sizer1=new wxBoxSizer(wxVERTICAL);
				auto func=[&](wxSpinDoubleEvent&) {
					set_pad_size({w_pad_outer->GetValue(),w_pad_inner->GetValue()});
				};
				w_pad_outer=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,
												 wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,APP.pad_size.outer,0.05);
				w_pad_inner=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,
												 wxDefaultPosition,size,wxSP_ARROW_KEYS,0,99.99,APP.pad_size.inner,0.05);
				w_pad_outer->SetToolTip(_("Pad outside diameter"));
				w_pad_inner->SetToolTip(_("Pad drilling diameter"));
				w_pad_outer->Bind(wxEVT_SPINCTRLDOUBLE,func);
				w_pad_inner->Bind(wxEVT_SPINCTRLDOUBLE,func);
				sizer1->Add(w_pad_outer, 0,wxEXPAND|wxLEFT|wxRIGHT,5);
				sizer1->Add(w_pad_inner, 0,wxEXPAND|wxLEFT|wxRIGHT,5);
				sizer->Add(sizer1);
			}
		}
		{
			wxBitmapButton *smd=new wxBitmapButton(this,wxID_ANY,
												   smd_xpm);
			smd->SetToolTip(_("Favorite list for SMD-Pads"));
			smd->Bind(wxEVT_BUTTON,[&](wxCommandEvent&e) {
				build_smd_menu();
			});
			sizer->Add(smd,0,wxEXPAND|wxLEFT|wxRIGHT,5);
			{
				wxBoxSizer *sizer1=new wxBoxSizer(wxVERTICAL);
				auto func=[&](wxSpinDoubleEvent&) {
					set_smd_size({w_smd_w->GetValue(),w_smd_h->GetValue()});
				};
				w_smd_w=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,
											 wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,APP.smd_size.width,0.05);
				w_smd_h=new wxSpinCtrlDouble(this,wxID_ANY,wxEmptyString,
											 wxDefaultPosition,size,wxSP_ARROW_KEYS,0.05,99.99,APP.smd_size.height,0.05);
				w_smd_w->SetToolTip(_("SMD-Pad size"));
				w_smd_h->SetToolTip(_("SMD-Pad size"));
				w_smd_w->Bind(wxEVT_SPINCTRLDOUBLE,func);
				w_smd_h->Bind(wxEVT_SPINCTRLDOUBLE,func);
				wxBitmapButton *swap_smd=new wxBitmapButton(this,wxID_ANY,
						swap_smd_xpm);
				swap_smd->SetToolTip(_("Swap smd sizes"));
				swap_smd->Bind(wxEVT_BUTTON,&LeftPanel::swap_smd_size,this);
				sizer1->Add(w_smd_w,0,wxEXPAND|wxLEFT|wxRIGHT,5);
				sizer1->Add(w_smd_h,0,wxEXPAND|wxLEFT|wxRIGHT,5);
				sizer1->Add(swap_smd,0,wxEXPAND|wxLEFT|wxRIGHT,5);
				sizer->Add(sizer1);
			}
		}

		left_box->Add(sizer,0,wxEXPAND);

	}
	SetSizerAndFit(left_box);
	SetScrollRate(10, 10);
	SetVirtualSize(left_box->GetSize());
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
void LeftPanel::swap_smd_size(wxCommandEvent&e) {
	swap(APP.smd_size.width,APP.smd_size.height);
	w_smd_w->SetValue(APP.smd_size.width);
	w_smd_h->SetValue(APP.smd_size.height);
}
void LeftPanel::set_pad_size(Vec2 size) {
	w_pad_outer->SetValue(size.outer);
	w_pad_inner->SetValue(size.inner);
	APP.pad_size=size;
}
void LeftPanel::set_smd_size(Vec2 size) {
	w_smd_w->SetValue(size.width);
	w_smd_h->SetValue(size.height);
	APP.smd_size=size;
}
void LeftPanel::set_track_size(float size) {
	w_track_size->SetValue(size);
	APP.track_size=size;
}

void LeftPanel::build_smd_menu() {
	wxMenu *smd_menu=new wxMenu();
	sort(SETTINGS.smd_sizes.begin(),SETTINGS.smd_sizes.end());
	bool sel=false;
	{
		for(int q=0; q<SETTINGS.smd_sizes.size(); q++) {
			Vec2 size=SETTINGS.smd_sizes[q];
			char text[128];
			sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
			if(APP.smd_size==size)sel=true;
			smd_menu->AppendCheckItem(ID_SMD_SEL+q,text,wxEmptyString)->Check(APP.smd_size==size);
		}
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			int id=e.GetId()-ID_SMD_SEL;
			set_smd_size(SETTINGS.smd_sizes[id]);
		},ID_SMD_SEL,ID_SMD_SEL+99);
	}
	smd_menu->AppendSeparator();
	{
		char text[128];
		sprintf(text,"%.2f X %.2f %s",APP.smd_size.width,APP.smd_size.height,"mm");
		add_item(smd_menu,text,ID_SMD_ADD,plus_xpm,!sel);
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			SETTINGS.smd_sizes.push_back(APP.smd_size);
		},ID_SMD_ADD);
	}
	{
		smd_menu->AppendSeparator();
		wxMenu *del_menu=new wxMenu();
		{
			for(int q=0; q<SETTINGS.smd_sizes.size(); q++) {
				Vec2 size=SETTINGS.smd_sizes[q];
				char text[128];
				sprintf(text,"%.2f X %.2f %s",size.width,size.height,"mm");
				del_menu->Append(ID_SMD_DEL+q,text);
			}
			Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
				int id=e.GetId()-ID_SMD_DEL;
				SETTINGS.smd_sizes.erase(SETTINGS.smd_sizes.begin()+id);
			},ID_SMD_DEL,ID_SMD_DEL+99);
		}
		add_submenu(smd_menu,del_menu,_("Remove..."),cross_xpm,SETTINGS.smd_sizes.size()!=0);
	}
	PopupMenu(smd_menu);
	delete smd_menu;
}
void LeftPanel::build_pad_menu() {
	wxMenu *pad_menu=new wxMenu();
	sort(SETTINGS.pad_sizes.begin(),SETTINGS.pad_sizes.end());
	bool sel=false;
	{
		for(int q=0; q<SETTINGS.pad_sizes.size(); q++) {
			Vec2 size=SETTINGS.pad_sizes[q];
			char text[128];
			sprintf(text,"%.2f X %.2f %s",size.outer,size.inner,"mm");
			if(APP.pad_size==size) {
				add_check_item(pad_menu,text,ID_PAD_SEL+q,true);
				sel=true;
			} else
				add_check_item(pad_menu,text,ID_PAD_SEL+q,false);
		}
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			int id=e.GetId()-ID_PAD_SEL;
			set_pad_size(SETTINGS.pad_sizes[id]);
		},ID_PAD_SEL,ID_PAD_SEL+99);
	}
	pad_menu->AppendSeparator();
	{
		char text[128];
		sprintf(text,"%.2f X %.2f %s",APP.pad_size.outer,APP.pad_size.inner,"mm");
		add_item(pad_menu,text,ID_PAD_ADD,plus_xpm,!sel);
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			SETTINGS.pad_sizes.push_back(APP.pad_size);
		},ID_PAD_ADD);
	}
	{
		pad_menu->AppendSeparator();
		wxMenu *del_menu=new wxMenu();
		{
			for(int q=0; q<SETTINGS.pad_sizes.size(); q++) {
				Vec2 size=SETTINGS.pad_sizes[q];
				char text[128];
				sprintf(text,"%.2f X %.2f %s",size.outer,size.inner,"mm");
				add_item(del_menu,text,ID_PAD_DEL+q,cross_xpm);
			}
			Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
				int id=e.GetId()-ID_PAD_DEL;
				SETTINGS.pad_sizes.erase(SETTINGS.pad_sizes.begin()+id);
			},ID_PAD_DEL,ID_PAD_DEL+99);
		}
		add_submenu(pad_menu,del_menu,_("Remove..."),cross_xpm,SETTINGS.pad_sizes.size()!=0);
	}
	PopupMenu(pad_menu);
	delete pad_menu;
}
void LeftPanel::build_track_menu() {
	wxMenu *track_menu=new wxMenu();
	sort(SETTINGS.track_sizes.begin(),SETTINGS.track_sizes.end());
	bool sel=false;
	{
		for(int q=0; q<SETTINGS.track_sizes.size(); q++) {
			float size=SETTINGS.track_sizes[q];
			char text[128];
			sprintf(text,"%.2f %s",size,"mm");
			if(APP.track_size==size) {
				add_check_item(track_menu,text,ID_TRACK_SEL+q,true);
				sel=true;
			} else
				add_check_item(track_menu,text,ID_TRACK_SEL+q,false);
		}
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			int id=e.GetId()-ID_TRACK_SEL;
			set_track_size(SETTINGS.track_sizes[id]);
		},ID_TRACK_SEL,ID_TRACK_SEL+99);
	}
	track_menu->AppendSeparator();
	{
		char text[128];
		sprintf(text,"%.2f %s",APP.track_size,"mm");
		add_item(track_menu,text,ID_TRACK_ADD,plus_xpm,!sel);
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			SETTINGS.track_sizes.push_back(APP.track_size);
		},ID_TRACK_ADD);
	}
	{
		track_menu->AppendSeparator();
		wxMenu *del_menu=new wxMenu();
		{
			for(int q=0; q<SETTINGS.track_sizes.size(); q++) {
				float size=SETTINGS.track_sizes[q];
				char text[128];
				sprintf(text,"%.2f %s",size,"mm");
				add_item(del_menu,text,ID_TRACK_DEL+q,cross_xpm);
			}
			Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
				int id=e.GetId()-ID_TRACK_DEL;
				SETTINGS.track_sizes.erase(SETTINGS.track_sizes.begin()+id);
			},ID_TRACK_DEL,ID_TRACK_DEL+99);
		}
		add_submenu(track_menu,del_menu,_("Remove..."),cross_xpm,SETTINGS.track_sizes.size()!=0);
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
void LeftPanel::build_grid_menu() {
	wxMenu *grid_menu=new wxMenu();
	for(int item=0; item<8; item++) {
		float grid=get_normal_grid(item);
		add_check_item(grid_menu,get_grid_str(grid),ID_GRID_NORMAL+item,BOARD.active_grid_val==grid);
	}
	grid_menu->AppendSeparator();
	{
		wxMenu *metric=new wxMenu();
		for(int item=0; item<11; item++) {
			float grid=get_metric_grid(item);
			add_check_item(metric,get_grid_str(grid),ID_GRID_METRIC+item,BOARD.active_grid_val==grid);
		}
		add_submenu(grid_menu,metric,_("Metric grids:"));
	}
	grid_menu->AppendSeparator();
	{
		wxMenu *user=new wxMenu();
		for(int item=0; item<SETTINGS.grids.size(); item++) {
			float grid=SETTINGS.grids[item];
			add_check_item(user,get_grid_str(grid),ID_GRID_USER+item,BOARD.active_grid_val==grid);
		}
		if(SETTINGS.grids.size())
			user->AppendSeparator();
		user->Append(ID_GRID_USER_NEW,_("Add new grid value"));
		Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
			InputGridDialog dialog(this);
			if(dialog.ShowModal()==wxID_OK) {
				float grid=dialog.Get();
				SETTINGS.grids.push_back(grid);
				sort(SETTINGS.grids.begin(),SETTINGS.grids.end());
				set_grid(grid);
			}
		},ID_GRID_USER_NEW);
		if(SETTINGS.grids.size()) {
			wxMenu *user_del=new wxMenu();
			for(int item=0; item<SETTINGS.grids.size(); item++) {
				float grid=SETTINGS.grids[item];
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
			grid=SETTINGS.grids[id-19];
		set_grid(grid);
	},ID_GRID_NORMAL,ID_GRID_NORMAL+118);
	Bind(wxEVT_MENU,[&](wxCommandEvent &e) {
		int id=e.GetId()-ID_GRID_USER_DEL;
		SETTINGS.grids.erase(SETTINGS.grids.begin()+id);
	},ID_GRID_USER_DEL,ID_GRID_USER_DEL+99);
	grid_menu->AppendSeparator();
	grid_menu->Append(ID_GRID_HOTKEYS,_("Hotkeys..."));
	Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
		ShowGridBinderDialog(this,SETTINGS.grids_bind);
	},ID_GRID_HOTKEYS);
	{
		wxMenu *style=new wxMenu;
		style->AppendRadioItem(ID_GRID_LINES,_("Lines"));
		style->AppendRadioItem(ID_GRID_DOTS,_("Dots"));
		if(SETTINGS.grid_style==GRID_DOTS)
			style->Check(ID_GRID_DOTS,true);
		else
			style->Check(ID_GRID_LINES,true);
		Bind(wxEVT_MENU,[&](wxCommandEvent &e) {
			if(e.GetId()==ID_GRID_DOTS)
				SETTINGS.grid_style=GRID_DOTS;
			else
				SETTINGS.grid_style=GRID_LINES;
		},ID_GRID_LINES,ID_GRID_DOTS);
		grid_menu->Append(wxID_ANY,_("Grid style"),style);
	}
	{
		wxMenu *sub=new wxMenu;
		sub->AppendRadioItem(ID_SUBGRID_OFF,_("Off"));
		sub->AppendRadioItem(ID_SUBGRID_2,"2");
		sub->AppendRadioItem(ID_SUBGRID_4,"4");
		sub->AppendRadioItem(ID_SUBGRID_5,"5");
		sub->AppendRadioItem(ID_SUBGRID_10,"10");

		sub->Check(ID_SUBGRID_OFF+SETTINGS.sub_grid,true); //enable item
		sub->Bind(wxEVT_MENU,[&](wxCommandEvent &e) {
			SETTINGS.sub_grid=e.GetId()-ID_SUBGRID_OFF;
		},ID_SUBGRID_OFF,ID_SUBGRID_10);
		grid_menu->Append(wxID_ANY,_("Subdivisions"),sub);
	}
	grid_menu->AppendCheckItem(ID_SHOW_GRID,_("Show grid"))->Check(SETTINGS.show_grid);
	Bind(wxEVT_MENU,[&](wxCommandEvent&e) {
		SETTINGS.show_grid=!SETTINGS.show_grid;
	},ID_SHOW_GRID);
	PopupMenu(grid_menu);
	delete grid_menu;
}
void LeftPanel::set_grid(float val) {
	BOARD.active_grid_val=val*10000.0;
	grid_button->SetLabel(get_grid_str(val));
}
