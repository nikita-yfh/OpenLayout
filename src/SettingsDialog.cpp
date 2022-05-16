#include "SettingsDialog.h"

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <wx/treebook.h>

#define EVT_COLOURPICKER_CHANGED_RANGE(id1, id2, func) \
	wx__DECLARE_EVT2(wxEVT_COLOURPICKER_CHANGED, id1, id2, wxColourPickerEventHandler(func))

enum {
	ID_COLOR_SCHEME_SELECTION = 1,
	ID_COLOR_SCHEME_PANEL,
	ID_COLOR_SCHEME_RESET,
	ID_COLOR_FIRST,
	ID_COLOR_LAST = ID_COLOR_FIRST + 13,
	ID_DIR_LAYOUT,
	ID_DIR_GERBER,
	ID_DIR_BITMAP,
	ID_DIR_HPGL,
	ID_DIR_SCANNED,
	ID_MACRO_RESET,
	ID_MACRO_OPEN,
	ID_TOOL_LIST,
	ID_KEY_LIST,
	ID_CROSSHAIR_PANEL,
	ID_AUTOSAVE_PANEL,
};

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_CHOICE(ID_COLOR_SCHEME_SELECTION,	SettingsDialog::SelectColorScheme)
	EVT_BUTTON(ID_COLOR_SCHEME_RESET,		SettingsDialog::ResetColorScheme)
	EVT_UPDATE_UI(ID_COLOR_SCHEME_PANEL,	SettingsDialog::UpdateColorSchemePanel)
	EVT_UPDATE_UI(ID_COLOR_SCHEME_RESET,	SettingsDialog::UpdateColorSchemePanel)
	EVT_COLOURPICKER_CHANGED_RANGE(ID_COLOR_FIRST, ID_COLOR_LAST,
											SettingsDialog::ChangeColorScheme)
	EVT_UPDATE_UI_RANGE(ID_DIR_GERBER, ID_DIR_SCANNED,
											SettingsDialog::UpdateDirs)
	EVT_BUTTON(ID_MACRO_RESET,				SettingsDialog::ResetMacroDir)
	EVT_BUTTON(ID_MACRO_OPEN,				SettingsDialog::OpenMacroDir)
	EVT_LIST_ITEM_SELECTED(ID_TOOL_LIST,	SettingsDialog::SelectTool)
	EVT_CHOICE(ID_KEY_LIST,					SettingsDialog::SelectKey)
	EVT_UPDATE_UI(ID_CROSSHAIR_PANEL,		SettingsDialog::UpdateCrosshairPanel)
	EVT_UPDATE_UI(ID_AUTOSAVE_PANEL,		SettingsDialog::UpdateAutosavePanel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, const Settings &s)
		: wxDialog(parent, wxID_ANY, _("General settings")) {
	wxTreebook *treebook = new wxTreebook(this, wxID_ANY);

	wxPanel *tabs[9];
	for(int i = 0; i < 9; i++)
		tabs[i] = new wxPanel(treebook);
	{
		//PAGE 1


		wxBoxSizer* content = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* choices = new wxBoxSizer(wxHORIZONTAL);

		choices->Add(new wxStaticText(tabs[0], wxID_ANY, _("Base unit:")),
			0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		unitsChoice = new wxChoice(tabs[0], wxID_ANY);
		unitsChoice ->Append(_("mm"));
		unitsChoice ->Append(_("mil (1/1000 Zoll)"));
		unitsChoice ->SetSelection(s.units);
		choices->Add(unitsChoice, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		choices->Add(new wxStaticText(tabs[0], wxID_ANY, _("Drillings:")),
			0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		drillChoice = new wxChoice(tabs[0], wxID_ANY);
		drillChoice->Append(_("Background color"));
		drillChoice->Append(_("White"));
		drillChoice->Append(_("Black"));
		drillChoice->SetSelection((int)s.drill);
		choices->Add(drillChoice, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		content->Add(choices, 0, wxEXPAND, 5);

		const char *buttons[]= {
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

		for(int i = 0; i < 11; i++) {
			checkboxes[i] = new wxCheckBox(tabs[0], wxID_ANY, buttons[i]);
			checkboxes[i]->SetValue(s[i]);
			content->Add(checkboxes[i], 0, wxEXPAND, 5);
		}
		tabs[0]->SetSizerAndFit(content);
	}
	{
		//PAGE 2
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
		box->Add(new wxStaticText(tabs[1], wxID_ANY, _("Color scheme:")),
			0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

		colorSelection = new wxChoice(tabs[1], ID_COLOR_SCHEME_SELECTION);
		colorSelection->Append(_("Standart"));
		colorSelection->Append(_("User 1"));
		colorSelection->Append(_("User 2"));
		colorSelection->Append(_("User 3"));
		colorSelection->SetSelection(s.selectedColorScheme);
		box->Add(colorSelection, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		content->Add(box, 0, wxALL | wxEXPAND, 5);

		wxPanel *panelColors = new wxPanel(tabs[1], ID_COLOR_SCHEME_PANEL);
		if(s.selectedColorScheme == 0)
			panelColors->Disable();

		const int colorIds[]= {0, 7, 1, 8, 2, 9, 3, 10, 4, 11, 5, 12, 6, 13};
		for(int i = 0; i < 3; i++)
			colors[i] = s.colors[i];
		const char* colorNames[]= {
			_("C1 (Copper-Top)"),
			_("Background"),
			_("S1 (Silkscreen-Top)"),
			_("Grid-lines"),
			_("C2 (Copper-Bottom)"),
			_("Grid-dots"),
			_("S2 (Silkscreen-Bottom)"),
			_("Connections"),
			_("I1 (Copper-Inner 1)"),
			_("Via"),
			_("I2 (Copper-Inner 2)"),
			_("Selected object"),
			_("O (Outline)"),
			_("Selection zone")
		};
		wxFlexGridSizer *table = new wxFlexGridSizer(7, 4, 0, 0);

		for(int i = 0; i < 14; i++) {
			colorPicker[colorIds[i]] = new wxColourPickerCtrl(panelColors, ID_COLOR_FIRST + colorIds[i]);
			table->Add(colorPicker[colorIds[i]], 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			table->Add(new wxStaticText(panelColors, wxID_ANY, colorNames[i]),
				1, wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
		}
		panelColors->SetSizer(table);
		content->Add(panelColors, 1, wxALL | wxEXPAND, 5);
		content->Add(new wxButton(tabs[1], ID_COLOR_SCHEME_RESET, _("Reset scheme to default")),
				0, wxALL | wxALIGN_RIGHT, 5);
		tabs[1]->SetSizerAndFit(content);

		if(s.selectedColorScheme != 0)
			SetColorScheme(colors[s.selectedColorScheme - 1]);
		else
			SetColorScheme(ColorScheme());

	}
	{
		//PAGE 3
		const char *names[]= {
			_("Layout files:"),
			_("Gerber export:"),
			_("Bitmap export:"),
			_("HPGL export:"),
			_("Scanned copies:")
		};

		const char *values[] = {
			s.layExport,
			s.gbrExport,
			s.bmpExport,
			s.hpglExport,
			s.scannedCopies
		};

		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);

		wxGridSizer *table = new wxGridSizer(5, 2, 2, 5);
		for(int i = 0; i < 5; i++) {
			table->Add(new wxStaticText(tabs[2], ID_DIR_LAYOUT + i, names[i]), 1, wxALL | wxEXPAND, 5);
			dirs[i] = new wxDirPickerCtrl(tabs[2], ID_DIR_LAYOUT + i, wxEmptyString, wxEmptyString,
					wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL | wxDIRP_SMALL);
			dirs[i]->SetPath(values[i]);
			table->Add(dirs[i], 2, wxALL | wxEXPAND, 5);
		}

		content->Add(table, 0, wxEXPAND | wxALL, 5);
		content->Add(new wxStaticText(tabs[2], wxID_ANY,
									  _("Leave this fields empty, if you want OpenLayout to remember\n"
										"the last used directories.")), 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

		content->Add(-1, -1, 1, wxALL | wxEXPAND, 5);

		sameDirs = new wxCheckBox(tabs[2], wxID_ANY, _("Use the same folder for all file types"));
		sameDirs->SetValue(s.sameDir);
		content->Add(sameDirs, 0, wxALL | wxEXPAND, 5);

		tabs[2]->SetSizerAndFit(content);
	}
	{
		//PAGE 4
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
		content->Add(new wxStaticText(tabs[3], wxID_ANY, _("Root-Directory for macros:")), 0, wxALL | wxALIGN_LEFT, 5);
		macroDir = new wxDirPickerCtrl(tabs[3], wxID_ANY, wxEmptyString, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL | wxDIRP_SMALL);
		macroDir->SetPath(s.macroDir);
		content->Add(macroDir, 0, wxALL | wxEXPAND, 20);

		wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 0, 0);
		wxButton *reset = new wxButton(tabs[3], ID_MACRO_RESET, _("Reset"));
		sizer->Add(reset, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		sizer->Add(new wxStaticText(tabs[3], wxID_ANY,
									_("This will reset the Macro-Directory to default setting")), 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
		wxButton *open = new wxButton(tabs[3], ID_MACRO_OPEN, _("Folder"));
		sizer->Add(open, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		sizer->Add(new wxStaticText(tabs[3], wxID_ANY, _("Opens the Macro-Directory in the file manager")), 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
		content->Add(sizer, 1, wxALL | wxEXPAND, 5);
		tabs[3]->SetSizerAndFit(content);
	}
	{
		//PAGE 5
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
		undoDepth = new wxSpinCtrl(tabs[4], wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 1, 50, 0);
		undoDepth->SetValue(s.undoDepth);
		content->Add(undoDepth, 0, wxALL | wxALIGN_LEFT, 5);
		content->Add(new wxStaticText(tabs[4], wxID_ANY,
									  _("Here you can define the maximum number of UNDO operations.\n"
										"The maximum is 50 operations\n"
										"If your system runs very slowly while working with big layouts, \n"
										"you can decrease this value down to 1.")), 1, wxALL | wxALIGN_LEFT, 5);
		tabs[4]->SetSizerAndFit(content);
	}
	{
		//PAGE 6
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
		{
			wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
			box->Add(new wxStaticText(tabs[5], wxID_ANY, _(L"Copper-Thickness in \x00b5m:")), 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			copperThickness = new wxSpinCtrl(tabs[5], wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 0, 299, 0);
			copperThickness->SetValue(s.copperThickness);
			box->Add(copperThickness, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			content->Add(box, 0, wxALL | wxEXPAND, 5);
		}
		{
			wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
			box->Add(new wxStaticText(tabs[5], wxID_ANY, _(L"Temperature enchance in \x00b0""C:")), 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			tempEnhance = new wxSpinCtrl(tabs[5], wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, 0, 299, 0);
			tempEnhance->SetValue(s.tempEnhance);
			box->Add(tempEnhance, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			content->Add(box, 0, wxALL | wxEXPAND, 5);
		}

		content->Add(new wxStaticText(tabs[5], wxID_ANY, _("The calculation of the max. current-carrying capacity for a track, \nis only a coarse approximate value.\nThe real value is depending on many other factors and the environment.\n")), 1, wxALL | wxALIGN_LEFT, 5);
		tabs[5]->SetSizerAndFit(content);
	}
	{
		//PAGE 7
		wxBoxSizer *content = new wxBoxSizer(wxHORIZONTAL);
		{

			const char *toolNames[] = {
				_("Edit"), _("Zoom"), _("Track"), _("Pad"),
				_("SMD-Pad"), _("Circle"), _("Rectangle"),
				_("Zone"), _("Special form"), _("Text"),
				_("Solder mask"), _("Connections"), _("Autoroute"),
				_("Test"), _("Measure"), _("Photoview")
			};
			toolList = new wxListView(tabs[6], ID_TOOL_LIST, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_SIMPLE);

			toolList->AppendColumn(_("Mode"), wxLIST_FORMAT_LEFT, 150);
			toolList->AppendColumn(_("Key"), wxLIST_FORMAT_LEFT, 50);
			for(int i = 0; i < TOOL_COUNT; i++) {
				toolList->InsertItem(i, toolNames[i]);
				toolList->SetItem(i, 0, toolNames[i]);
				if(s.toolKeys[i] == ESCAPE)
					toolList->SetItem(i, 1, "ESC");
				else
					toolList->SetItem(i, 1, s.toolKeys[i]);
			}
			content->Add(toolList, 5, wxALL | wxEXPAND, 5);
		}
		{
			wxStaticBoxSizer *hkBox = new wxStaticBoxSizer(wxHORIZONTAL, tabs[6], _("Change hotkey"));
			{
				keyList = new wxChoice(tabs[6], ID_KEY_LIST);
				keyList->Append("ESC", reinterpret_cast<void*>(ESCAPE));
				for(char i = 'A'; i <= 'Z'; i++)
					keyList->Append(i, reinterpret_cast<void*>(i));
				keyList->Disable();
				hkBox->Add(keyList, 0, wxALL | wxEXPAND, 5);
			}
			content->Add(hkBox, 3, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
		}
		tabs[6]->SetSizerAndFit(content);
	}
	{
		//PAGE 8
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);

		show45 = new wxCheckBox(tabs[7], wxID_ANY, _("Show 45 lines"));
		show45->SetValue(s.measure45Lines);
		content->Add(show45, 0, wxALL | wxALIGN_LEFT, 5);

		measureShow = new wxCheckBox(tabs[7], wxID_ANY, _("Show coordinates of crosshair"));
		measureShow->SetValue(s.measureShow);
		content->Add(measureShow, 0, wxALL | wxALIGN_LEFT, 5);
		{
			wxPanel *panelCrosshair = new wxPanel(tabs[7], ID_CROSSHAIR_PANEL);
			wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
			{
				measureTp = new wxCheckBox(panelCrosshair, wxID_ANY, _("Transparent"));
				measureTp->SetValue(s.measureTp);
				box->Add(measureTp, 0, wxALL | wxALIGN_LEFT, 5);
			}
			{
				measureBig = new wxCheckBox(panelCrosshair, wxID_ANY, _("Big text"));
				measureBig->SetValue(s.measureBig);
				box->Add(measureBig, 0, wxALL | wxALIGN_LEFT, 5);
			}
			{
				wxStaticBoxSizer *radioBox = new wxStaticBoxSizer(wxVERTICAL, panelCrosshair, _("Textbox"));
				{
					rbBlack = new wxRadioButton(panelCrosshair, wxID_ANY, _("Black background"));
					rbBlack->SetValue(!s.measureLight);
					radioBox->Add(rbBlack, 0, wxALIGN_LEFT | wxALL, 5);
				}
				{
					rbWhite = new wxRadioButton(panelCrosshair, wxID_ANY, _("White background"));
					rbWhite->SetValue(s.measureLight);
					radioBox->Add(rbWhite, 0, wxALIGN_LEFT | wxALL, 5);
				}
				box->Add(radioBox, 0, wxALL, 5);
			}
			panelCrosshair->SetSizerAndFit(box);
			content->Add(panelCrosshair, 0, wxLEFT | wxEXPAND, 30);
		}
		tabs[7]->SetSizerAndFit(content);

	}
	{
		//PAGE 9
		wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
		content->Add(new wxStaticText(tabs[8], wxID_ANY, _("Save layout periodical\nto a separate backup file (*.bak)")), 0, wxALL | wxALIGN_LEFT, 5);
		{
			autosaveEnabled = new wxCheckBox(tabs[8], wxID_ANY, _("Activate AutoSave"));
			autosaveEnabled->SetValue(s.autosave);
			content->Add(autosaveEnabled, 0, wxALL | wxALIGN_LEFT, 5);
		}
		{
			wxPanel *panelAutosave = new wxPanel(tabs[8], ID_AUTOSAVE_PANEL);
			wxBoxSizer* minBox = new wxBoxSizer(wxHORIZONTAL);
			minBox->Add(new wxStaticText(panelAutosave, wxID_ANY, _("Interval:")), 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			{
				autosaveTimer = new wxSpinCtrl(panelAutosave, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, 0, 1, 100, 0);
				autosaveTimer->SetValue(s.autosaveTimer);
				minBox->Add(autosaveTimer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			}
			minBox->Add(new wxStaticText(panelAutosave, wxID_ANY, _("min")), 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
			panelAutosave->SetSizerAndFit(minBox);
			content->Add(panelAutosave, 0, wxLEFT | wxALIGN_LEFT, 20);
		}
		tabs[8]->SetSizerAndFit(content);
	}
	const char *tabNames[]= {
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
	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	for(int i = 0; i < 9; i++)
		treebook->AddPage(tabs[i], tabNames[i]);
	content->Add(treebook, 1, wxALL | wxEXPAND, 5);
	content->Add(CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);
	SetSizerAndFit(content);
}

void SettingsDialog::Get(Settings &s) {
	for(int i = 0; i < 11; i++)
		s[i] = checkboxes[i]->GetValue();
	for(int i = 0; i < 3; i++)
		s.colors[i] = colors[i];
	for(int i = 0; i < TOOL_COUNT; i++) {
		wxString text = toolList->GetItemText(i, 1);
		if(text == "ESC")
			s.toolKeys[i] = 27;
		else
			s.toolKeys[i] = char(text[0]);
	}
	s.units = unitsChoice->GetSelection();
	s.drill = drillChoice->GetSelection();
	s.selectedColorScheme = colorSelection->GetSelection();

	strcpy(s.layExport, dirs[0]->GetPath().c_str());
	strcpy(s.gbrExport, dirs[1]->GetPath().c_str());
	strcpy(s.hpglExport, dirs[2]->GetPath().c_str());
	strcpy(s.scannedCopies, dirs[3]->GetPath().c_str());
	strcpy(s.macroDir, dirs[4]->GetPath().c_str());
	s.sameDir = sameDirs->GetValue();

	s.undoDepth = undoDepth->GetValue();
	s.copperThickness = copperThickness->GetValue();
	s.tempEnhance = tempEnhance->GetValue();

	s.measureBig = measureBig->GetValue();
	s.measureTp = measureTp->GetValue();
	s.measureShow = measureShow->GetValue();
	s.measureLight = rbWhite->GetValue();
	s.measure45Lines = show45->GetValue();

	s.autosave = autosaveEnabled->GetValue();
	s.autosaveTimer = autosaveTimer->GetValue();
}

void SettingsDialog::SetColorScheme(const ColorScheme &c) {
	for(int i = 0; i < 14; i++)
		colorPicker[i]->SetColour(c[i]);
}

void SettingsDialog::GetColorScheme(ColorScheme &c) {
	for(int i = 0; i < 14; i++)
		c[i] = colorPicker[i]->GetColour();
}

void SettingsDialog::UpdateColorSchemePanel(wxUpdateUIEvent &e) {
	e.Enable(colorSelection->GetSelection() != 0);
}

void SettingsDialog::SelectColorScheme(wxCommandEvent &e) {
	int selection = e.GetInt();
	if(selection == 0)
		SetColorScheme(ColorScheme());
	else
		SetColorScheme(colors[selection - 1]);

}

void SettingsDialog::ResetColorScheme(wxCommandEvent&) {
	int selection = colorSelection->GetSelection();
	colors[selection - 1].SetDefault();
	SetColorScheme(colors[selection - 1]);
}

void SettingsDialog::ChangeColorScheme(wxColourPickerEvent &e) {
	int selection = colorSelection->GetSelection();
	colors[selection - 1][e.GetId() - ID_COLOR_FIRST] = e.GetColour();
}

void SettingsDialog::UpdateDirs(wxUpdateUIEvent &e) {
	e.Enable(!sameDirs->IsChecked());
}

void SettingsDialog::ResetMacroDir(wxCommandEvent&) {
	macroDir->SetPath(wxGetCwd() + "/macros/");
}

void SettingsDialog::OpenMacroDir(wxCommandEvent&) {
#ifdef _WIN32
	wxString cmd = wxString::Format("explorer.exe %s", macroDir->GetPath());
#else
	wxString cmd = wxString::Format("exo-open --launch FileManager %s &", macroDir->GetPath());
#endif
	system(cmd.c_str());
}

void SettingsDialog::SelectTool(wxListEvent &e) {
	wxString text = toolList->GetItemText(e.GetIndex(), 1);
	if(text == "ESC")
		keyList->SetSelection(0);
	else
		keyList->SetSelection(text[0] - 'A' + 1);
	keyList->Enable();
}

void SettingsDialog::SelectKey(wxCommandEvent &e) {
	int selectedKey = keyList->GetSelection();
	char text[4] = "ESC";
	if(selectedKey != 0) {
		text[0] = selectedKey - 1 + 'A';
		text[1] = '\0';
	}
	bool use = false;
	for(int i = 0; i < toolList->GetItemCount(); i++)
		if(toolList->GetItemText(i, 1) == text)
			use = true;
	if(!use)
		toolList->SetItem(toolList->GetFirstSelected(), 1, text);
	else
		wxMessageBox(wxString::Format(_("The hotkey %s is already in use"),
				text), _("OpenLayout"), wxICON_ERROR);
}

void SettingsDialog::UpdateCrosshairPanel(wxUpdateUIEvent &e) {
	e.Enable(measureShow->IsChecked());
}

void SettingsDialog::UpdateAutosavePanel(wxUpdateUIEvent &e) {
	e.Enable(autosaveEnabled->IsChecked());
}
