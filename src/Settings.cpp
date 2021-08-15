#include "Settings.h"
#include <wx/utils.h>

const char* Settings::tool_names[TOOL_COUNT]= {
    "Edit",
    "Zoom",
    "Track",
    "Pad",
    "SMD-Pad",
    "Circle",
    "Rectangle",
    "Zone",
    "Special form",
    "Text",
    "Solder mask",
    "Connections",
    "Autoroute",
    "Test",
    "Measure",
    "Photoview"
};


ColorScheme::ColorScheme() {
    reset();
}

void ColorScheme::reset() {
    colors[COLOR_C1]		= Color(30, 106,249);
    colors[COLOR_C2]		= Color(0,  186,0);
    colors[COLOR_S1]		= Color(255,0,  0);
    colors[COLOR_S2]		= Color(225,215,4);
    colors[COLOR_I1]		= Color(194,124,20);
    colors[COLOR_I2]		= Color(238,182,98);
    colors[COLOR_O]			= Color(255,255,255);
    colors[COLOR_BGR]		= Color(0,  0,  0);
    colors[COLOR_LINES]		= Color(70, 70, 70);
    colors[COLOR_DOTS]		= Color(170,170,170);
    colors[COLOR_CON]		= Color(215,215,215);
    colors[COLOR_VIA]		= Color(81, 227,253);
}
Settings::Settings() {
    SetDefault();
}
void Settings::SetDefault() {
    //Default settings from Sprint Layout
    for(int q=0; q<3; q++)
        colors[q].reset();
    s_color_scheme=0; //Default

    undo=50;
    gen_settings[S_BOARD_ZOOM]			=false;
    gen_settings[S_DARK_GROUND]			=true;
    gen_settings[S_ALL_GROUND]			=false;
    gen_settings[S_TEST_CONNECTIONS]	=false;
    gen_settings[S_TEST_BLINKING]		=true;
    gen_settings[S_CTRL_CAPTURE_SIZE]	=true;
    gen_settings[S_LIMIT_TEXT_W]		=false;
    gen_settings[S_ALWAYS_READABLE]		=true;
    gen_settings[S_OPTIMIZE]			=true;
    gen_settings[S_ANCHOR_LEFT_TOP]		=false;
    gen_settings[S_ANCHOR_EXPORT]		=false;

    same_dir=true;
    lay_export.clear();
    gbr_export.clear();
    bmp_export.clear();
    hpgl_export.clear();
    scanned_copies.clear();
    grids.clear();
    grids= {0.6,0.7,0.964};
    macro_dir=GetDefaultMacroPath();

    units=UNITS_MM;
    drill=DRILL_BGR;
    grid_style=GRID_LINES;

    copper_thickness=35;
    temp_enhance=20;

    show_45_lines=true;
    ccoord_show=true;
    ccoord_big=true;
    ccoord_light=false;
    ccoord_tp=true;
    autosave=true;
    autosave_timer=5;

    sub_grid=4;


    const char def_keys[TOOL_COUNT]= {
        '\27', //escape
        'Z',
        'L',
        'P',
        'S',
        'R',
        'Q',
        'F',
        'N',
        'T',
        'C',
        'A',
        'X',
        'M',
        'V',
        'O'
    };
    memcpy(tool_keys,def_keys,sizeof(def_keys));

    smd_sizes= {
        {0.3f,0.3f},
        {0.5f,0.6f},
        {0.6f,0.9f},
        {0.9f,1.6f},
        {1.0f,1.2f},
        {1.2f,1.7f},
        {1.2f,2.7f},
        {1.2f,3.2f}
    };
    pad_sizes= {
        {0.8f,0.4f},
        {1.0f,0.5f},
        {1.4f,0.8f},
        {1.4f,1.0f},
        {1.6f,0.8f},
        {1.6f,1.0f},
        {2.0f,0.8f},
        {2.0f,1.0f},
        {2.4f,1.2f},
        {3.0f,1.4f}
    };
    track_sizes= {
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

}
string Settings::GetDefaultMacroPath() {
    // You can customize it there
    string macro_dir(wxGetCwd());
    macro_dir+="/macros/";
    return macro_dir;
}
ColorScheme Settings::get_current_colors() {
	static ColorScheme def;
    if(s_color_scheme==0)return def;
    else return colors[s_color_scheme-1];
}
Color Settings::get_color(uint8_t type) {
    return get_current_colors().colors[type];
}
