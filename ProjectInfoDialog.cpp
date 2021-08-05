#include "ProjectInfoDialog.h"

#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>
ProjectInfoDialog::ProjectInfoDialog(wxWindow* parent,const ProjectInfo &info):
		wxDialog(parent,wxID_ANY,_("Project info"),wxDefaultPosition,wxDefaultSize,wxRESIZE_BORDER|0x20000000|0x800|0x1000) {
	wxBoxSizer* all_box = new wxBoxSizer(wxVERTICAL);
    {
		wxStaticBoxSizer* copyright = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Copyright"));
		{
			wxBoxSizer* cr_box = new wxBoxSizer(wxHORIZONTAL);
			{
				wxBoxSizer* text_box = new wxBoxSizer(wxVERTICAL);
				text_box->Add(new wxStaticText(this, wxID_ANY, _("Title:")), 1, wxALL|wxEXPAND, 5);
				text_box->Add(new wxStaticText(this, wxID_ANY, _("Author:")), 1, wxALL|wxEXPAND, 5);
				text_box->Add(new wxStaticText(this, wxID_ANY, _("Company:")), 1, wxALL|wxEXPAND, 5);
				cr_box->Add(text_box, 0, wxALL|wxEXPAND, 5);
			}
			{
				wxBoxSizer* input_box = new wxBoxSizer(wxVERTICAL);
				{
					TextTitle = new wxTextCtrl(this, wxID_ANY, info.title);
					TextTitle->SetMaxLength(100);
					input_box->Add(TextTitle, 1, wxALL|wxEXPAND, 5);
				}
				{
					TextAuthor = new wxTextCtrl(this, wxID_ANY, info.author);
					TextAuthor->SetMaxLength(100);
					input_box->Add(TextAuthor, 1, wxALL|wxEXPAND, 5);
				}
				{
					TextCompany = new wxTextCtrl(this, wxID_ANY, info.company);
					TextCompany->SetMaxLength(100);
					input_box->Add(TextCompany, 1, wxALL|wxEXPAND, 5);
				}
				cr_box->Add(input_box, 3, wxALL|wxEXPAND, 5);
			}
			copyright->Add(cr_box, 1, wxALL|wxEXPAND, 5);
		}
		all_box->Add(copyright, 0, wxALL|wxEXPAND, 5);
    }
    {
		all_box->Add(new wxStaticText(this, wxID_ANY, _("Comment:")), 0, wxALL|wxEXPAND, 5);
		TextComment = new wxTextCtrl(this, wxID_ANY, info.comment,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE);
		all_box->Add(TextComment, 1, wxALL|wxEXPAND, 10);
    }
    {
		wxStdDialogButtonSizer* buttons = new wxStdDialogButtonSizer();
		buttons->AddButton(new wxButton(this, wxID_OK, wxEmptyString));
		buttons->Realize();
		all_box->Add(buttons, 0, wxALL|wxEXPAND, 5);
    }
    SetSizerAndFit(all_box);
}

void ProjectInfoDialog::Get(ProjectInfo &info) {
    strcpy(info.title,TextTitle->GetValue().c_str());
    strcpy(info.author,TextAuthor->GetValue().c_str());
    strcpy(info.company,TextCompany->GetValue().c_str());
    strcpy(info.comment,TextComment->GetValue().c_str());
}

