#pragma once
#include "File.h"
#include "Vec2.h"
#include "PadSize.h"
#include "Array.h"
#include "ColorScheme.h" 

#include <QPoint>
#include <QString>
#include <QSize>

#define PATH_LENGTH 256
enum {
	TOOL_EDIT,
	TOOL_ZOOM,
	TOOL_TRACK,
	TOOL_THT_PAD,
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

enum {
	RUBBERBAND_DISABLED,
	RUBBERBAND_SMALL,
	RUBBERBAND_LARGE
};

enum {
	ANGLE_90,
	ANGLE_45,
	ANGLE_15,
	ANGLE_5,
	ANGLE_CUSTOM
};

enum {
    COLOR_SCHEME_DEFAULT,
    COLOR_SCHEME_1,
    COLOR_SCHEME_2,
    COLOR_SCHEME_3,

    COLOR_SCHEME_COUNT
};

const int MAX_UNDO_DEPTH = 50;

struct Settings {
	Settings();
	void SetDefault();
	void SetDefaultMacroPath();

	void Save(File &file) const;
	void Load(File &file);

	bool &operator[](uint8_t index);
	bool operator[](uint8_t index) const;

	bool boardZoom;
	bool darkGround;
	bool allGround;
	bool testConnections;
	bool testBlinking;
	bool ctrlCaptureSize;
	bool limitTextHeight;
	bool alwaysReadable;
	bool optimize;
	bool originLeftTop;
	bool originExport;

	uint8_t undoDepth;

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

	ColorScheme colors[COLOR_SCHEME_COUNT]; //user color schemes
	uint8_t selectedColorScheme;

	uint16_t copperThickness;
	uint16_t tempEnhance;

    Qt::Key toolKeys[TOOL_COUNT]; //key bindings for tools

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
	uint8_t rotationAngleSel;

	uint8_t selectedTool;
	bool metallization;
	uint8_t padShape;
	bool rectFill;
	bool transparent;
	bool capture;
	uint8_t rubberband;
	float groundDistance;

	QPoint windowPos;
	QSize windowSize;

	bool showSelectorPanel;
	bool showComponentsPanel;
	bool showPropertiesPanel;
	bool showDRCPanel;
	bool showMacroPanel;

	uint8_t GetSubGrid() const;
	float GetRotationAngle() const;
	ColorScheme &GetColorScheme();
	const ColorScheme &GetColorScheme() const;

	QString GetGridStr(double grid) const;
	QString GetStr(float value) const;
	QString GetStr(const Vec2 &value) const;
	QString GetStr(const PadSize &value) const;

	double ConvertToUnits(double value) const;
	double ConvertFromUnits(double value) const;
};

inline bool &Settings::operator[](uint8_t index) {
	return *(&boardZoom + index);
}

inline bool Settings::operator[](uint8_t index) const {
	return *(&boardZoom + index);
}

