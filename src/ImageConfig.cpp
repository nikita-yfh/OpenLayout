#include "ImageConfig.h"
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>

ImageConfig::ImageConfig(){
	enabled = false;
	path[0] = '\0';
	dpi = 600;
	shift.SetZero();
}
void ImageConfigs::Save(File &file) const{
	file.Write<uint8_t>(images[0].enabled);
	file.Write<uint8_t>(images[1].enabled);
	file.WriteString(images[0].path, 200);
	file.WriteString(images[1].path, 200);
	file.Write<uint32_t>(images[0].dpi);
	file.Write<uint32_t>(images[1].dpi);
	images[0].shift.Save<float>(file);
	images[1].shift.Save<float>(file);
}
void ImageConfigs::Load(File &file){
	images[0].enabled = file.Read<uint8_t>();
	images[1].enabled = file.Read<uint8_t>();
	file.ReadString(images[0].path, 200);
	file.ReadString(images[1].path, 200);
	images[0].dpi = file.Read<uint32_t>();
	images[1].dpi = file.Read<uint32_t>();
	images[0].shift.Load<float>(file);
	images[1].shift.Load<float>(file);
}

void ImageConfigs::ShowDialog(wxWindow *parent){
	wxDialog *dialog = new wxDialog(parent, wxID_ANY, _("Scanned copy"));
	wxSpinCtrl *dpi[2];
	wxSpinCtrl *dx[2];
	wxSpinCtrl *dy[2];
	wxCheckBox *enabled[2];
	wxFilePickerCtrl *path[2];
	wxBoxSizer *all_box = new wxBoxSizer(wxVERTICAL);
	{
		wxNotebook *notebook = new wxNotebook(dialog, wxID_ANY);
		for(int q = 0; q<2; q++) {
			wxPanel *panel = new wxPanel(notebook);
			{
				wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
				{
					wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
					{
						wxBoxSizer *input = new wxBoxSizer(wxVERTICAL);
						input->Add(new wxStaticText(panel, wxID_ANY, _("Load bitmap:")), 1, wxALL, 5);
						content->Add(sizer, 0, wxEXPAND);
						{
							path[q]= new wxFilePickerCtrl(panel, wxID_ANY, images[q].path, _("Load bitmap"),
						 _("*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.bmp|*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.bmp|*|*"),
						 wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL|wxFLP_USE_TEXTCTRL);
							path[q]->SetPath(images[q].path);
							input->Add(path[q], 0, wxEXPAND|wxALL, 5);
						}
						sizer->Add(input, 1, wxEXPAND);
					}
					{
						wxPanel *tp = new wxPanel(panel);
						wxStaticText *text = new wxStaticText(tp, wxID_ANY, char(q+'1'));
						wxFont myFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
						text->SetFont(myFont);
						//tp->SetBackgroundColour(SETTINGS.get_color(COLOR_BGR));
						//text->SetForegroundColour(SETTINGS.get_color(q?COLOR_C2:COLOR_C1));
						sizer->Add(tp, 0, wxEXPAND|wxALL, 5);
					}
				}
				{
					enabled[q] = new wxCheckBox(panel, wxID_ANY, _("Show bitmap"));
					enabled[q]->SetValue(images[q].enabled);
					content->Add(enabled[q], 0, wxEXPAND|wxALL, 5);
				}
				wxSize wsize(75, -1);
				{
					wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("Resolution")), 1, wxALL, 5);
					dpi[q] = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wsize, 0, 20, 2400, images[q].dpi);
					sizer->Add(dpi[q], 1, wxEXPAND|wxALL, 5);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("[dpi]")), 1, wxALL, 5);
					content->Add(sizer, 0, wxEXPAND);
				}
				{
					wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("X-Offset")), 1, wxALL, 5);
					dx[q] = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wsize, 0, -3000, 3000, images[q].shift.x);
					sizer->Add(dx[q], 1, wxEXPAND|wxALL, 5);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("[1/10 mm]")), 1, wxALL, 5);
					content->Add(sizer, 0, wxEXPAND);
				}
				{
					wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("Y-Offset")), 1, wxALL, 5);
					dy[q] = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wsize, 0, -3000, 3000, images[q].shift.y);
					sizer->Add(dy[q], 1, wxEXPAND|wxALL, 5);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("[1/10 mm]")), 1, wxALL, 5);
					content->Add(sizer, 0, wxEXPAND);
				}
				panel->SetSizerAndFit(content);
			}
			char name[128];
			sprintf(name, _("Board side %d"), q+1);
			notebook->AddPage(panel, name);
		}
		notebook->SetSelection(1);
		all_box->Add(notebook, 1, wxALL|wxEXPAND, 5);
	}
	{
		wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
		buttons->AddButton(new wxButton(dialog, wxID_OK));
		buttons->AddButton(new wxButton(dialog, wxID_CANCEL));
		buttons->Realize();
		all_box->Add(buttons, 0, wxALL|wxEXPAND, 5);
	}
	dialog->SetSizerAndFit(all_box);
	dialog->ShowModal();
	for(int q = 0; q < 2; q++) {
		images[q].shift.x = dx[q]->GetValue();
		images[q].shift.y = dy[q]->GetValue();
		images[q].dpi = dpi[q]->GetValue();
		images[q].enabled = enabled[q]->GetValue();
		strncpy(images[q].path, path[q]->GetPath().c_str(), 200);
	}
}

