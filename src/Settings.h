#pragma once
#include <wx/dialog.h>
#include "File.h"
#include "Vec2.h"
#include "PadSize.h"
#include "Array.h"
#include "ColorScheme.h" 

#define PATH_LENGTH 256

enum {
	TOOL_EDIT,
	TOOL_ZOOM,
	TOOL_TRACK,
	TOOL_PAD,
	TOOL_SMD_PAD,
	TOOL_CIRCLE,
	TOOL_RECTANGLE,
	TOOL_ZONE,
	TOOL_FORM,
	TOOL_TEXT,
	TOOL_SOLDER_MASK,
	TOOL_CONNECTIONS,
	TOOL_AUTOROUTE,
	TOOL_TEST,
	TOOL_MEASURE,
	TOOL_PHOTOVIEW,

	TOOL_COUNT //16
};
enum {
	UNITS_MM,
	UNITS_MIL
};

enum {
	DRILL_BGR,
	DRILL_WHITE,
	DRILL_BLACK
};

enum {
	GRID_LINES,
	GRID_DOTS
};
enum {
	SUBGRID_OFF,
	SUBGRID_2,
	SUBGRID_4,
	SUBGRID_5,
	SUBGRID_10,
};

struct Settings {
	Settings();
	void SetDefault();
	void SetDefaultMacroPath();

	uint8_t undoCount;

	bool sameDir;
	char layExport[PATH_LENGTH];
	char gbrExport[PATH_LENGTH];
	char bmpExport[PATH_LENGTH];
	char hpglExport[PATH_LENGTH];
	char scannedCopies[PATH_LENGTH];

	char macroDir[PATH_LENGTH];

	uint8_t units;
	uint8_t drill;
	uint8_t gridStyle;

	ColorScheme colors[3]; //user color schemes
	uint8_t selectedColorScheme; //0-Standart; 1-User1; 2-User2; 3-User3;

	const ColorScheme &GetCurrentColors();

	uint16_t copperThickness;
	uint16_t tempEnhance;

	char toolKeys[TOOL_COUNT]; //key bindings for tools

	bool measure45Lines;
	bool measureShow;
	bool measureBig;
	bool measureTp;
	bool measureLight;

	bool autosave;
	uint16_t autosaveTimer;

	PadSize padSize;
	Vec2 smdSize;
	float trackSize;

	UniqueSortArray<PadSize> padSizes;
	UniqueSortArray<Vec2> smdSizes;
	UniqueSortArray<float> trackSizes;

	UniqueSortArray<double> grids;
	double gridBind[9]; //for keys 1-9

	uint8_t subGrid;
	bool showGrid;

	float customRotationAngle;
	float rotationAngle;

	uint8_t circleQuality;

	ColorScheme &GetColorScheme();

	wxString GetGridStr(double grid) const;
	wxString GetStr(float value) const;
	wxString GetStr(const Vec2 &value) const;
	wxString GetStr(const PadSize &value) const;

	double ShowInputGridDialog(wxWindow *parent, double init) const;
	void ShowGridBinderDialog(wxWindow *parent);

	double ConvertToUnits(double value) const;
	double ConvertFromUnits(double value) const;
};
