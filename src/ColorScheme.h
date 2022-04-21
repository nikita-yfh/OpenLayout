#pragma once
#include <wx/collpane.h>
#include "File.h"

typedef wxColour Color;
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

struct ColorScheme {
	ColorScheme();

	Color colors[COLOR_COUNT];

	void ReadColor(File &file, Color &color);
	void WriteColor(File &file, const Color &color)const;

	void Save(File &file) const;
	void Load(File &file);

	void SetDefault();
};
