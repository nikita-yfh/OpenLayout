#include "ProjectInfo.h"

//(*InternalHeaders(ProjectInfo)
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ProjectInfo)
const long ProjectInfo::ID_STATICTEXT2 = wxNewId();
const long ProjectInfo::ID_STATICTEXT3 = wxNewId();
const long ProjectInfo::ID_STATICTEXT4 = wxNewId();
const long ProjectInfo::ID_TEXTCTRL2 = wxNewId();
const long ProjectInfo::ID_TEXTCTRL3 = wxNewId();
const long ProjectInfo::ID_TEXTCTRL4 = wxNewId();
const long ProjectInfo::ID_STATICTEXT1 = wxNewId();
const long ProjectInfo::ID_TEXTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ProjectInfo,wxDialog)
	//(*EventTable(ProjectInfo)
	//*)
END_EVENT_TABLE()

ProjectInfo::ProjectInfo(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ProjectInfo)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStdDialogButtonSizer* StdDialogButtonSizer1;

	Create(parent, id, _("Project info"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Copyright"));
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Title:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer3->Add(StaticText2, 1, wxALL|wxEXPAND, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Author:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer3->Add(StaticText3, 1, wxALL|wxEXPAND, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Company:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	BoxSizer3->Add(StaticText4, 1, wxALL|wxEXPAND, 5);
	BoxSizer2->Add(BoxSizer3, 0, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	TextTitle = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	TextTitle->SetMaxLength(100);
	BoxSizer4->Add(TextTitle, 1, wxALL|wxEXPAND, 5);
	TextAuthor = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextAuthor->SetMaxLength(100);
	BoxSizer4->Add(TextAuthor, 1, wxALL|wxEXPAND, 5);
	TextCompany = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	TextCompany->SetMaxLength(100);
	BoxSizer4->Add(TextCompany, 1, wxALL|wxEXPAND, 5);
	BoxSizer2->Add(BoxSizer4, 1, wxALL|wxEXPAND, 5);
	StaticBoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Comment:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
	TextComment = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer1->Add(TextComment, 1, wxALL|wxEXPAND, 5);
	StdDialogButtonSizer1 = new wxStdDialogButtonSizer();
	StdDialogButtonSizer1->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
	StdDialogButtonSizer1->Realize();
	BoxSizer1->Add(StdDialogButtonSizer1, 1, wxALL|wxEXPAND, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

void ProjectInfo::Set(const char *title,const char *author, const char *company, const char *comment){
	TextTitle->SetValue(title);
	TextAuthor->SetValue(author);
	TextCompany->SetValue(company);
	TextComment->SetValue(comment);
}
void ProjectInfo::Get(char *title,char *author,char *company,char *comment){
	strcpy(title,TextTitle->GetValue().c_str());
	strcpy(author,TextAuthor->GetValue().c_str());
	strcpy(company,TextCompany->GetValue().c_str());
	strcpy(comment,TextComment->GetValue().c_str());
}
ProjectInfo::~ProjectInfo()
{
	//(*Destroy(ProjectInfo)
	//*)
}

