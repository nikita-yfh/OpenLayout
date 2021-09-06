#include "ScannedCopyDialog.h"
#include "OpenLayoutApp.h"
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
ScannedCopyDialog::ScannedCopyDialog(wxWindow* parent,const ImageConfig images[2],Settings &s)
	:wxDialog(parent, wxID_ANY, _("Scanned copy")) {
	wxBoxSizer *all_box=new wxBoxSizer(wxVERTICAL);
	{
		wxNotebook *notebook = new wxNotebook(this, wxID_ANY);
		for(int q=0; q<2; q++) {
			wxPanel *panel=new wxPanel(notebook);
			{
				wxBoxSizer *content=new wxBoxSizer(wxVERTICAL);
				{
					wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
					{
						wxBoxSizer *input=new wxBoxSizer(wxVERTICAL);
						input->Add(new wxStaticText(panel,wxID_ANY,_("Load bitmap:")),1,wxALL,5);
						content->Add(sizer,0,wxEXPAND);
						{
							path[q]= new wxFilePickerCtrl(panel, wxID_ANY,images[q].path,_("Load bitmap"),
														  _("*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.bmp|*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.bmp|*|*"),
														  wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL|wxFLP_USE_TEXTCTRL);
							path[q]->SetPath(images[q].path);
							input->Add(path[q],0,wxEXPAND|wxALL,5);
						}
						sizer->Add(input,1,wxEXPAND);
					}
					{
						wxPanel *tp=new wxPanel(panel);
						wxStaticText *text=new wxStaticText(tp,wxID_ANY,char(q+'1'));
						wxFont myFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
						text->SetFont(myFont);
						tp->SetBackgroundColour(SETTINGS.get_color(COLOR_BGR));
						text->SetForegroundColour(SETTINGS.get_color(q?COLOR_C2:COLOR_C1));
						sizer->Add(tp,0,wxEXPAND|wxALL,5);
					}
				}
				{
					show[q]=new wxCheckBox(panel,wxID_ANY,_("Show bitmap"));
					show[q]->SetValue(images[q].show);
					content->Add(show[q],0,wxEXPAND|wxALL,5);
				}
				wxSize wsize(75,-1);
				{
					wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
					sizer->Add(new wxStaticText(panel,wxID_ANY,_("Resolution")),1,wxALL,5);
					dpi[q]=new wxSpinCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wsize,0,20,2400,images[q].dpi);
					sizer->Add(dpi[q],1,wxEXPAND|wxALL,5);
					sizer->Add(new wxStaticText(panel,wxID_ANY,_("[dpi]")),1,wxALL,5);
					content->Add(sizer,0,wxEXPAND);
				}
				{
					wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
					sizer->Add(new wxStaticText(panel,wxID_ANY,_("X-Offset")),1,wxALL,5);
					dx[q]=new wxSpinCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wsize,0,-3000,3000,images[q].pos.x);
					sizer->Add(dx[q],1,wxEXPAND|wxALL,5);
					sizer->Add(new wxStaticText(panel,wxID_ANY,_("[1/10 mm]")),1,wxALL,5);
					content->Add(sizer,0,wxEXPAND);
				}
				{
					wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
					sizer->Add(new wxStaticText(panel,wxID_ANY,_("Y-Offset")),1,wxALL,5);
					dy[q]=new wxSpinCtrl(panel,wxID_ANY,wxEmptyString,wxDefaultPosition,wsize,0,-3000,3000,images[q].pos.y);
					sizer->Add(dy[q],1,wxEXPAND|wxALL,5);
					sizer->Add(new wxStaticText(panel,wxID_ANY,_("[1/10 mm]")),1,wxALL,5);
					content->Add(sizer,0,wxEXPAND);
				}
				panel->SetSizerAndFit(content);
			}
			char name[128];
			sprintf(name,_("Board side %d"),q+1);
			notebook->AddPage(panel,name);
		}
		notebook->SetSelection(1);
		all_box->Add(notebook, 1, wxALL|wxEXPAND, 5);
	}
	{
		wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
		buttons->AddButton(new wxButton(this, wxID_OK));
		buttons->AddButton(new wxButton(this, wxID_CANCEL));
		buttons->Realize();
		all_box->Add(buttons, 0, wxALL|wxEXPAND, 5);
	}
	SetSizerAndFit(all_box);
}

void ScannedCopyDialog::Get(ImageConfig images[2]) {
	for(int q=0; q<2; q++) {
		images[q].pos.x=dx[q]->GetValue();
		images[q].pos.y=dy[q]->GetValue();
		images[q].dpi=dpi[q]->GetValue();
		images[q].show=show[q]->GetValue();
		images[q].path=path[q]->GetPath();
	}
}
