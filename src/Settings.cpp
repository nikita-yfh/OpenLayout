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
							 
	boardZoom		= false;
	darkGround		= true;
	allGround		= false;
	testConnections	= false;
	testBlinking	= true;
	ctrlCaptureSize	= true;
	limitTextHeight	= false;
	alwaysReadable	= true;
	optimize		= true;
	originLeftTop	= false;
	originExport	= false;

	undoDepth = 50;

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
	rotationAngleSel = 1; // 45 deg
	
	transparent = false;
	capture = true;
	rubberband = RUBBERBAND_DISABLED;

	const char defKeys[TOOL_COUNT]= {
		ESCAPE, 'Z', 'L', 'P',
		'S', 'R', 'Q', 'F',
		'N', 'T', 'C', 'A',
		'X', 'M', 'V', 'O'
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

	transparent = false;

	windowPos = wxPoint(-1, -1);
	windowSize = wxSize(800, 600);

	showSelectorPanel = false;
	showComponentsPanel = false;
	showPropertiesPanel = false;
	showDRCPanel = false;
	showMacroPanel = true;

}
void Settings::SetDefaultMacroPath() {
	getcwd(macroDir, PATH_LENGTH - 10);
	strncat(macroDir, "/macros/", PATH_LENGTH);
}
uint8_t Settings::GetSubGrid() const {
	switch(subGrid) {
		case SUBGRID_2: return 2;
		case SUBGRID_4: return 4;
		case SUBGRID_5: return 5;
		case SUBGRID_10: return 10;
	}
	return 1;
}

static ColorScheme def;

ColorScheme &Settings::GetColorScheme() {
	if(selectedColorScheme == 0)
		return def;
	return colors[selectedColorScheme - 1];
}

const ColorScheme &Settings::GetColorScheme() const {
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

void Settings::Save(File &file) const {
	file.Write<bool>(boardZoom);
	file.Write<bool>(darkGround);
	file.Write<bool>(allGround);
	file.Write<bool>(testConnections);
	file.Write<bool>(testBlinking);
	file.Write<bool>(ctrlCaptureSize);
	file.Write<bool>(limitTextHeight);
	file.Write<bool>(alwaysReadable);
	file.Write<bool>(optimize);
	file.Write<bool>(originLeftTop);
	file.Write<bool>(originExport);
	file.Write<uint8_t>(undoDepth);
	file.Write<bool>(sameDir);
	file.WriteString(layExport);
	file.WriteString(gbrExport);
	file.WriteString(bmpExport);
	file.WriteString(hpglExport);
	file.WriteString(scannedCopies);
	file.WriteString(macroDir);
	file.Write<uint8_t>(units);
	file.Write<uint8_t>(drill);
	file.Write<uint8_t>(gridStyle);
	for(int i = 0; i < 3; i++)
		colors[i].Save(file);
	file.Write<uint8_t>(selectedColorScheme);
	file.Write<uint16_t>(copperThickness);
	file.Write<uint16_t>(tempEnhance);
	for(int i = 0; i < TOOL_COUNT; i++)
		file.Write<char>(toolKeys[i]);
	file.Write<bool>(measure45Lines);
	file.Write<bool>(measureShow);
	file.Write<bool>(measureBig);
	file.Write<bool>(measureTp);
	file.Write<bool>(measureLight);
	file.Write<bool>(autosave);
	file.Write<uint16_t>(autosaveTimer);
	padSize.Save(file);
	smdSize.Save(file);
	file.Write<float>(trackSize);

	file.Write<uint32_t>(padSizes.Size());
	for(int i = 0; i < padSizes.Size(); i++)
		padSizes[i].Save(file);

	file.Write<uint32_t>(smdSizes.Size());
	for(int i = 0; i < smdSizes.Size(); i++)
		smdSizes[i].Save(file);

	file.Write<uint32_t>(trackSizes.Size());
	for(int i = 0; i < trackSizes.Size(); i++)
		file.Write<float>(trackSizes[i]);

	file.Write<uint32_t>(grids.Size());
	for(int i = 0; i < grids.Size(); i++)
		file.Write<double>(grids[i]);

	for(int i = 0; i < 9; i++)
		file.Write<double>(gridBind[i]);
	file.Write<uint8_t>(subGrid);
	file.Write<bool>(showGrid);
	file.Write<float>(customRotationAngle);
	file.Write<uint8_t>(rotationAngleSel);
	file.Write<uint8_t>(capture);
	file.Write<uint8_t>(rubberband);
	file.Write<uint16_t>(windowPos.x);
	file.Write<uint16_t>(windowPos.y);
	file.Write<uint16_t>(windowSize.x);
	file.Write<uint16_t>(windowSize.y);
	file.Write<bool>(showSelectorPanel);
	file.Write<bool>(showComponentsPanel);
	file.Write<bool>(showPropertiesPanel);
	file.Write<bool>(showDRCPanel);
	file.Write<bool>(showMacroPanel);
}


void Settings::Load(File &file) {
	boardZoom = file.Read<bool>();
	darkGround = file.Read<bool>();
	allGround = file.Read<bool>();
	testConnections = file.Read<bool>();
	testBlinking = file.Read<bool>();
	ctrlCaptureSize = file.Read<bool>();
	limitTextHeight = file.Read<bool>();
	alwaysReadable = file.Read<bool>();
	optimize = file.Read<bool>();
	originLeftTop = file.Read<bool>();
	originExport = file.Read<bool>();
	undoDepth = file.Read<uint8_t>();
	sameDir = file.Read<bool>();
	file.ReadString(layExport);
	file.ReadString(gbrExport);
	file.ReadString(bmpExport);
	file.ReadString(hpglExport);
	file.ReadString(scannedCopies);
	file.ReadString(macroDir);
	units = file.Read<uint8_t>();
	drill = file.Read<uint8_t>();
	gridStyle = file.Read<uint8_t>();
	for(int i = 0; i < 3; i++)
		colors[i].Load(file);
	selectedColorScheme = file.Read<uint8_t>();
	copperThickness = file.Read<uint16_t>();
	tempEnhance = file.Read<uint16_t>();
	for(int i = 0; i < TOOL_COUNT; i++)
		toolKeys[i] = file.Read<char>();
	measure45Lines = file.Read<bool>();
	measureShow = file.Read<bool>();
	measureBig = file.Read<bool>();
	measureTp = file.Read<bool>();
	measureLight = file.Read<bool>();
	autosave = file.Read<bool>();
	autosaveTimer = file.Read<uint16_t>();
	padSize.Load(file);
	smdSize.Load(file);
	trackSize = file.Read<float>();

	padSizes.Init(file.Read<uint32_t>());
	for(int i = 0; i < padSizes.Size(); i++)
		padSizes[i].Load(file);

	smdSizes.Init(file.Read<uint32_t>());
	for(int i = 0; i < smdSizes.Size(); i++)
		smdSizes[i].Load(file);

	trackSizes.Init(file.Read<uint32_t>());
	for(int i = 0; i < trackSizes.Size(); i++)
		trackSizes[i] = file.Read<float>();

	grids.Init(file.Read<uint32_t>());
	for(int i = 0; i < grids.Size(); i++)
		grids[i] = file.Read<double>();

	for(int i = 0; i < 9; i++)
		gridBind[i] = file.Read<double>();
	subGrid = file.Read<uint8_t>();
	showGrid = file.Read<bool>();
	customRotationAngle = file.Read<float>();
	rotationAngleSel = file.Read<uint8_t>();
	capture = file.Read<uint8_t>();
	rubberband = file.Read<uint8_t>();
	windowPos.x = file.Read<uint16_t>();
	windowPos.y = file.Read<uint16_t>();
	windowSize.x = file.Read<uint16_t>();
	windowSize.y = file.Read<uint16_t>();
	showSelectorPanel = file.Read<bool>();
	showComponentsPanel = file.Read<bool>();
	showPropertiesPanel = file.Read<bool>();
	showDRCPanel = file.Read<bool>();
	showMacroPanel = file.Read<bool>();
}
