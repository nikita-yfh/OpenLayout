#ifndef SETTINGS_H
#define SETTINGS_H

//(*Headers(Settings)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
//*)

class Settings: public wxDialog
{
	public:

		Settings(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Settings();

		//(*Declarations(Settings)
		wxButton* Button1;
		wxCheckBox* CheckBox10;
		wxCheckBox* CheckBox11;
		wxCheckBox* CheckBox12;
		wxCheckBox* CheckBox1;
		wxCheckBox* CheckBox2;
		wxCheckBox* CheckBox3;
		wxCheckBox* CheckBox4;
		wxCheckBox* CheckBox5;
		wxCheckBox* CheckBox6;
		wxCheckBox* CheckBox7;
		wxCheckBox* CheckBox8;
		wxCheckBox* CheckBox9;
		wxChoice* Choice1;
		wxChoice* Choice2;
		wxChoice* Choice3;
		wxColourPickerCtrl* ColourPickerCtrl10;
		wxColourPickerCtrl* ColourPickerCtrl11;
		wxColourPickerCtrl* ColourPickerCtrl12;
		wxColourPickerCtrl* ColourPickerCtrl1;
		wxColourPickerCtrl* ColourPickerCtrl2;
		wxColourPickerCtrl* ColourPickerCtrl3;
		wxColourPickerCtrl* ColourPickerCtrl4;
		wxColourPickerCtrl* ColourPickerCtrl5;
		wxColourPickerCtrl* ColourPickerCtrl6;
		wxColourPickerCtrl* ColourPickerCtrl7;
		wxColourPickerCtrl* ColourPickerCtrl8;
		wxColourPickerCtrl* ColourPickerCtrl9;
		wxDirPickerCtrl* DirPickerCtrl1;
		wxDirPickerCtrl* DirPickerCtrl2;
		wxDirPickerCtrl* DirPickerCtrl3;
		wxDirPickerCtrl* DirPickerCtrl4;
		wxDirPickerCtrl* DirPickerCtrl5;
		wxNotebook* Notebook1;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* Panel4;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText15;
		wxStaticText* StaticText16;
		wxStaticText* StaticText17;
		wxStaticText* StaticText18;
		wxStaticText* StaticText19;
		wxStaticText* StaticText1;
		wxStaticText* StaticText20;
		wxStaticText* StaticText21;
		wxStaticText* StaticText22;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		//*)

	protected:

		//(*Identifiers(Settings)
		static const long ID_STATICTEXT1;
		static const long ID_CHOICE1;
		static const long ID_STATICTEXT2;
		static const long ID_CHOICE2;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_CHECKBOX4;
		static const long ID_CHECKBOX5;
		static const long ID_CHECKBOX6;
		static const long ID_CHECKBOX7;
		static const long ID_CHECKBOX8;
		static const long ID_CHECKBOX9;
		static const long ID_CHECKBOX10;
		static const long ID_CHECKBOX11;
		static const long ID_PANEL1;
		static const long ID_STATICTEXT3;
		static const long ID_CHOICE3;
		static const long ID_COLOURPICKERCTRL1;
		static const long ID_STATICTEXT4;
		static const long ID_COLOURPICKERCTRL2;
		static const long ID_STATICTEXT5;
		static const long ID_COLOURPICKERCTRL3;
		static const long ID_STATICTEXT6;
		static const long ID_COLOURPICKERCTRL4;
		static const long ID_STATICTEXT7;
		static const long ID_COLOURPICKERCTRL5;
		static const long ID_STATICTEXT8;
		static const long ID_COLOURPICKERCTRL6;
		static const long ID_STATICTEXT9;
		static const long ID_COLOURPICKERCTRL7;
		static const long ID_STATICTEXT10;
		static const long ID_COLOURPICKERCTRL8;
		static const long ID_STATICTEXT11;
		static const long ID_COLOURPICKERCTRL9;
		static const long ID_STATICTEXT12;
		static const long ID_COLOURPICKERCTRL10;
		static const long ID_STATICTEXT13;
		static const long ID_COLOURPICKERCTRL11;
		static const long ID_STATICTEXT15;
		static const long ID_COLOURPICKERCTRL12;
		static const long ID_STATICTEXT14;
		static const long ID_BUTTON1;
		static const long ID_PANEL2;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_STATICTEXT19;
		static const long ID_STATICTEXT20;
		static const long ID_DIRPICKERCTRL1;
		static const long ID_DIRPICKERCTRL2;
		static const long ID_DIRPICKERCTRL3;
		static const long ID_DIRPICKERCTRL4;
		static const long ID_DIRPICKERCTRL5;
		static const long ID_STATICTEXT21;
		static const long ID_CHECKBOX12;
		static const long ID_PANEL3;
		static const long ID_STATICTEXT22;
		static const long ID_PANEL4;
		static const long ID_NOTEBOOK1;
		//*)

	private:

		//(*Handlers(Settings)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
