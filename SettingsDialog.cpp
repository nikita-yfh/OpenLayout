#include "SettingsDialog.h"

#include <wx/intl.h>
#include <wx/string.h>

#define WIDGET(type,id)\
	static_cast<type>(FindWindowById(id))

const long SettingsDialog::ID_PANEL_COLORS = wxNewId();
const long SettingsDialog::ID_PANEL_DIRS = wxNewId();
const long SettingsDialog::ID_PANEL_CROSSHAIR = wxNewId();
const long SettingsDialog::ID_PANEL_AUTOSAVE = wxNewId();
const long SettingsDialog::ID_COLORSELECTION = wxNewId();
const long SettingsDialog::ID_MACRO_DIR = wxNewId();
const long SettingsDialog::ID_RB_BLACK = wxNewId();
const long SettingsDialog::ID_RB_WHITE = wxNewId();
const long SettingsDialog::ID_TEST_PANEL = wxNewId();
const long SettingsDialog::ID_TEST_TEXT1 = wxNewId();
const long SettingsDialog::ID_TEST_TEXT2 = wxNewId();
const long SettingsDialog::ID_AUTOSAVE_CHECK = wxNewId();
const long SettingsDialog::ID_AUTOSAVE_TIMER = wxNewId();
const long SettingsDialog::ID_UNITS_CHOISE = wxNewId();
const long SettingsDialog::ID_DRILL_CHOISE = wxNewId();
const long SettingsDialog::ID_UNDO_DEPTH = wxNewId();
const long SettingsDialog::ID_SHOW_45 = wxNewId();
const long SettingsDialog::ID_CCOORD_BIG = wxNewId();
const long SettingsDialog::ID_CCOORD_SHOW = wxNewId();
const long SettingsDialog::ID_CCOORD_TP = wxNewId();
const long SettingsDialog::ID_COPPER_THICKNESS = wxNewId();
const long SettingsDialog::ID_TEMP_ENHANCE = wxNewId();
const long SettingsDialog::ID_KEY_CHOICE = wxNewId();
const long SettingsDialog::ID_KEY_LIST = wxNewId();
const long SettingsDialog::ID_COLORS[12]={wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId()};
const long SettingsDialog::ID_CHECKBOXES[11]={wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId()};
const long SettingsDialog::ID_DIRS[5]={wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId()};
const long SettingsDialog::ID_SAME_DIRS=wxNewId();


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
		wxChoice *utils = new wxChoice(tabs[0], ID_UNITS_CHOISE);
		utils->Append(_("mm"));
		utils->Append(_("mil (1/1000 Zoll)"));
		utils->SetSelection((int)s.units);
		choices->Add(utils, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		choices->Add(new wxStaticText(tabs[0], wxID_ANY, _("Drillings:")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		wxChoice *bgr = new wxChoice(tabs[0], ID_DRILL_CHOISE);
		bgr->Append(_("Background color"));
		bgr->Append(_("White"));
		bgr->Append(_("Black"));
		bgr->SetSelection((int)s.drill);
		choices->Add(bgr, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
			wxCheckBox *checkbox = new wxCheckBox(tabs[0],ID_CHECKBOXES[q], buttons[q]);
			checkbox->SetValue(s.gen_settings[q]);
			all_box->Add(checkbox, 0, wxEXPAND, 5);
		}
		tabs[0]->SetSizerAndFit(all_box);
	}
	{//PAGE 2
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
		box->Add(new wxStaticText(tabs[1], wxID_ANY, _("Color scheme:")),0,wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,5);

		wxChoice *ColorSchemeChoise = new wxChoice(tabs[1], ID_COLORSELECTION);
		ColorSchemeChoise->Append(_("Standart"));
		ColorSchemeChoise->Append(_("User 1"));
		ColorSchemeChoise->Append(_("User 2"));
		ColorSchemeChoise->Append(_("User 3"));
		ColorSchemeChoise->SetSelection(s.s_color_scheme);


		box->Add(ColorSchemeChoise, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		all_box->Add(box, 0, wxALL|wxEXPAND, 5);
		wxPanel *color_panel = new wxPanel(tabs[1],ID_PANEL_COLORS);
		if(s.s_color_scheme==0)
			color_panel->Disable();

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
			int selected=WIDGET(wxChoice*,ID_COLORSELECTION)->GetSelection();
			assert(selected);
			GetColorScheme(colors[selected-1]);
		};

		for(int q=0;q<13;q++){
			if(color_names[q]){
				wxColourPickerCtrl *color_ctrl=new wxColourPickerCtrl(color_panel,ID_COLORS[color_ids[q]]);
				color_ctrl->Bind(wxEVT_COLOURPICKER_CHANGED,color_cb);
				FlexGridSizer1->Add(color_ctrl,1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
				FlexGridSizer1->Add(
					new wxStaticText(color_panel, wxID_ANY, color_names[q]),
					1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			}else{
				for(int q=0;q<2;q++)
					FlexGridSizer1->Add(-1,-1,1, wxEXPAND, 5);
			}
		}
		BoxSizer9->Add(FlexGridSizer1, 0, wxEXPAND, 5);
		wxButton *reset=new wxButton(color_panel, wxID_ANY, _("Reset scheme to default"));
		BoxSizer9->Add(reset, 0, wxALL|wxALIGN_RIGHT, 5);
		color_panel->SetSizerAndFit(BoxSizer9);
		all_box->Add(color_panel, 1, wxALL|wxEXPAND, 5);
		tabs[1]->SetSizerAndFit(all_box);

		if(s.s_color_scheme!=0)
			SetColorScheme(colors[s.s_color_scheme-1]);
		else
			SetColorScheme(ColorScheme());

		ColorSchemeChoise->Bind(wxEVT_CHOICE,[&](wxCommandEvent&event){
			int selected=static_cast<wxChoice*>(event.GetEventObject())->GetSelection();
			if(selected!=0)
				SetColorScheme(colors[selected-1]);
			else
				SetColorScheme(ColorScheme());
			FindWindowById(ID_PANEL_COLORS)->Enable(selected!=0);
		});

		reset->Bind(wxEVT_BUTTON,[&](wxCommandEvent&){
			int selected=WIDGET(wxChoice*,ID_COLORSELECTION)->GetSelection();
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
		wxPanel *other_panel = new wxPanel(tabs[2],ID_PANEL_DIRS);

		wxBoxSizer *dir1_box  = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *other_box = new wxBoxSizer(wxVERTICAL);

		other_panel->Disable();
		for(int q=0;q<5;q++){
			wxPanel *panel=other_panel;
			wxBoxSizer *box=other_box;
			if(q==0){
				panel=dir1_panel;
				box=dir1_box;
			}

			wxBoxSizer *box_l = new wxBoxSizer(wxHORIZONTAL);
			box_l->Add(new wxStaticText(panel, wxID_ANY, names[q]), 1, wxALL|wxEXPAND, 5);

			wxDirPickerCtrl *dir = new wxDirPickerCtrl(panel, ID_DIRS[q],
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
			dir->SetPath(text);
			box_l->Add(dir, 2, wxLEFT|wxRIGHT|wxEXPAND, 5);

			box->Add(box_l,0,wxEXPAND,5);
		}

		other_panel->SetSizerAndFit(other_box);
		dir1_panel->SetSizerAndFit(dir1_box);

		other_panel->Enable(!s.same_dir);

		all_box->Add(dir1_panel, 0, wxEXPAND, 5);
		all_box->Add(other_panel, 0, wxEXPAND, 5);

		all_box->Add(new wxStaticText(tabs[2], wxID_ANY,
			_("Leave this fields empty, if you want OpenLayout to remember\n"
				"the last used directories.")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

		all_box->Add(-1,-1,1, wxALL|wxEXPAND, 5);

		wxCheckBox *SameDir = new wxCheckBox(tabs[2], ID_SAME_DIRS, _("Use the same folder for all file types"));
		SameDir->SetValue(s.same_dir);
		SameDir->Bind(wxEVT_CHECKBOX,[&](wxCommandEvent&e){
			WIDGET(wxPanel*,ID_PANEL_DIRS)->Enable(
				!static_cast<wxCheckBox*>(e.GetEventObject())->GetValue());
		});
		all_box->Add(SameDir, 0, wxALL|wxEXPAND, 5);

		tabs[2]->SetSizerAndFit(all_box);
	}
	{//PAGE 4
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
		all_box->Add(new wxStaticText(tabs[3], wxID_ANY, _("Root-Directory for macros:")), 0, wxALL|wxALIGN_LEFT, 5);
		wxDirPickerCtrl *MacroDir = new wxDirPickerCtrl(tabs[3], ID_MACRO_DIR, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL|wxDIRP_SMALL);
		MacroDir->SetPath(s.macro_dir);
		all_box->Add(MacroDir, 0, wxALL|wxEXPAND, 20);

		wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 0, 0);
		wxButton *reset=new wxButton(tabs[3], wxID_ANY, _("Reset"));
		reset->Bind(wxEVT_BUTTON,[&](wxCommandEvent&){
			WIDGET(wxDirPickerCtrl*,ID_MACRO_DIR)->SetPath(Settings::GetDefaultMacroPath());
		});
		sizer->Add(reset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		sizer->Add(new wxStaticText(tabs[3], wxID_ANY,
				_("This will reset the Macro-Directory to default setting")), 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
		wxButton*open=new wxButton(tabs[3], wxID_ANY, _("Folder"));
		sizer->Add(open, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		open->Bind(wxEVT_BUTTON,[&](wxCommandEvent&){
			string path=(string)WIDGET(wxDirPickerCtrl*,ID_MACRO_DIR)->GetPath();
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
		wxSpinCtrl *ctrl=new wxSpinCtrl(tabs[4], ID_UNDO_DEPTH, "0", wxDefaultPosition, wxDefaultSize, 0,1,50,0);
		ctrl->SetValue(s.undo);
		all_box->Add(ctrl, 0, wxALL|wxALIGN_LEFT, 5);
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
			wxSpinCtrl *thickness=new wxSpinCtrl(tabs[5], ID_COPPER_THICKNESS, "0", wxDefaultPosition, wxDefaultSize, 0,0, 299,0);
			thickness->SetValue(s.copper_thickness);
			box1->Add(thickness, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			all_box->Add(box1, 0, wxALL|wxEXPAND, 5);
		}
		{
			wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
			box2->Add(new wxStaticText(tabs[5], wxID_ANY, _("Temperature enchance in C:")), 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			wxSpinCtrl *temp=new wxSpinCtrl(tabs[5], ID_TEMP_ENHANCE, "0", wxDefaultPosition, wxDefaultSize, 0,0,299,0);
			temp->SetValue(s.temp_enhance);
			box2->Add(temp, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			all_box->Add(box2, 0, wxALL|wxEXPAND, 5);
		}

		all_box->Add(new wxStaticText(tabs[5], wxID_ANY, _("The calculation of the max. current-carrying capacity for a track,\nis only a coarse approximate value.\nThe real value is depending on many other factors and the environment.\n")), 1, wxALL|wxALIGN_LEFT, 5);
		tabs[5]->SetSizerAndFit(all_box);
	}
	{//PAGE 7
		wxBoxSizer *all_box = new wxBoxSizer(wxHORIZONTAL);
		{
			wxListView *list=new wxListView(tabs[6], ID_KEY_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxBORDER_SIMPLE);

			list->AppendColumn("Mode", wxLIST_FORMAT_LEFT, 150);
			list->AppendColumn("Key", wxLIST_FORMAT_LEFT, 50);
			for(int q=0;q<TOOL_COUNT;q++){
				list->InsertItem(q,Settings::tool_names[q]);
				list->SetItem(q,0,Settings::tool_names[q]);
				if(s.tool_keys[q]=='\27')
					list->SetItem(q,1,"ESC");
				else
					list->SetItem(q,1,s.tool_keys[q]);
				list->Bind(wxEVT_LIST_ITEM_SELECTED,[&](wxCommandEvent &e){
					int s=static_cast<wxListView*>(e.GetEventObject())->GetFirstSelected();
					wxString text=static_cast<wxListView*>(e.GetEventObject())->GetItemText(s,1);
					if(text=="ESC")
						WIDGET(wxChoice*,ID_KEY_CHOICE)->SetSelection(0);
					else
						WIDGET(wxChoice*,ID_KEY_CHOICE)->SetSelection(text[0]-'A'+1);
					WIDGET(wxChoice*,ID_KEY_CHOICE)->Enable();
				});
			}
			all_box->Add(list, 5, wxALL|wxEXPAND, 5);
		}
		{
			wxStaticBoxSizer *hk_box = new wxStaticBoxSizer(wxHORIZONTAL, tabs[6], _("Change hotkey"));
			{
				wxChoice *ToolKeyChoice = new wxChoice(tabs[6], ID_KEY_CHOICE);
				ToolKeyChoice->Append("ESC",reinterpret_cast<void*>('\27'));
				for(char q='A';q<='Z';q++)
					ToolKeyChoice->Append(q,reinterpret_cast<void*>(q));
				ToolKeyChoice->Bind(wxEVT_CHOICE,[&](wxCommandEvent &e){
					int s1=static_cast<wxChoice*>(e.GetEventObject())->GetSelection();
					int s2=WIDGET(wxListView*,ID_KEY_LIST)->GetFirstSelected();
					if(s1==0)
						WIDGET(wxListView*,ID_KEY_LIST)->SetItem(s2,1,"ESC");
					else
						WIDGET(wxListView*,ID_KEY_LIST)->SetItem(s2,1,char(s1-1+'A'));
				});
				ToolKeyChoice->Disable();
				hk_box->Add(ToolKeyChoice, 0, wxALL|wxEXPAND, 5);
			}
			all_box->Add(hk_box, 3, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
		}
		tabs[6]->SetSizerAndFit(all_box);
	}
	{//PAGE 8
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);

		wxCheckBox *c45 = new wxCheckBox(tabs[7], ID_SHOW_45, _("Show 45 lines"));
		c45->SetValue(s.show_45_lines);
		all_box->Add(c45, 0, wxALL|wxALIGN_LEFT, 5);

		wxCheckBox *coord = new wxCheckBox(tabs[7], ID_CCOORD_SHOW,_("Show coordinates of crosshair"));
		coord->SetValue(s.ccoord_show);
		coord->Bind(wxEVT_CHECKBOX,[&](wxCommandEvent &e){
			WIDGET(wxPanel*,ID_PANEL_CROSSHAIR)->Enable(
				static_cast<wxCheckBox*>(e.GetEventObject())->GetValue());
		});
		all_box->Add(coord, 0, wxALL|wxALIGN_LEFT, 5);
		{
			wxPanel *panel = new wxPanel(tabs[7],ID_PANEL_CROSSHAIR);
			panel->Enable(s.ccoord_show);
			wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
			{
				wxCheckBox *tp = new wxCheckBox(panel, ID_CCOORD_TP,_("Transparent"));
				tp->SetValue(s.ccoord_tp);
				box->Add(tp, 0, wxALL|wxALIGN_LEFT, 5);
			}
			{
				wxCheckBox *bt = new wxCheckBox(panel, ID_CCOORD_BIG, _("Big text"));
				bt->SetValue(s.ccoord_big);
				box->Add(bt, 0, wxALL|wxALIGN_LEFT, 5);
			}
			wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
			auto set_test_style=[&](bool light){
				wxColor color_panel=wxColour(255,255,255);
				wxColor color_text=wxColour(0,0,0);
				if(!light)
					swap(color_panel,color_text);
				WIDGET(wxPanel*,ID_TEST_PANEL)->SetBackgroundColour(color_panel);
				WIDGET(wxPanel*,ID_TEST_TEXT1)->SetForegroundColour(color_text);
				WIDGET(wxPanel*,ID_TEST_TEXT2)->SetForegroundColour(color_text);
			};
			{
				wxStaticBoxSizer *radio_box = new wxStaticBoxSizer(wxVERTICAL, panel, _("Textbox"));
				{
					wxRadioButton *rb_black=new wxRadioButton(panel, ID_RB_BLACK, _("Black background"));
					rb_black->SetValue(!s.ccoord_light);
					rb_black->Bind(wxEVT_RADIOBUTTON,[&](wxCommandEvent&){set_test_style(false);});
					radio_box->Add(rb_black, 0, wxALIGN_LEFT, 5);
				}
				{
					wxRadioButton *rb_white=new wxRadioButton(panel, ID_RB_WHITE, _("White background"));
					rb_white->SetValue(s.ccoord_light);
					rb_white->Bind(wxEVT_RADIOBUTTON,[&](wxCommandEvent&){set_test_style(true);});
					radio_box->Add(rb_white, 0, wxALIGN_LEFT, 5);
				}
				box2->Add(radio_box, 0, wxALL|wxEXPAND, 5);
			}
			{
				wxPanel *test_panel = new wxPanel(panel,ID_TEST_PANEL);
				test_panel->SetBackgroundColour(wxColour(0,0,0));
				wxBoxSizer *test_box = new wxBoxSizer(wxVERTICAL);


				const char *str[2]={"X:  12.280 mm", "Y:  14.640 mm"};
				for(int q=0;q<2;q++){
					wxStaticText *text = new wxStaticText(test_panel, q?ID_TEST_TEXT2:ID_TEST_TEXT1, str[q]);
					test_box->Add(text, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
				}
				test_panel->SetSizerAndFit(test_box);
				box2->Add(test_panel, 0, wxALL|wxEXPAND, 5);
			}
			set_test_style(s.ccoord_light);
			box->Add(box2, 0, wxALL|wxEXPAND, 5);
			panel->SetSizerAndFit(box);
			all_box->Add(panel, 0, wxLEFT|wxALIGN_LEFT, 20);
		}
		tabs[7]->SetSizerAndFit(all_box);

	}
	{//PAGE 9
		wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
		all_box->Add(new wxStaticText(tabs[8], wxID_ANY, _("Save layout periodical\nto a separate backup file (*.bak)")), 0, wxALL|wxALIGN_LEFT, 5);
		{
			wxCheckBox *check=new wxCheckBox(tabs[8], ID_AUTOSAVE_CHECK, _("Activate AutoSave"));
			check->SetValue(s.autosave);
			check->Bind(wxEVT_CHECKBOX,[&](wxCommandEvent &e){
				WIDGET(wxPanel*,ID_PANEL_AUTOSAVE)->Enable(
					static_cast<wxCheckBox*>(e.GetEventObject())->GetValue());
			});
			all_box->Add(check, 0, wxALL|wxALIGN_LEFT, 5);
		}
		{
			wxPanel *autosave_panel = new wxPanel(tabs[8],ID_PANEL_AUTOSAVE);
			autosave_panel->Enable(s.autosave);
			wxBoxSizer* min_box = new wxBoxSizer(wxHORIZONTAL);
			min_box->Add(new wxStaticText(autosave_panel, wxID_ANY, _("Interval:")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			{
				wxSpinCtrl *timer=new wxSpinCtrl(autosave_panel, ID_AUTOSAVE_TIMER, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 100, 0);
				timer->SetValue(s.autosave_timer);
				min_box->Add(timer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			}
			min_box->Add(new wxStaticText(autosave_panel, wxID_ANY, _("min")), 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			autosave_panel->SetSizerAndFit(min_box);
			all_box->Add(autosave_panel, 0, wxLEFT|wxALIGN_LEFT, 20);
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
	for(int q=0;q<(int)GenSettings::COUNT;q++){
		wxCheckBox *checkbox = WIDGET(wxCheckBox*,ID_CHECKBOXES[q]);
		s.gen_settings[q]=checkbox->GetValue();
	}
	for(int q=0;q<3;q++)
		s.colors[q]=colors[q];
	for(int q=0;q<TOOL_COUNT;q++){
		wxString text=WIDGET(wxListView*,ID_KEY_LIST)->GetItemText(q,1);
		if(text=="ESC")
			s.tool_keys[q]='\27';
		else
			s.tool_keys[q]=char(text[0]);
	}
	s.units=(Unit)WIDGET(wxChoice*,ID_UNITS_CHOISE)->GetSelection();
	s.drill=(Drillings)WIDGET(wxChoice*,ID_DRILL_CHOISE)->GetSelection();
	s.s_color_scheme=WIDGET(wxChoice*,ID_COLORSELECTION)->GetSelection();

	s.lay_export=WIDGET(wxDirPickerCtrl*,ID_DIRS[0])->GetPath();
	s.gbr_export=WIDGET(wxDirPickerCtrl*,ID_DIRS[1])->GetPath();
	s.bmp_export=WIDGET(wxDirPickerCtrl*,ID_DIRS[2])->GetPath();
	s.hpgl_export=WIDGET(wxDirPickerCtrl*,ID_DIRS[3])->GetPath();
	s.scanned_copies=WIDGET(wxDirPickerCtrl*,ID_DIRS[4])->GetPath();
	s.macro_dir=WIDGET(wxDirPickerCtrl*,ID_MACRO_DIR)->GetPath();
	s.same_dir=WIDGET(wxCheckBox*,ID_SAME_DIRS)->GetValue();

	s.undo=WIDGET(wxSpinCtrl*,ID_UNDO_DEPTH)->GetValue();
	s.copper_thickness=WIDGET(wxSpinCtrl*,ID_COPPER_THICKNESS)->GetValue();
	s.temp_enhance=WIDGET(wxSpinCtrl*,ID_TEMP_ENHANCE)->GetValue();

	s.ccoord_big=WIDGET(wxCheckBox*,ID_CCOORD_BIG)->GetValue();
	s.ccoord_tp=WIDGET(wxCheckBox*,ID_CCOORD_TP)->GetValue();
	s.ccoord_show=WIDGET(wxCheckBox*,ID_CCOORD_SHOW)->GetValue();
	s.ccoord_light=WIDGET(wxCheckBox*,ID_RB_WHITE)->GetValue();
	s.show_45_lines=WIDGET(wxCheckBox*,ID_SHOW_45)->GetValue();

	s.autosave=WIDGET(wxCheckBox*,ID_AUTOSAVE_CHECK)->GetValue();
	s.autosave_timer=WIDGET(wxSpinCtrl*,ID_AUTOSAVE_TIMER)->GetValue();
}
void SettingsDialog::SetColorScheme(const ColorScheme c){
	for(int q=0;q<12;q++)
		WIDGET(wxColourPickerCtrl*,ID_COLORS[q])->SetColour(c.colors[q]);
}

void SettingsDialog::GetColorScheme(ColorScheme &c){
	for(int q=0;q<12;q++)
		c.colors[q]=WIDGET(wxColourPickerCtrl*,ID_COLORS[q])->GetColour();
}
