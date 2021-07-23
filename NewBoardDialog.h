#ifndef NEWBOARDDIALOG_H
#define NEWBOARDDIALOG_H
#include "Board.h"

//(*Headers(NewBoardDialog)
#include <wx/bmpbuttn.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class NewBoardDialog: public wxDialog
{
	public:
		NewBoardDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~NewBoardDialog();

		//(*Declarations(NewBoardDialog)
		wxBitmapButton* Image1;
		wxBitmapButton* Image2;
		wxBitmapButton* Image3;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* Panel4;
		wxPanel* Panel5;
		wxPanel* Panel;
		wxRadioButton* Button1;
		wxRadioButton* Button2;
		wxRadioButton* Button3;
		wxSpinCtrlDouble* BorderSize;
		wxSpinCtrlDouble* Diameter;
		wxSpinCtrlDouble* SizeH;
		wxSpinCtrlDouble* SizeW;
		wxStaticText* AllSizeH;
		wxStaticText* AllSizeW;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText16;
		wxStaticText* StaticText17;
		wxStaticText* StaticText19;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* TextInput;
		//*)
		Board board;

	protected:

		//(*Identifiers(NewBoardDialog)
		static const long ID_BITMAPBUTTON1;
		static const long ID_RADIOBUTTON1;
		static const long ID_BITMAPBUTTON2;
		static const long ID_RADIOBUTTON2;
		static const long ID_BITMAPBUTTON3;
		static const long ID_RADIOBUTTON3;
		static const long ID_PANEL2;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRLDOUBLE1;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRLDOUBLE2;
		static const long ID_STATICTEXT6;
		static const long ID_PANEL1;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_SPINCTRLDOUBLE3;
		static const long ID_STATICTEXT9;
		static const long ID_PANEL3;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT13;
		static const long ID_SPINCTRLDOUBLE5;
		static const long ID_STATICTEXT14;
		static const long ID_PANEL4;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_STATICTEXT19;
		static const long ID_PANEL5;
		static const long ID_PANEL6;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL1;
		//*)

	private:

		//(*Handlers(NewBoardDialog)
		void RecalcSize(wxSpinDoubleEvent& event);
		void ChangeName(wxSpinDoubleEvent& event);
		void OnImage1Click(wxCommandEvent& event);
		void OnImage2Click(wxCommandEvent& event);
		void OnImage3Click(wxCommandEvent& event);
		void ChangeText(wxCommandEvent& event);
		//*)
		void SetType(BoardType type);
		DECLARE_EVENT_TABLE()
};

#endif
