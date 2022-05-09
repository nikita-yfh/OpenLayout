#include "Settings.h"
#include <stdlib.h>
#include <wx/spinctrl.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

Settings::Settings() {
	SetDefault();
}
void Settings::SetDefault() {
	//Default settings from Sprint Layout
	for(int i = 0; i < 3; i++)
		colors[i].SetDefault();
	selectedColorScheme = 0; //default

	undoCount = 50;

	sameDir = true;
	*layExport = '\0';
	*gbrExport = '\0';
	*bmpExport = '\0';
	*hpglExport = '\0';
	*scannedCopies = '\0';
	grids.Clear();
	SetDefaultMacroPath();

	units = UNITS_MM;
	drill = DRILL_BGR;
	gridStyle = GRID_LINES;

	copperThickness = 35;
	tempEnhance = 20;

	measure45Lines = true;
	measureShow = true;
	measureBig = true;
	measureLight = false;
	measureTp = true;
	autosave = true;
	autosaveTimer = 5;

	subGrid = SUBGRID_4;
	showGrid = true;

	customRotationAngle = 22.5f;
	rotationAngle = 45.0f;

	circleQuality = 40;

	const char defKeys[TOOL_COUNT]= {
		27, //escape
		'Z', 'L', 'P', 'S',
		'R', 'Q', 'F', 'N',
		'T', 'C', 'A', 'X',
		'M', 'V', 'O'
	};
	memcpy(toolKeys, defKeys, sizeof(defKeys));

	const float defTrackSizes[] = {
		0.15f,
		0.2f,
		0.3f,
		0.4f,
		0.6f,
		0.8f,
		1.0f,
		1.2f,
		1.4f
	};
	const PadSize defPadSizes[] = {
		{0.8f, 0.4f},
		{1.0f, 0.5f},
		{1.4f, 0.8f},
		{1.4f, 1.0f},
		{1.6f, 0.8f},
		{1.6f, 1.0f},
		{2.0f, 0.8f},
		{2.0f, 1.0f},
		{2.4f, 1.2f},
		{3.0f, 1.4f}
	};
	const Vec2 defSmdSizes[] = {
		{0.3f, 0.3f},
		{0.5f, 0.6f},
		{0.6f, 0.9f},
		{0.8f, 0.9f},
		{0.9f, 1.6f},
		{1.0f, 1.2f},
		{1.2f, 1.7f},
		{1.2f, 2.7f},
		{1.2f, 3.2f}
	};

	trackSizes.Set(defTrackSizes, 8);
	padSizes.Set(defPadSizes, 10);
	smdSizes.Set(defSmdSizes, 9);

	trackSize = 0.8f;
	padSize.Set(1.8f, 0.6f);
	smdSize.Set(0.9f, 1.8f);

	gridBind[0] = 2.54;
	gridBind[1] = 1.27;
	gridBind[2] = 0.635;
	gridBind[3] = 0.3175;
	gridBind[4] = 0.15875;
	gridBind[5] = 0.079375;
	gridBind[6] = 0.0396875;
	gridBind[7] = 1.0;
	gridBind[8] = 0.1;
}
void Settings::SetDefaultMacroPath() {
	getcwd(macroDir, PATH_LENGTH - 10);
	strncat(macroDir, "/macros/", PATH_LENGTH);
}
ColorScheme &Settings::GetColorScheme() {
	static ColorScheme def;
	if(selectedColorScheme == 0)
		return def;
	return colors[selectedColorScheme - 1];
}

double Settings::ShowInputGridDialog(wxWindow *parent, double init) const {
	wxDialog *dialog = new wxDialog(parent, wxID_ANY, _("New grid"));

	wxString list[] = {_("mm"), _(L"\x00b5m"), _("mil")};
	wxChoice *unitsList = new wxChoice(dialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, list);

	char mm[50];
	if(units == UNITS_MIL) {
		init /= 0.0254;
		unitsList->SetSelection(2);
	}else if(snprintf(mm, 50, "%g", init) > 6) {
		init *= 1000.0;
		unitsList->SetSelection(1);
	} else
		unitsList->SetSelection(0);

	wxSpinCtrlDouble *input = new wxSpinCtrlDouble(dialog, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 0.001, 999999.0, init, 0.1);
	input->SetDigits(6);

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(input, 1, wxALL, 5);
	sizer->Add(unitsList, 0, wxALL, 5);
	content->Add(sizer, 1, wxEXPAND | wxALL, 10);
	content->Add(dialog->CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);

	input->SetFocus();
	dialog->SetSizerAndFit(content);
	if(dialog->ShowModal() != wxID_OK)
		return 0.0;
	int selection = unitsList->GetSelection();
	double value = input->GetValue();
	if(selection == 2) //mil
		value *= 0.0254;
	else if(selection == 1) //um
		value *= 0.001;
	if(value > 10.0f) {
		wxMessageBox(_("The grid must not exceed 10 mm"), wxEmptyString, wxICON_ERROR);
		return 0.0;
	}
	return value;
}

void Settings::ShowGridBinderDialog(wxWindow *parent) {
	wxDialog *dialog = new wxDialog(parent, wxID_ANY, _("Grid values for Keys 1..9"));
	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);

	double grids[sizeof(gridBind) / sizeof(double)];
	memcpy(grids, gridBind, sizeof(grids));

	wxListView *keyList = new wxListView(dialog, wxID_ANY, wxDefaultPosition,
			wxDefaultSize, wxLC_REPORT | wxBORDER_SIMPLE);

	keyList->AppendColumn(_("Key"), wxLIST_FORMAT_LEFT, 50);
	keyList->AppendColumn(_("Grid"), wxLIST_FORMAT_LEFT, 100);
	for(int i = 0; i < 9; i++) {
		keyList->InsertItem(i, char(i + 1));
		keyList->SetItem(i, 0, char(i + '1'));
		keyList->SetItem(i, 1, GetGridStr(grids[i]));
	}
	content->Add(keyList, 1, wxEXPAND | wxALL, 10);

	wxButton *change = new wxButton(dialog, wxID_ANY, _("Change grid"));
	change->Enable(false);
	change->Bind(wxEVT_BUTTON, [&](wxCommandEvent &e) {
		int n = keyList->GetFirstSelected();
		double grid = ShowInputGridDialog(dialog, grids[n]);
		if(grid != 0.0) {
			keyList->SetItem(n, 1, GetGridStr(grid));
			grids[n] = grid;
		}
	});
	keyList->Bind(wxEVT_LIST_ITEM_SELECTED, [&](wxCommandEvent&) {
		change->Enable(true);
	});
	content->Add(change, 0, wxEXPAND | wxALL, 10);

	content->Add(dialog->CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 5);
	dialog->SetSizerAndFit(content);

	if(dialog->ShowModal() == wxID_OK)
		memcpy(gridBind, grids, sizeof(gridBind));
}

wxString Settings::GetGridStr(double grid) const {
	if(units == UNITS_MIL)
		return wxString::Format("%g mil", grid / 0.0254, _("mil"));
	char mm[50];
	if(snprintf(mm, 50, "%g", grid) > 6) //if µm more compact
		return wxString::Format("%g %s", grid * 1000.0, _(L"\x00b5m"));
	else
		return wxString::Format("%g %s", grid, _("mm"));
}

wxString Settings::GetStr(float value) const {
	if(units == UNITS_MIL)
		return wxString::Format("%.01f %s", value / 0.0254, _("mil"));
	return wxString::Format("%.02f %s", value, _("mm"));
}

wxString Settings::GetStr(const Vec2 &value) const {
	if(units == UNITS_MIL)
		return wxString::Format("%.01f x %.01f %s", value.x / 0.0254, value.y / 0.0254, _("mil"));
	return wxString::Format("%.02f x %.02f %s", value.x, value.y, _("mm"));
}

wxString Settings::GetStr(const PadSize &value) const {
	return GetStr(Vec2(value.out, value.in));
}

double Settings::ConvertToUnits(double value) const {
	if(units == UNITS_MIL)
		return value / 0.0254;
	return value;
}

double Settings::ConvertFromUnits(double value) const {
	if(units == UNITS_MIL)
		return value * 0.0254;
	return value;
}

