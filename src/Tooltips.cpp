#include "Tooltips.h"

TextTooltip::TextTooltip(wxWindow *parent):
		wxDialog(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSTAY_ON_TOP|wxWANTS_CHARS|wxNO_BORDER){
	SetCanFocus(false);
	wxBoxSizer *sizer=new wxBoxSizer(wxVERTICAL);
	text=new wxStaticText(this,wxID_ANY,wxEmptyString);
	sizer->Add(text,1,wxEXPAND|wxLEFT|wxRIGHT,2);
	SetSizer(sizer);
}
void TextTooltip::Show(int mousex,int mousey,wxString t){
	text->SetLabel(t);
	wxDialog::Show(true);
	SetPosition({mousex,mousey});
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
		texts[q]=new wxStaticText(this,wxID_ANY,wxEmptyString);
		sizer->Add(texts[q],0,wxEXPAND);
	}
	SetSizerAndFit(sizer);
}

void MeasureTooltip::UpdateTheme(bool light){

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
	SetBackgroundColour(light?colors_light[0]:colors_dark[0]);
	for(int q=0;q<6;q++)
		texts[q]->SetForegroundColour((light?colors_light:colors_dark)[q/2+1]);
}
void MeasureTooltip::Show(int mousex,int mousey,float x,float y){
	wxDialog::Show(true);
	SetPosition({mousex,mousey});

	//texts[0]->Set

	Fit();
}
void MeasureTooltip::Hide(){
	wxDialog::Show(false);
}
