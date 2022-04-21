#include "ProjectInfo.h"
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/string.h>

ProjectInfo::ProjectInfo(){
	title[0] = '\0';
	author[0] = '\0';
	company[0] = '\0';
	comment[0] = '\0';
}
void ProjectInfo::Save(File &file) const{
	file.WriteString(title, 100);
	file.WriteString(author, 100);
	file.WriteString(company, 100);
	file.WriteString(comment);
}
void ProjectInfo::Load(File &file){
	file.ReadString(title, 100);
	file.ReadString(author, 100);
	file.ReadString(company, 100);
	file.ReadString(comment);
}

void ProjectInfo::ShowDialog(wxWindow *parent){
	wxDialog *dialog = new wxDialog(parent, wxID_ANY, _("Project info"),
		wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER|0x20000000|0x800|0x1000);
	wxTextCtrl* textAuthor;
	wxTextCtrl* textComment;
	wxTextCtrl* textCompany;
	wxTextCtrl* textTitle;
	wxBoxSizer* all_box = new wxBoxSizer(wxVERTICAL);
	{
		wxStaticBoxSizer* copyright = new wxStaticBoxSizer(wxHORIZONTAL, dialog, _("Copyright"));
		{
			wxBoxSizer* cr_box = new wxBoxSizer(wxHORIZONTAL);
			{
				wxBoxSizer* text_box = new wxBoxSizer(wxVERTICAL);
				text_box->Add(new wxStaticText(dialog, wxID_ANY, _("Title:")), 1, wxALL|wxEXPAND, 5);
				text_box->Add(new wxStaticText(dialog, wxID_ANY, _("Author:")), 1, wxALL|wxEXPAND, 5);
				text_box->Add(new wxStaticText(dialog, wxID_ANY, _("Company:")), 1, wxALL|wxEXPAND, 5);
				cr_box->Add(text_box, 0, wxALL|wxEXPAND, 5);
			}
			{
				wxBoxSizer* input_box = new wxBoxSizer(wxVERTICAL);
				{
					textTitle = new wxTextCtrl(dialog, wxID_ANY, title);
					textTitle->SetMaxLength(100);
					input_box->Add(textTitle, 1, wxALL|wxEXPAND, 5);
				}
				{
					textAuthor = new wxTextCtrl(dialog, wxID_ANY, author);
					textAuthor->SetMaxLength(100);
					input_box->Add(textAuthor, 1, wxALL|wxEXPAND, 5);
				}
				{
					textCompany = new wxTextCtrl(dialog, wxID_ANY, company);
					textCompany->SetMaxLength(100);
					input_box->Add(textCompany, 1, wxALL|wxEXPAND, 5);
				}
				cr_box->Add(input_box, 3, wxALL|wxEXPAND, 5);
			}
			copyright->Add(cr_box, 1, wxALL|wxEXPAND, 5);
		}
		all_box->Add(copyright, 0, wxALL|wxEXPAND, 5);
	}
	{
		all_box->Add(new wxStaticText(dialog, wxID_ANY, _("Comment:")), 0, wxALL|wxEXPAND, 5);
		textComment = new wxTextCtrl(dialog, wxID_ANY, comment,
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
		all_box->Add(textComment, 1, wxALL|wxEXPAND, 10);
	}
	{
		wxStdDialogButtonSizer* buttons = new wxStdDialogButtonSizer();
		buttons->AddButton(new wxButton(dialog, wxID_OK, wxEmptyString));
		buttons->Realize();
		all_box->Add(buttons, 0, wxALL|wxEXPAND, 5);
	}
	dialog->SetSizerAndFit(all_box);
	dialog->ShowModal();
	strncpy(title,		textTitle->GetValue().c_str(), 100);
	strncpy(author,		textAuthor->GetValue().c_str(), 100);
	strncpy(company,	textCompany->GetValue().c_str(), 100);
	strncpy(comment,	textComment->GetValue().c_str(), 4096);
}
