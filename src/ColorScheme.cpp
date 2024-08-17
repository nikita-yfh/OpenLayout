#include "ColorScheme.h"
#include "Settings.h"

#include <QtOpenGL>

static const float DARKER_COEF = 5.0f / 3.0f * 100.0f;

static void SetGLColor(const Color &color) {
    glColor4ub(color.red(), color.green(), color.blue(), color.alpha());
}

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
	Color color(colors[index]);
    color.setAlpha(50);
    SetGLColor(color);
}
void ColorScheme::SetGroundColor(uint8_t index) const {
    SetGLColor(colors[index].darker(DARKER_COEF));
}
void ColorScheme::SetBackgroundClearColor() const {
    SetGLColor(colors[COLOR_BGR].darker(DARKER_COEF));
}
void ColorScheme::SetDrillingsColor(uint8_t drill) const {
	if(drill == DRILL_BGR)
		SetColor(COLOR_BGR);
	else if(drill == DRILL_BLACK)
        SetGLColor(Qt::black);
	else
        SetGLColor(Qt::white);
}
void ColorScheme::WriteColor(File &file, const Color &color) const {
	file.Write<QRgb>(color.rgba());
}
void ColorScheme::ReadColor(File &file, Color &color) {
	color.setRgba(file.Read<QRgb>());
}
void ColorScheme::Save(File &file) const {
	for(int i = 0; i < COLOR_COUNT; i++)
		WriteColor(file, colors[i]);
}
void ColorScheme::Load(File &file) {
	for(int i = 0; i < COLOR_COUNT; i++)
		ReadColor(file, colors[i]);
}

