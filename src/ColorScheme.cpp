#include "ColorScheme.h"
#include "Settings.h"
#include <wx/glcanvas.h>

union ColorU {
	uint32_t color;
	struct {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;
	};
	ColorU(const wxColor &_color) {
		color = _color.GetRGBA();
	}
};

ColorScheme::ColorScheme() {
	SetDefault();
}

void ColorScheme::SetDefault() {
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
	colors[COLOR_SELO]		= Color(255,170,255);
	colors[COLOR_SELR]		= Color(0,	255,255);
}
void ColorScheme::SetColor(uint8_t index) const {
	ColorU color(colors[index]);
	glColor4ub(color.red, color.green, color.blue, 50);
}
void ColorScheme::SetGroundColor(uint8_t index) const {
	ColorU color(colors[index]);
	glColor4f(color.red / 425.0f, color.green / 425.0f, color.blue / 425.0f, color.alpha / 255.0f);
}
void ColorScheme::SetBackgroundClearColor() const {
	ColorU color(colors[COLOR_BGR]);
	glClearColor(color.red / 425.0f, color.green / 425.0f, color.blue / 425.0f, color.alpha / 255.0f);
}
void ColorScheme::SetDrillingsColor(uint8_t drill) const {
	if(drill == DRILL_BGR)
		SetColor(COLOR_BGR);
	else if(drill == DRILL_BLACK)
		glColor4ub(0, 0, 0, 255);
	else
		glColor4ub(255, 255, 255, 255);
}
void ColorScheme::WriteColor(File &file, const Color &color) const {
	file.Write<uint32_t>(color.GetRGBA());
}
void ColorScheme::ReadColor(File &file, Color &color) {
	color.SetRGBA(file.Read<uint32_t>());
}
void ColorScheme::Save(File &file) const {
	for(int i = 0; i < COLOR_COUNT; i++)
		WriteColor(file,colors[i]);
}
void ColorScheme::Load(File &file) {
	for(int i = 0; i < COLOR_COUNT; i++)
		ReadColor(file,colors[i]);
}

