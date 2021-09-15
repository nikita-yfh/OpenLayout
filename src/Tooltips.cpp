#include "Tooltips.h"
#include "OpenLayoutApp.h"
#include <wx/frame.h>
TextTooltip::TextTooltip(wxWindow *parent):
		wxDialog(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSTAY_ON_TOP|wxWANTS_CHARS|wxNO_BORDER|wxFRAME_FLOAT_ON_PARENT){
	SetCanFocus(false);
	wxBoxSizer *sizer=new wxBoxSizer(wxVERTICAL);
	text=new wxStaticText(this,wxID_ANY,"test");
	sizer->Add(text,1,wxEXPAND|wxLEFT|wxRIGHT,2);
	SetSizer(sizer);
}
void TextTooltip::Show(Vec2i mouse,wxString t){
	text->SetLabel(t);
	wxDialog::Show(true);
	SetPosition({mouse.x+20,mouse.y+20});
	Fit();
}
void TextTooltip::Hide(){
	wxDialog::Show(false);
}

MeasureTooltip::MeasureTooltip(wxWindow *parent):
		wxDialog(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSTAY_ON_TOP|wxWANTS_CHARS|wxNO_BORDER){
	SetCanFocus(false);
	wxBoxSizer *sizer=new wxBoxSizer(wxVERTICAL);
	for(int q=0;q<6;q++){
		texts[q]=new wxStaticText(this,wxID_ANY,"test");
		sizer->Add(texts[q],1,wxEXPAND|wxLEFT|wxRIGHT,2);
	}
	SetTransparent(200);
	SetSizer(sizer);
	UpdateColors();
}

void MeasureTooltip::UpdateColors(){

	wxColor colors_dark[]={
		{0,0,0},		//background
		{255,255,255},	//x,y
		{255,255,0},	//dx,dy
		{190,220,255}	//distance,angle
	};
	wxColor colors_light[]={
		{240,240,210},	//background
		{0,0,0},		//x,y
		{0,0,255},		//dx,dy
		{255,0,0}		//distance,angle
	};
	bool &light=SETTINGS.ccoord_light;
	SetBackgroundColour(light?colors_light[0]:colors_dark[0]);
	for(int q=0;q<6;q++)
		texts[q]->SetForegroundColour((light?colors_light:colors_dark)[q/2+1]);
}
void MeasureTooltip::Show(Vec2i mouse,Vec2 v){
	for(int q=0;q<6;q++)
		texts[q]->Show(q<2); //enable 0 and 1 texts

	texts[0]->SetLabel(wxString::Format("X:\t%.3f\t%s",v.x,_("mm")));	//x
	texts[1]->SetLabel(wxString::Format("Y:\t%.3f\t%s",v.y,_("mm")));	//y
	Fit();
	wxDialog::Show(true);
	SetPosition({mouse.x+20,mouse.y+20});
}
void MeasureTooltip::Show(Vec2i mouse,Vec2 v1,Vec2 v2,bool show_angle){
	for(int q=0;q<5;q++)
		texts[q]->Show(true);
	texts[5]->Show(show_angle);

	texts[0]->SetLabel(wxString::Format("X:\t%.3f\t%s",v1.x,_("mm")));	//x
	texts[1]->SetLabel(wxString::Format("Y:\t%.3f\t%s",v1.y,_("mm")));	//y

	texts[2]->SetLabel(wxString::Format("dX:\t%.3f\t%s",v2.x-v1.x,_("mm")));	//dx
	texts[3]->SetLabel(wxString::Format("dY:\t%.3f\t%s",v2.y-v1.y,_("mm")));	//dy

	float dist=Distance(v1,v2);
	texts[4]->SetLabel(wxString::Format("%s:\t%.3f\t%s",_("Dist"),dist,_("mm")));	//distance
	if(show_angle){
		float angle=deg(get_angle_v(v2-v1));
		texts[5]->SetLabel(wxString::Format("%s:\t%.1f\t%s",_("Angle"),angle,_("mm")));	//angle
	}
	Fit();
	wxDialog::Show(true);
	SetPosition({mouse.x+20,mouse.y+20});
}
void MeasureTooltip::Hide(){
	wxDialog::Show(false);
}
