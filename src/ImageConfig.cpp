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
	*path = '\0';
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

void ImageConfigs::ShowDialog(wxWindow *parent, const ColorScheme &colors){
	wxDialog *dialog = new wxDialog(parent, wxID_ANY, _("Scanned copy"));
	wxSpinCtrl *dpi[2];
	wxSpinCtrl *dx[2];
	wxSpinCtrl *dy[2];
	wxCheckBox *enabled[2];
	wxFilePickerCtrl *path[2];
	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	{
		wxNotebook *notebook = new wxNotebook(dialog, wxID_ANY);
		for(int i = 0; i<2; i++) {
			wxPanel *panel = new wxPanel(notebook);
			{
				wxBoxSizer *page = new wxBoxSizer(wxVERTICAL);
				{
					wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
					{
						wxBoxSizer *input = new wxBoxSizer(wxVERTICAL);
						input->Add(new wxStaticText(panel, wxID_ANY, _("Load bitmap:")), 1, wxALL, 5);
						page->Add(sizer, 0, wxEXPAND);
						{
							path[i] = new wxFilePickerCtrl(panel, wxID_ANY, images[i].path, _("Load bitmap"),
								 _("*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.bmp|*.png;*.bmp;*.jpg;*.jpeg;*.tga;*.bmp|*|*"),
								 wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_SMALL|wxFLP_USE_TEXTCTRL);
							path[i]->SetPath(images[i].path);
							input->Add(path[i], 0, wxEXPAND|wxALL, 5);
						}
						sizer->Add(input, 1, wxEXPAND);
					}
					{
						wxPanel *tp = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxSize(50, 50));
						wxStaticText *text = new wxStaticText(tp, wxID_ANY, wxString::Format(" %d ", i + 1));
						wxFont myFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
						text->SetFont(myFont);
						tp->SetBackgroundColour(colors[COLOR_BGR]);
						text->SetForegroundColour(colors[i ? COLOR_C2 : COLOR_C1]);
						sizer->Add(tp, 0, wxALL, 5);
					}
				}
				{
					enabled[i] = new wxCheckBox(panel, wxID_ANY, _("Show bitmap"));
					enabled[i]->SetValue(images[i].enabled);
					page->Add(enabled[i], 0, wxEXPAND|wxALL, 5);
				}
				wxSize size(75, -1);
				wxGridSizer *sizer = new wxGridSizer(3, 3, 3, 10);
				{
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("Resolution")), 1, wxALIGN_CENTER_VERTICAL);
					dpi[i] = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, size, 0, 20, 2400, images[i].dpi);
					sizer->Add(dpi[i], 1, wxEXPAND|wxALL);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("[dpi]")), 1, wxALIGN_CENTER_VERTICAL);
				}
				{
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("X-Offset")), 1, wxALIGN_CENTER_VERTICAL);
					dx[i] = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, size, 0, -3000, 3000, images[i].shift.x);
					sizer->Add(dx[i], 1, wxEXPAND|wxALL);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("[1/10 mm]")), 1, wxALIGN_CENTER_VERTICAL);
				}
				{
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("Y-Offset")), 1, wxALIGN_CENTER_VERTICAL);
					dy[i] = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, size, 0, -3000, 3000, images[i].shift.y);
					sizer->Add(dy[i], 1, wxEXPAND|wxALL);
					sizer->Add(new wxStaticText(panel, wxID_ANY, _("[1/10 mm]")), 1, wxALIGN_CENTER_VERTICAL);
				}
				page->Add(sizer);
				panel->SetSizerAndFit(page);
			}
			notebook->AddPage(panel, wxString::Format(_("Board side %d"), i+1));
		}
		notebook->SetSelection(1);
		content->Add(notebook, 1, wxALL|wxEXPAND, 5);
	}
	{
		wxStdDialogButtonSizer *buttons = new wxStdDialogButtonSizer();
		buttons->AddButton(new wxButton(dialog, wxID_OK));
		buttons->AddButton(new wxButton(dialog, wxID_CANCEL));
		buttons->Realize();
		content->Add(buttons, 0, wxALL|wxEXPAND, 5);
	}
	dialog->SetSizerAndFit(content);
	dialog->Show();
	for(int i = 0; i < 2; i++) {
		images[i].shift.x = dx[i]->GetValue();
		images[i].shift.y = dy[i]->GetValue();
		images[i].dpi = dpi[i]->GetValue();
		images[i].enabled = enabled[i]->GetValue();
		strncpy(images[i].path, path[i]->GetPath().c_str(), 200);
	}
}

