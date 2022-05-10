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

