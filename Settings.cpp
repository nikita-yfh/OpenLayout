#include "Settings.h"
#include <wx/utils.h>

const char* Settings::tool_names[TOOL_COUNT]={
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

const char Settings::def_keys[TOOL_COUNT]={
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

ColorScheme::ColorScheme(){reset();}

void ColorScheme::reset(){
	get(ColorType::c1)		= Color(30, 106,249);
	get(ColorType::c2)		= Color(0,  186,0);
	get(ColorType::s1)		= Color(255,0,  0);
	get(ColorType::s2)		= Color(225,215,4);
	get(ColorType::i1)		= Color(194,124,20);
	get(ColorType::i2)		= Color(238,182,98);
	get(ColorType::o)		= Color(255,255,255);
	get(ColorType::bgr)		= Color(255,255,255);
	get(ColorType::lines)	= Color(70, 70, 70);
	get(ColorType::dots)	= Color(170,170,170);
	get(ColorType::con) 	= Color(215,215,215);
	get(ColorType::via)		= Color(81, 227,253);
}
Color ColorScheme::get(ColorType n) const{
	return colors[static_cast<int>(n)];
}
Color &ColorScheme::get(ColorType n){
	return colors[static_cast<int>(n)];
}
Settings::Settings(){SetDefault();}
void Settings::SetDefault(){
	for(int q=0;q<3;q++)
		colors[q].reset();
	s_color_scheme=0; //Default

	undo=50;
	get_gen(GenSettings::board_zoom)			=false;
	get_gen(GenSettings::dark_ground)			=true;
	get_gen(GenSettings::all_ground)			=false;
	get_gen(GenSettings::test_connections)		=false;
	get_gen(GenSettings::test_blinking)			=true;
	get_gen(GenSettings::ctrl_capture_size)		=true;
	get_gen(GenSettings::limit_text_w)			=false;
	get_gen(GenSettings::always_readable)		=true;
	get_gen(GenSettings::optimize)				=true;
	get_gen(GenSettings::anchor_left_top)		=false;
	get_gen(GenSettings::anchor_export)			=false;

	same_dir=true;
	lay_export.clear();
	gbr_export.clear();
	bmp_export.clear();
	hpgl_export.clear();
	scanned_copies.clear();

	macro_dir=GetDefaultMacroPath();

	units=Unit::mm;
	drill=Drillings::bgr;

	copper_thickness=35;
	temp_enhance=20;

	show_45_lines=true;
	ccoord_show=true;
	ccoord_big=true;
	ccoord_light=false;
	ccoord_tp=true;
	autosave=true;
	autosave_timer=5;


	memcpy(tool_keys,def_keys,sizeof(def_keys));

}
bool Settings::get_gen(GenSettings t)const{
	return gen_settings[static_cast<int>(t)];
}
bool &Settings::get_gen(GenSettings t){
	return gen_settings[static_cast<int>(t)];
}
string Settings::GetDefaultMacroPath(){
	// You can customize it there
	string macro_dir(wxGetCwd());
	macro_dir+="/macros/";
	return macro_dir;
}
