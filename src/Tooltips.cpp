#include "Tooltips.h"

TextTooltip::TextTooltip(wxWindow *parent):
		wxDialog(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSTAY_ON_TOP|wxWANTS_CHARS|wxNO_BORDER){
	SetCanFocus(false);
	text=new wxStaticText(this,wxID_ANY,wxEmptyString);
}
void TextTooltip::Show(int mousex,int mousey,wxString t){
	text->SetLabel(t);
	int tw,th; text->GetSize(&tw,&th);
	if(tw>0 && th>0){
		SetSize({tw+4,th});
		wxDialog::Show(true);
		text->SetPosition({2,0});
		SetPosition({mousex,mousey});
	}
}
void TextTooltip::Hide(){
	wxDialog::Show(false);
}
