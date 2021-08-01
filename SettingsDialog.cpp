#include "SettingsDialog.h"

#include <wx/intl.h>
#include <wx/string.h>

#define WIDGET(type,id)\
	static_cast<type>(FindWindowById(id))


SettingsDialog::SettingsDialog(wxWindow* parent,const Settings &s) {
    Create(parent, wxID_ANY, _("General settings"));

    wxBoxSizer *BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    wxTreebook *Treebook = new wxTreebook(this, wxID_ANY);

    wxPanel *tabs[9];
    for(int q=0;q<9;q++)
		tabs[q] = new wxPanel(Treebook);
	{//PAGE 1


		wxBoxSizer* all_box = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* choices = new wxBoxSizer(wxHORIZONTAL);

		choices->Add(new wxStaticText(tabs[0], wxID_ANY, _("Base unit:")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		units_choice = new wxChoice(tabs[0], wxID_ANY);
		units_choice ->Append(_("mm"));
		units_choice ->Append(_("mil (1/1000 Zoll)"));
		units_choice ->SetSelection((int)s.units);
		choices->Add(units_choice , 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		choices->Add(new wxStaticText(tabs[0], wxID_ANY, _("Drillings:")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		drill_choice = new wxChoice(tabs[0], wxID_ANY);
		drill_choice->Append(_("Background color"));
		drill_choice->Append(_("White"));
		drill_choice->Append(_("Black"));
		drill_choice->SetSelection((int)s.drill);
		choices->Add(drill_choice, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		all_box->Add(choices, 0, wxEXPAND, 5);

		const char *buttons[]={
			_("Show Board-Zoom"),
			_("Darken ground-plane"),
			_("Show ground-plane of all layers simultaneos"),
			_("Consider connections (rubberbands) in TEST mode"),
			_("Blinking TEST mode"),
			_("Double-click takes size parameters of elements"),
			_("Limit text height (track width min. 0.15 mm)"),
			_("Components ID and value always readable after rotating"),
			_("Optimize nodes of a track automatically"),
			_("Origin top/left (instead of bottom/left)"),
			_("Use origin in CAM-export (Gerber/Excellon/HPGL)")
		};

		for(int q=0;q<(int)GenSettings::COUNT;q++){
			checkboxes[q]=new wxCheckBox(tabs[0],wxID_ANY, buttons[q]);
			checkboxes[q]->SetValue(s.gen_settings[q]);
			all_box->Add(checkboxes[q], 0, wxEXPAND, 5);
		}
		tabs[0]->SetSizerAndFit(all_box);
	}
	{//PAGE 2
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
		box->Add(new wxStaticText(tabs[1], wxID_ANY, _("Color scheme:")),0,wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,5);

		colorselection = new wxChoice(tabs[1], wxID_ANY);
		colorselection->Append(_("Standart"));
		colorselection->Append(_("User 1"));
		colorselection->Append(_("User 2"));
		colorselection->Append(_("User 3"));
		colorselection->SetSelection(s.s_color_scheme);


		box->Add(colorselection, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		all_box->Add(box, 0, wxALL|wxEXPAND, 5);
		panel_colors = new wxPanel(tabs[1],wxID_ANY);
		if(s.s_color_scheme==0)
			panel_colors->Disable();

		wxBoxSizer *BoxSizer9 = new wxBoxSizer(wxVERTICAL);
		const int color_ids[]={0,7,1,8,2,9,3,-1,4,10,5,11,6};
		for(int q=0;q<3;q++)
			colors[q]=s.colors[q];
		const char* color_names[]={
			_("C1 (Copper-Top)"),
			_("Background"),
			_("S1 (Silkscreen-Top)"),
			_("Grid-lines"),
			_("C2 (Copper-Bottom)"),
			_("Grid-dots"),
			_("S2 (Silkscreen-Bottom)"),
			nullptr,
			_("I1 (Copper-Inner 1)"),
			_("Connections"),
			_("I2 (Copper-Inner 2)"),
			_("Via"),
			_("O (Outline)")
		};
		wxFlexGridSizer *FlexGridSizer1 = new wxFlexGridSizer(7, 4,0,0);


		auto color_cb=[&](wxCommandEvent& event){
			int selected=colorselection->GetSelection();
			assert(selected);
			GetColorScheme(colors[selected-1]);
		};

		for(int q=0;q<13;q++){
			if(color_names[q]){
				c_colors[color_ids[q]]=new wxColourPickerCtrl(panel_colors,wxID_ANY);
				c_colors[color_ids[q]]->Bind(wxEVT_COLOURPICKER_CHANGED,color_cb);
				FlexGridSizer1->Add(c_colors[color_ids[q]],1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
				FlexGridSizer1->Add(
					new wxStaticText(panel_colors, wxID_ANY, color_names[q]),
					1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			}else{
				for(int q=0;q<2;q++)
					FlexGridSizer1->Add(-1,-1,1, wxEXPAND, 5);
			}
		}
		BoxSizer9->Add(FlexGridSizer1, 0, wxEXPAND, 5);
		wxButton *reset=new wxButton(panel_colors, wxID_ANY, _("Reset scheme to default"));
		BoxSizer9->Add(reset, 0, wxALL|wxALIGN_RIGHT, 5);
		panel_colors->SetSizerAndFit(BoxSizer9);
		all_box->Add(panel_colors, 1, wxALL|wxEXPAND, 5);
		tabs[1]->SetSizerAndFit(all_box);

		if(s.s_color_scheme!=0)
			SetColorScheme(colors[s.s_color_scheme-1]);
		else
			SetColorScheme(ColorScheme());

		colorselection->Bind(wxEVT_CHOICE,[&](wxCommandEvent&event){
			int selected=static_cast<wxChoice*>(event.GetEventObject())->GetSelection();
			if(selected!=0)
				SetColorScheme(colors[selected-1]);
			else
				SetColorScheme(ColorScheme());
			panel_colors->Enable(selected!=0);
		});

		reset->Bind(wxEVT_BUTTON,[&](wxCommandEvent&){
			int selected=colorselection->GetSelection();
			assert(selected!=0);
			SetColorScheme(ColorScheme());
			colors[selected-1]=ColorScheme();
		});


	}
	{//PAGE 3
		const char *names[]={
			_("Layout files:"),
			_("Gerber export:"),
			_("Bitmap export:"),
			_("HPGL export:"),
			_("Scanned copies:")
		};

		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);

		wxPanel *dir1_panel  = new wxPanel(tabs[2]);
		panel_dirs = new wxPanel(tabs[2],wxID_ANY);

		wxBoxSizer *dir1_box  = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *other_box = new wxBoxSizer(wxVERTICAL);

		panel_dirs->Disable();
		for(int q=0;q<5;q++){
			wxPanel *panel=panel_dirs;
			wxBoxSizer *box=other_box;
			if(q==0){
				panel=dir1_panel;
				box=dir1_box;
			}

			wxBoxSizer *box_l = new wxBoxSizer(wxHORIZONTAL);
			box_l->Add(new wxStaticText(panel, wxID_ANY, names[q]), 1, wxALL|wxEXPAND, 5);

			dirs[q] = new wxDirPickerCtrl(panel, wxID_ANY,
					wxEmptyString, wxEmptyString, wxDefaultPosition,
					wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL);
			string text;
			switch(q){
				case 0:text=s.lay_export; break;
				case 1:text=s.gbr_export; break;
				case 2:text=s.bmp_export; break;
				case 3:text=s.hpgl_export; break;
				case 4:text=s.scanned_copies; break;
			}
			dirs[q]->SetPath(text);
			box_l->Add(dirs[q], 2, wxLEFT|wxRIGHT|wxEXPAND, 5);

			box->Add(box_l,0,wxEXPAND,5);
		}

		panel_dirs->SetSizerAndFit(other_box);
		dir1_panel->SetSizerAndFit(dir1_box);

		panel_dirs->Enable(!s.same_dir);

		all_box->Add(dir1_panel, 0, wxEXPAND, 5);
		all_box->Add(panel_dirs, 0, wxEXPAND, 5);

		all_box->Add(new wxStaticText(tabs[2], wxID_ANY,
			_("Leave this fields empty, if you want OpenLayout to remember\n"
				"the last used directories.")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

		all_box->Add(-1,-1,1, wxALL|wxEXPAND, 5);

		same_dirs = new wxCheckBox(tabs[2], wxID_ANY, _("Use the same folder for all file types"));
		same_dirs->SetValue(s.same_dir);
		same_dirs->Bind(wxEVT_CHECKBOX,[&](wxCommandEvent&e){
			panel_dirs->Enable(
				!static_cast<wxCheckBox*>(e.GetEventObject())->GetValue());
		});
		all_box->Add(same_dirs, 0, wxALL|wxEXPAND, 5);

		tabs[2]->SetSizerAndFit(all_box);
	}
	{//PAGE 4
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
		all_box->Add(new wxStaticText(tabs[3], wxID_ANY, _("Root-Directory for macros:")), 0, wxALL|wxALIGN_LEFT, 5);
		macro_dir = new wxDirPickerCtrl(tabs[3], wxID_ANY, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL);
		macro_dir->SetPath(s.macro_dir);
		all_box->Add(macro_dir, 0, wxALL|wxEXPAND, 20);

		wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 0, 0);
		wxButton *reset=new wxButton(tabs[3], wxID_ANY, _("Reset"));
		reset->Bind(wxEVT_BUTTON,[&](wxCommandEvent&){
			macro_dir->SetPath(Settings::GetDefaultMacroPath());
		});
		sizer->Add(reset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		sizer->Add(new wxStaticText(tabs[3], wxID_ANY,
				_("This will reset the Macro-Directory to default setting")), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
		wxButton*open=new wxButton(tabs[3], wxID_ANY, _("Folder"));
		sizer->Add(open, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		open->Bind(wxEVT_BUTTON,[&](wxCommandEvent&){
			string path=(string)macro_dir->GetPath();
#ifdef _WIN32
			system(string("explorer.exe "+path).c_str());
#else
			system(string("exo-open --launch FileManager "+path+" &").c_str());
#endif
		});
		sizer->Add(new wxStaticText(tabs[3], wxID_ANY, _("Opens the Macro-Directory in the file manager")), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
		all_box->Add(sizer, 1, wxALL|wxEXPAND, 5);
		tabs[3]->SetSizerAndFit(all_box);
	}
	{//PAGE 5
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
		undo_depth=new wxSpinCtrl(tabs[4], wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0,1,50,0);
		undo_depth->SetValue(s.undo);
		all_box->Add(undo_depth, 0, wxALL|wxALIGN_LEFT, 5);
		all_box->Add(new wxStaticText(tabs[4], wxID_ANY,
			_("Here you can define the maximum number of UNDO operations.\n"
				"The maximum is 50 operations\n"
				"If your system runs very slowly while working with big layouts,\n"
				"you can decrease this value down to 1.")), 1, wxALL|wxALIGN_LEFT, 5);
		tabs[4]->SetSizerAndFit(all_box);
	}
	{//PAGE 6
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
		{
			wxBoxSizer *box1 = new wxBoxSizer(wxHORIZONTAL);
			box1->Add(new wxStaticText(tabs[5], wxID_ANY, _("Copper-Thickness in um:")), 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			copper_thickness=new wxSpinCtrl(tabs[5], wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0,0, 299,0);
			copper_thickness->SetValue(s.copper_thickness);
			box1->Add(copper_thickness, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			all_box->Add(box1, 0, wxALL|wxEXPAND, 5);
		}
		{
			wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
			box2->Add(new wxStaticText(tabs[5], wxID_ANY, _("Temperature enchance in C:")), 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			temp_enhance=new wxSpinCtrl(tabs[5], wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0,0,299,0);
			temp_enhance->SetValue(s.temp_enhance);
			box2->Add(temp_enhance, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			all_box->Add(box2, 0, wxALL|wxEXPAND, 5);
		}

		all_box->Add(new wxStaticText(tabs[5], wxID_ANY, _("The calculation of the max. current-carrying capacity for a track,\nis only a coarse approximate value.\nThe real value is depending on many other factors and the environment.\n")), 1, wxALL|wxALIGN_LEFT, 5);
		tabs[5]->SetSizerAndFit(all_box);
	}
	{//PAGE 7
		wxBoxSizer *all_box = new wxBoxSizer(wxHORIZONTAL);
		{
			key_list=new wxListView(tabs[6], wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxBORDER_SIMPLE);

			key_list->AppendColumn("Mode", wxLIST_FORMAT_LEFT, 150);
			key_list->AppendColumn("Key", wxLIST_FORMAT_LEFT, 50);
			for(int q=0;q<TOOL_COUNT;q++){
				key_list->InsertItem(q,Settings::tool_names[q]);
				key_list->SetItem(q,0,Settings::tool_names[q]);
				if(s.tool_keys[q]=='\27')
					key_list->SetItem(q,1,"ESC");
				else
					key_list->SetItem(q,1,s.tool_keys[q]);
				key_list->Bind(wxEVT_LIST_ITEM_SELECTED,[&](wxCommandEvent &e){
					int s=static_cast<wxListView*>(e.GetEventObject())->GetFirstSelected();
					wxString text=static_cast<wxListView*>(e.GetEventObject())->GetItemText(s,1);
					if(text=="ESC")
						key_choice->SetSelection(0);
					else
						key_choice->SetSelection(text[0]-'A'+1);
					key_choice->Enable();
				});
			}
			all_box->Add(key_list, 5, wxALL|wxEXPAND, 5);
		}
		{
			wxStaticBoxSizer *hk_box = new wxStaticBoxSizer(wxHORIZONTAL, tabs[6], _("Change hotkey"));
			{
				key_choice = new wxChoice(tabs[6], wxID_ANY);
				key_choice->Append("ESC",reinterpret_cast<void*>('\27'));
				for(char q='A';q<='Z';q++)
					key_choice->Append(q,reinterpret_cast<void*>(q));
				key_choice->Bind(wxEVT_CHOICE,[&](wxCommandEvent &e){
					int s1=static_cast<wxChoice*>(e.GetEventObject())->GetSelection();
					int s2=key_list->GetFirstSelected();
					if(s1==0)
						key_list->SetItem(s2,1,"ESC");
					else
						key_list->SetItem(s2,1,char(s1-1+'A'));
				});
				key_choice->Disable();
				hk_box->Add(key_choice, 0, wxALL|wxEXPAND, 5);
			}
			all_box->Add(hk_box, 3, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		}
		tabs[6]->SetSizerAndFit(all_box);
	}
	{//PAGE 8
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);

		show_45 = new wxCheckBox(tabs[7], wxID_ANY, _("Show 45 lines"));
		show_45->SetValue(s.show_45_lines);
		all_box->Add(show_45, 0, wxALL|wxALIGN_LEFT, 5);

		wxCheckBox *coord_show = new wxCheckBox(tabs[7], wxID_ANY,_("Show coordinates of crosshair"));
		coord_show->SetValue(s.ccoord_show);
		coord_show->Bind(wxEVT_CHECKBOX,[&](wxCommandEvent &e){
			panel_crosshair->Enable(
				static_cast<wxCheckBox*>(e.GetEventObject())->GetValue());
		});
		all_box->Add(coord_show, 0, wxALL|wxALIGN_LEFT, 5);
		{
			panel_crosshair = new wxPanel(tabs[7]);
			panel_crosshair->Enable(s.ccoord_show);
			wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
			{
				ccoord_tp = new wxCheckBox(panel_crosshair, wxID_ANY,_("Transparent"));
				ccoord_tp->SetValue(s.ccoord_tp);
				box->Add(ccoord_tp, 0, wxALL|wxALIGN_LEFT, 5);
			}
			{
				ccoord_big = new wxCheckBox(panel_crosshair, wxID_ANY, _("Big text"));
				ccoord_big->SetValue(s.ccoord_big);
				box->Add(ccoord_big, 0, wxALL|wxALIGN_LEFT, 5);
			}
			{
				wxStaticBoxSizer *radio_box = new wxStaticBoxSizer(wxVERTICAL, panel_crosshair, _("Textbox"));
				{
					rb_black=new wxRadioButton(panel_crosshair, wxID_ANY, _("Black background"));
					rb_black->SetValue(!s.ccoord_light);
					radio_box->Add(rb_black, 0, wxALIGN_LEFT, 5);
				}
				{
					rb_white=new wxRadioButton(panel_crosshair, wxID_ANY, _("White background"));
					rb_white->SetValue(s.ccoord_light);
					radio_box->Add(rb_white, 0, wxALIGN_LEFT, 5);
				}
				box->Add(radio_box,0,wxLEFT,30);
			}
			panel_crosshair->SetSizerAndFit(box);
			all_box->Add(panel_crosshair,0,wxLEFT|wxEXPAND,30);
		}
		tabs[7]->SetSizerAndFit(all_box);

	}
	{//PAGE 9
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
		all_box->Add(new wxStaticText(tabs[8], wxID_ANY, _("Save layout periodical\nto a separate backup file (*.bak)")), 0, wxALL|wxALIGN_LEFT, 5);
		{
			autosave_enable=new wxCheckBox(tabs[8], wxID_ANY, _("Activate AutoSave"));
			autosave_enable->SetValue(s.autosave);
			autosave_enable->Bind(wxEVT_CHECKBOX,[&](wxCommandEvent &e){
				panel_autosave->Enable(
					static_cast<wxCheckBox*>(e.GetEventObject())->GetValue());
			});
			all_box->Add(autosave_enable, 0, wxALL|wxALIGN_LEFT, 5);
		}
		{
			panel_autosave = new wxPanel(tabs[8],wxID_ANY);
			panel_autosave->Enable(s.autosave);
			wxBoxSizer* min_box = new wxBoxSizer(wxHORIZONTAL);
			min_box->Add(new wxStaticText(panel_autosave, wxID_ANY, _("Interval:")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			{
				autosave_timer=new wxSpinCtrl(panel_autosave, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 100, 0);
				autosave_timer->SetValue(s.autosave_timer);
				min_box->Add(autosave_timer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			}
			min_box->Add(new wxStaticText(panel_autosave, wxID_ANY, _("min")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			panel_autosave->SetSizerAndFit(min_box);
			all_box->Add(panel_autosave, 0, wxLEFT|wxALIGN_LEFT, 20);
		}
		tabs[8]->SetSizerAndFit(all_box);
	}
    const char *tab_names[]={
		_("General settings"),
		_("Colors"),
		_("Working directories"),
		_("Macro-Directory"),
		_("Undo-Depth"),
		_("Imax"),
		_("Hotkeys"),
		_("Crosshair"),
		_("AutoSave")
    };
    for(int q=0;q<9;q++)
		Treebook->AddPage(tabs[q],tab_names[q]);
    BoxSizer1->Add(Treebook, 1, wxALL|wxEXPAND, 5);
    wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
    buttons->AddButton(new wxButton(this, wxID_OK));
    buttons->AddButton(new wxButton(this, wxID_CANCEL));
    buttons->Realize();
    BoxSizer1->Add(buttons, 0, wxALL|wxEXPAND, 5);
    SetSizerAndFit(BoxSizer1);
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::Get(Settings &s){
	for(int q=0;q<(int)GenSettings::COUNT;q++)
		s.gen_settings[q]=checkboxes[q]->GetValue();
	for(int q=0;q<3;q++)
		s.colors[q]=colors[q];
	for(int q=0;q<TOOL_COUNT;q++){
		wxString text=key_list->GetItemText(q,1);
		if(text=="ESC")
			s.tool_keys[q]='\27';
		else
			s.tool_keys[q]=char(text[0]);
	}
	s.units=(Unit)units_choice->GetSelection();
	s.drill=(Drillings)drill_choice->GetSelection();
	s.s_color_scheme=colorselection->GetSelection();

	s.lay_export=dirs[0]->GetPath();
	s.gbr_export=dirs[1]->GetPath();
	s.bmp_export=dirs[2]->GetPath();
	s.hpgl_export=dirs[3]->GetPath();
	s.scanned_copies=dirs[4]->GetPath();
	s.macro_dir=macro_dir->GetPath();
	s.same_dir=same_dirs->GetValue();

	s.undo=undo_depth->GetValue();
	s.copper_thickness=copper_thickness->GetValue();
	s.temp_enhance=temp_enhance->GetValue();

	s.ccoord_big=ccoord_big->GetValue();
	s.ccoord_tp=ccoord_tp->GetValue();
	s.ccoord_show=ccoord_show->GetValue();
	s.ccoord_light=rb_white->GetValue();
	s.show_45_lines=show_45->GetValue();

	s.autosave=autosave_enable->GetValue();
	s.autosave_timer=autosave_timer->GetValue();
}
void SettingsDialog::SetColorScheme(const ColorScheme c){
	for(int q=0;q<12;q++)
		c_colors[q]->SetColour(c.colors[q]);
}

void SettingsDialog::GetColorScheme(ColorScheme &c){
	for(int q=0;q<12;q++)
		c.colors[q]=c_colors[q]->GetColour();
}
