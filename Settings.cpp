#include "Settings.h"

ColorScheme::ColorScheme(){reset();}

void ColorScheme::reset(){
	c1		= Color(30, 106,249);
	c2		= Color(0,  186,0);
	s1		= Color(255,0,  0);
	s2		= Color(225,215,4);
	i1		= Color(194,124,20);
	i2		= Color(238,182,98);
	o		= Color(255,255,255);
	bgr		= Color(255,255,255);
	lines	= Color(70, 70, 70);
	dots	= Color(170,170,170);
	con 	= Color(215,215,215);
	via		= Color(81, 227,253);

}

Settings::Settings(){
}
void Settings::SetDefault(){
	for(int q=0;q<3;q++)
		colors[q].reset();
	s_color_scheme=0; //Default
}
