#ifndef SETTINGS_H
#define SETTINGS_H
#include <wx/collpane.h>
#include <string>
#include <vector>
#include "Board.h"

using namespace std;

typedef wxColour Color;

#define TOOL_COUNT 16

enum class Unit {
    mm,mil
};

enum class Drillings {
    bgr,white,black
};

enum class GridStyle{
	lines,dots
};

enum class ColorType {
    c1,		// Copper-top
    c2,		// Copper-bottom
    s1,		// Silkscreen-top
    s2,		// Silkscreen
    i1,		// Copper-Inner 1
    i2,		// Copper-Inner 2
    o,		// Outline
    bgr,	// Background
    lines,	// Grid lines
    dots,	// Grid dots
    con,	// Connections
    via,	// Metallization
    COUNT
};

enum class GenSettings {
    board_zoom,
    dark_ground,
    all_ground,
    test_connections,
    test_blinking,
    ctrl_capture_size,
    limit_text_w,
    always_readable,
    optimize,
    anchor_left_top,
    anchor_export,
    COUNT
};

struct ColorScheme {
    ColorScheme();

    Color &get(ColorType t);
    Color get(ColorType t) const;

    Color colors[(int)ColorType::COUNT];

    void reset();
};


struct Settings {
    Settings();
    void SetDefault();
    static string GetDefaultMacroPath();
    bool gen_settings[(int)GenSettings::COUNT];

    bool get_gen(GenSettings t) const;
    bool &get_gen(GenSettings t);

    uint8_t undo;

    bool same_dir;
    string lay_export;
    string gbr_export;
    string bmp_export;
    string hpgl_export;
    string scanned_copies;

    string macro_dir;

    Unit units;
    Drillings drill;
    GridStyle grid_style;

    ColorScheme colors[3]; //user color schemes
    uint8_t s_color_scheme; //0-Standart; 1-User1; 2-User2; 3-User3;

    ColorScheme get_current_colors();

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

    vector<PadSize>pad_sizes;
    vector<SMDSize>smd_sizes;
    vector<float>track_sizes;

    vector<float>grids;

    uint8_t sub_grid;
};

#endif // SETTINGS_H
