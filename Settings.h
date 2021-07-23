#ifndef SETTINGS_H
#define SETTINGS_H
#include <wx/collpane.h>

typedef wxColour Color;

enum class Unit{
	mm,mil
};

enum class Drillings{
	bgr,white,black
};

struct ColorScheme{
	ColorScheme();

	Color c1;		// Copper-top
	Color c2;		// Copper-bottom
	Color s1;		// Silkscreen-top
	Color s2;		// Silkscreen
	Color i1;		// Copper-Inner 1
	Color i2;		// Copper-Inner 2
	Color  o;		// Outline

	Color bgr;		// Background
	Color lines;	// Grid lines
	Color dots;		// Grid dots
	Color con;		// Connections
	Color via;		// Metallization
	void reset();
};

struct Settings{
	Settings();
	void SetDefault();
	bool board_zoom;
	bool dark_ground;
	bool all_ground;
	bool test_connections;
	bool test_blinking;
	bool ctrl_capture_size;
	bool limit_text_w;
	bool always_readable;
	bool optimize;
	bool anchor_left_top;
	bool anchor_export;

	bool same_dir;


	Unit units;
	Drillings drill;

	ColorScheme colors[3];
	uint8_t s_color_scheme;
};

#endif // SETTINGS_H
