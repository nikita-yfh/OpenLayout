#include "OpenLayoutMain.h"
#include "MenuBar.h"
#include "ToolBar.h"
#include "NewBoardDialog.h"
#include "BottomPanel.h"

wxBEGIN_EVENT_TABLE(OpenLayoutFrame, wxFrame)
	EVT_MENU(wxID_EXIT,				OpenLayoutFrame::Close)
	EVT_MENU(wxID_PROPERTIES,		OpenLayoutFrame::ShowSettings)
	EVT_MENU(wxID_ABOUT,			OpenLayoutFrame::ShowAbout)
	EVT_MENU(wxID_INFO,				OpenLayoutFrame::ShowProjectInfo)
	EVT_MENU(ID_BOARD_NEW,			OpenLayoutFrame::NewBoard)
	EVT_MENU(ID_SCANNED_COPY,		OpenLayoutFrame::ShowImagesConfig)
	EVT_MENU(ID_GROUP,				OpenLayoutFrame::Group)
	EVT_MENU(ID_UNGROUP,			OpenLayoutFrame::Ungroup)
	EVT_MENU(wxID_SELECTALL,		OpenLayoutFrame::SelectAll)
	EVT_MENU(wxID_SAVE,				OpenLayoutFrame::SaveFile)
	EVT_MENU(wxID_SAVEAS,			OpenLayoutFrame::SaveFileAs)
	EVT_MENU(wxID_OPEN,				OpenLayoutFrame::OpenFile)
	EVT_MENU_RANGE(ID_LAYER_C1, ID_LAYER_O, OpenLayoutFrame::SetSelectionLayer)
	EVT_UPDATE_UI(wxID_COPY,		OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(wxID_CUT,			OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(wxID_DELETE,		OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(wxID_DUPLICATE,	OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_ROTATE,		OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_HMIRROR,		OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_VMIRROR,		OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_SNAP_GRID,		OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_GROUP,			OpenLayoutFrame::UpdateUIGroup)
	EVT_UPDATE_UI(ID_UNGROUP,		OpenLayoutFrame::UpdateUIUngroup)
	EVT_UPDATE_UI_RANGE(ID_LAYER_C1, ID_LAYER_S2, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI_RANGE(ID_LAYER_I1, ID_LAYER_I2, OpenLayoutFrame::UpdateUIMultilayer)
	EVT_UPDATE_UI(ID_LAYER_O, OpenLayoutFrame::UpdateUIEdit)

	EVT_MENU(ID_PANEL_SELECTOR,			OpenLayoutFrame::ToggleSelectorPanel)
	EVT_MENU(ID_PANEL_COMPONENTS,		OpenLayoutFrame::ToggleComponentsPanel)
	EVT_MENU(ID_PANEL_PROPERTIES,		OpenLayoutFrame::TogglePropertiesPanel)
	EVT_MENU(ID_PANEL_DRC,				OpenLayoutFrame::ToggleDRCPanel)
	EVT_MENU(ID_PANEL_MACRO,			OpenLayoutFrame::ToggleMacrosPanel)

	EVT_UPDATE_UI(ID_PANEL_SELECTOR,	OpenLayoutFrame::UpdateSelectorPanel)
	EVT_UPDATE_UI(ID_PANEL_COMPONENTS,	OpenLayoutFrame::UpdateComponentsPanel)
	EVT_UPDATE_UI(ID_PANEL_PROPERTIES,	OpenLayoutFrame::UpdatePropertiesPanel)
	EVT_UPDATE_UI(ID_PANEL_DRC,			OpenLayoutFrame::UpdateDRCPanel)
	EVT_UPDATE_UI(ID_PANEL_MACRO,		OpenLayoutFrame::UpdateMacrosPanel)
wxEND_EVENT_TABLE()

OpenLayoutFrame::OpenLayoutFrame()
		:wxFrame(0, wxID_ANY, "OpenLayout", wxDefaultPosition, wxSize(800, 600)) {
	MenuBar *menubar = new MenuBar();
	SetMenuBar(menubar);
	ToolBar *toolbar = new ToolBar(this);
	SetToolBar(toolbar);

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);

	{
		wxBoxSizer *panels = new wxBoxSizer(wxHORIZONTAL);

		selector = new SelectorPanel(this, pcb);
		panels->Add(selector, 0, wxEXPAND);

		components = new ComponentPanel(this, pcb);
		panels->Add(components, 0, wxEXPAND);

		macros = new MacroPanel(this);
		panels->Add(macros, 0, wxEXPAND);

		content->Add(panels, wxEXPAND);
	}

	BottomPanel *bottomPanel = new BottomPanel(this, pcb);
	content->Add(bottomPanel, 0, wxEXPAND);

	SetSizer(content);
	SetAutoLayout(true);
}

void OpenLayoutFrame::Close(wxCommandEvent&) {}
void OpenLayoutFrame::ShowSettings(wxCommandEvent&) {}
void OpenLayoutFrame::ShowAbout(wxCommandEvent&) {}
void OpenLayoutFrame::ShowProjectInfo(wxCommandEvent&) {
	pcb.info.ShowDialog(this);
}
void OpenLayoutFrame::NewBoard(wxCommandEvent&) {
	NewBoardDialog dialog(this);
	dialog.ShowModal();
}
void OpenLayoutFrame::Group(wxCommandEvent&) {}
void OpenLayoutFrame::Ungroup(wxCommandEvent&) {}
void OpenLayoutFrame::SelectAll(wxCommandEvent&) {}
void OpenLayoutFrame::SaveFile(wxCommandEvent&) {}
void OpenLayoutFrame::SaveFileAs(wxCommandEvent&) {}
void OpenLayoutFrame::OpenFile(wxCommandEvent&) {}
void OpenLayoutFrame::SetSelectionLayer(wxCommandEvent&) {}
void OpenLayoutFrame::Delete(wxCommandEvent&) {}
void OpenLayoutFrame::ShowImagesConfig(wxCommandEvent&) {
	pcb.GetSelectedBoard()->images.ShowDialog(this, ColorScheme());
}

void OpenLayoutFrame::ToggleSelectorPanel(wxCommandEvent &e) {
	selector->Show(e.IsChecked());
	Layout();
}
void OpenLayoutFrame::ToggleComponentsPanel(wxCommandEvent &e) {
	components->Show(e.IsChecked());
	Layout();
}
void OpenLayoutFrame::TogglePropertiesPanel(wxCommandEvent &e) {
}
void OpenLayoutFrame::ToggleDRCPanel(wxCommandEvent &e) {
}
void OpenLayoutFrame::ToggleMacrosPanel(wxCommandEvent &e) {
	macros->Show(e.IsChecked());
	Layout();
}

void OpenLayoutFrame::UpdateSelectorPanel(wxUpdateUIEvent &e) {
	e.Check(selector->IsShown());
}
void OpenLayoutFrame::UpdateComponentsPanel(wxUpdateUIEvent &e) {
	e.Check(components->IsShown());
}
void OpenLayoutFrame::UpdatePropertiesPanel(wxUpdateUIEvent &e) {
}
void OpenLayoutFrame::UpdateDRCPanel(wxUpdateUIEvent &e) {
}
void OpenLayoutFrame::UpdateMacrosPanel(wxUpdateUIEvent &e) {
	e.Check(macros->IsShown());
}

void OpenLayoutFrame::UpdateUIEdit(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIGroup(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIUngroup(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIMultilayer(wxUpdateUIEvent&) {}
