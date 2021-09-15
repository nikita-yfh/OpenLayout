#pragma once
#include <wx/collpane.h>
#include <string>
#include <vector>
#include "PCBFile.h"

using namespace std;

typedef wxColour Color;

enum{
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
enum {
	COLOR_C1,	// Copper-top
	COLOR_S1,	// Silkscreen-top
	COLOR_C2,	// Copper-bottom
	COLOR_S2,	// Silkscreen
	COLOR_I1,	// Copper-Inner 1
	COLOR_I2,	// Copper-Inner 2
	COLOR_O,	// Outline
	COLOR_BGR,	// Background
	COLOR_LINES,// Grid lines
	COLOR_DOTS,	// Grid dots
	COLOR_CON,	// Connections
	COLOR_VIA,	// Metallization

	COLOR_SELO,	// Selected object
	COLOR_SELR,	// Rect of selection
	COLOR_COUNT
};

enum {
	S_BOARD_ZOOM,
	S_DARK_GROUND,
	S_ALL_GROUND,
	S_TEST_CONNECTIONS,
	S_TEST_BLINKING,
	S_CTRL_CAPTURE_SIZE,
	S_LIMIT_TEXT_W,
	S_ALWAYS_READABLE,
	S_OPTIMIZE,
	S_ANCHOR_LEFT_TOP,
	S_ANCHOR_EXPORT,
	S_COUNT
};

struct ColorScheme {
	ColorScheme();

	Color colors[COLOR_COUNT];

	void reset();
};


struct Settings {
	Settings();
	void SetDefault();
	static string GetDefaultMacroPath();
	bool gen_settings[S_COUNT];

	uint8_t undo;

	bool same_dir;
	string lay_export;
	string gbr_export;
	string bmp_export;
	string hpgl_export;
	string scanned_copies;

	string macro_dir;

	uint8_t units;
	uint8_t drill;
	uint8_t grid_style;

	ColorScheme colors[3]; //user color schemes
	uint8_t s_color_scheme; //0-Standart; 1-User1; 2-User2; 3-User3;

	ColorScheme get_current_colors();
	Color get_color(uint8_t type);

	uint16_t copper_thickness;
	uint16_t temp_enhance;

	char tool_keys[TOOL_COUNT]; //key bindings for tools

	static const char *tool_names[TOOL_COUNT];

	bool show_45_lines;
	bool ccoord_show;
	bool ccoord_big;
	bool ccoord_tp;
	bool ccoord_light;

	bool autosave;
	uint16_t autosave_timer;

	vector<Vec2>pad_sizes;
	vector<Vec2>smd_sizes;
	vector<float>track_sizes;

	vector<double>grids;
	double grids_bind[9]; //for keys 1-9


	uint8_t sub_grid;
	bool show_grid;

	float custom_rotation_angle;
	float rotation_angle;

	uint8_t circle_quality;

};
