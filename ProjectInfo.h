#ifndef PROJECTINFO_H
#define PROJECTINFO_H

//(*Headers(ProjectInfo)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class ProjectInfo: public wxDialog
{
	public:
		void Set(const char *title,const char *author, const char *company, const char *comment);
		void Get(char *title,char *author, char *company, char *comment);
		ProjectInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ProjectInfo();

		//(*Declarations(ProjectInfo)
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxTextCtrl* TextAuthor;
		wxTextCtrl* TextComment;
		wxTextCtrl* TextCompany;
		wxTextCtrl* TextTitle;
		//*)

	protected:

		//(*Identifiers(ProjectInfo)
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL2;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		//*)

	private:
		//(*Handlers(ProjectInfo)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
