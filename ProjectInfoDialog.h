#ifndef PROJECTINFO_H
#define PROJECTINFO_H
#include "PCBFile.h"

//(*Headers(ProjectInfoDialog)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class ProjectInfoDialog: public wxDialog
{
	public:
		void Set(const ProjectInfo info);
		void Get(ProjectInfo &info);
		ProjectInfoDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ProjectInfoDialog();

		//(*Declarations(ProjectInfoDialog)
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

		//(*Identifiers(ProjectInfoDialog)
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
		//(*Handlers(ProjectInfoDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
