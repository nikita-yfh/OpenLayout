#include "OpenLayoutMain.h"
#include "MenuBar.h"
#include "ToolBar.h"
#include "NewBoardDialog.h"
#include "BottomPanel.h"
#include "LeftPanel.h"
#include "SettingsDialog.h"
#include <wx/sysopt.h>
#include <wx/msgdlg.h>

wxBEGIN_EVENT_TABLE(OpenLayoutFrame, wxFrame)
	EVT_MOVE(							OpenLayoutFrame::Move)
	EVT_SIZE(							OpenLayoutFrame::Resize)
	EVT_CLOSE(							OpenLayoutFrame::Close)
	EVT_MENU(wxID_SAVE,					OpenLayoutFrame::SaveFile)
	EVT_MENU(wxID_SAVEAS,				OpenLayoutFrame::SaveFileAs)
	EVT_MENU(wxID_OPEN,					OpenLayoutFrame::OpenFile)
	EVT_MENU(wxID_EXIT,					OpenLayoutFrame::Close)
	EVT_MENU(wxID_PROPERTIES,			OpenLayoutFrame::ShowSettings)
	EVT_MENU(wxID_ABOUT,				OpenLayoutFrame::ShowAbout)
	EVT_MENU(wxID_INFO,					OpenLayoutFrame::ShowProjectInfo)
	EVT_MENU(ID_BOARD_NEW,				OpenLayoutFrame::NewBoard)
	EVT_MENU(ID_BOARD_DELETE,			OpenLayoutFrame::DeleteBoard)
	EVT_MENU(ID_SCANNED_COPY,			OpenLayoutFrame::ShowImagesConfig)
	EVT_MENU(ID_GROUP,					OpenLayoutFrame::Group)
	EVT_MENU(ID_UNGROUP,				OpenLayoutFrame::Ungroup)
	EVT_MENU(wxID_SELECTALL,			OpenLayoutFrame::SelectAll)
	EVT_MENU(wxID_SAVE,					OpenLayoutFrame::SaveFile)
	EVT_MENU(wxID_SAVEAS,				OpenLayoutFrame::SaveFileAs)
	EVT_MENU(wxID_OPEN,					OpenLayoutFrame::OpenFile)
	EVT_MENU(ID_TRANSPARENT,			OpenLayoutFrame::ToggleTransparent)
	EVT_MENU(ID_ZOOM_BOARD,				OpenLayoutFrame::ZoomBoard)
	EVT_MENU(ID_ZOOM_OBJECTS,			OpenLayoutFrame::ZoomObjects)
	EVT_MENU(ID_ZOOM_SELECTION,			OpenLayoutFrame::ZoomSelection)
	EVT_MENU_RANGE(ID_LAYER_C1, ID_LAYER_O,
										OpenLayoutFrame::SetSelectionLayer)
	EVT_UPDATE_UI(wxID_COPY,			OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(wxID_CUT,				OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(wxID_DELETE,			OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(wxID_DUPLICATE,		OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(ID_ROTATE,			OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(ID_HMIRROR,			OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(ID_VMIRROR,			OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(ID_SNAP_GRID,			OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI(ID_BOARD_DELETE,		OpenLayoutFrame::UpdateUIDeleteBoard)
	EVT_UPDATE_UI(ID_GROUP,				OpenLayoutFrame::UpdateUIGroup)
	EVT_UPDATE_UI(ID_UNGROUP,			OpenLayoutFrame::UpdateUIUngroup)
	EVT_UPDATE_UI(ID_ZOOM_OBJECTS,		OpenLayoutFrame::UpdateUIObjects)
	EVT_UPDATE_UI(ID_ZOOM_SELECTION,	OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI_RANGE(ID_LAYER_C1, ID_LAYER_S2,
										OpenLayoutFrame::UpdateUISelection)
	EVT_UPDATE_UI_RANGE(ID_LAYER_I1, ID_LAYER_I2,
										OpenLayoutFrame::UpdateUIMultilayer)
	EVT_UPDATE_UI(ID_LAYER_O,			OpenLayoutFrame::UpdateUISelection)

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

OpenLayoutFrame::OpenLayoutFrame() {
	LoadSettings();
	Create(nullptr, wxID_ANY, "OpenLayout", settings.windowPos, settings.windowSize);
	MenuBar *menubar = new MenuBar();
	SetMenuBar(menubar);
	ToolBar *toolbar = new ToolBar(this);
	SetToolBar(toolbar);

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer *panels = new wxBoxSizer(wxHORIZONTAL);

		LeftPanel *right = new LeftPanel(this, pcb, settings);
		panels->Add(right, 0, wxEXPAND);

		canvas = new MainCanvas(this, pcb, settings);
		panels->Add(canvas, 1, wxEXPAND);

		selector = new SelectorPanel(this, pcb);
		selector->Show(settings.showSelectorPanel);
		panels->Add(selector, 0, wxEXPAND);

		components = new ComponentsPanel(this, pcb);
		components->Show(settings.showComponentsPanel);
		panels->Add(components, 0, wxEXPAND);

		macro = new MacroPanel(this);
		macro->Show(settings.showMacroPanel);
		panels->Add(macro, 0, wxEXPAND);

		content->Add(panels, 1, wxEXPAND);
	}

	BottomPanel *bottomPanel = new BottomPanel(this, pcb, settings);
	content->Add(bottomPanel, 0, wxEXPAND);

	SetFocus();

	SetSizer(content);
	SetAutoLayout(true);
	Layout();

	pcb.AddBoard(new Board(Board::Type::Empty, Vec2(160.0f, 100.0f), 0.0f, settings.originLeftTop));
}

void OpenLayoutFrame::OpenFile(wxCommandEvent &e) {
	wxFileDialog dialog(this, _("Open layout file"), "", "",
		_("Layout files (*.lay*)|*.lay*|All files (*.*)|*.*"), wxFD_OPEN);

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	File file(dialog.GetPath().c_str(), "rb");
	if(file.IsOk() && pcb.Load(file)) {
		lastFile = dialog.GetPath();
		Refresh();
	} else
		wxMessageBox(_("Error opening file"), _("Open"), wxICON_ERROR);
}

void OpenLayoutFrame::SaveFileAs(wxCommandEvent &e) {
	wxFileDialog dialog(this, _("Save layout file"), "", lastFile,
		_("Layout files (*.lay*)|*.lay*|All files (*.*)|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	File file(dialog.GetPath().c_str(), "wb");
	if(file.IsOk()) {
		pcb.Save(file);
		lastFile = dialog.GetPath();
	} else
		wxMessageBox(_("Error saving file"), _("Save"), wxICON_ERROR);
}

void OpenLayoutFrame::SaveFile(wxCommandEvent &e) {
	if(lastFile.IsEmpty())
		SaveFileAs(e);
	else {
		File file(lastFile, "wb");
		if(file.IsOk()) 
			pcb.Save(file);
		else
			wxMessageBox(_("Error saving file"), _("Save"), wxICON_ERROR);
	}
}

wxString OpenLayoutFrame::GetDir() {
#ifdef _WIN32
	wxString path = wxGetHomeDir() + "/AppData/Roaming/OpenLayout";
#else
	wxString path = wxGetHomeDir() + "/.local/share/OpenLayout";
#endif
	if(!wxDirExists(path))
		wxMkdir(path);
	return path;
}

void OpenLayoutFrame::SaveSettings() const {
	wxString path = GetDir() + "/settings";
	File file(path.c_str(), "wb");
	settings.Save(file);
}

void OpenLayoutFrame::LoadSettings() {
	wxString path = GetDir() + "/settings";
	File file(path.c_str(), "rb");
	if(!file.IsOk())
		return;
	settings.Load(file);
}

void OpenLayoutFrame::Move(wxMoveEvent &e) {
	settings.windowPos = e.GetPosition();
	e.Skip();
}
void OpenLayoutFrame::Resize(wxSizeEvent &e) {
	settings.windowSize = e.GetSize();
	e.Skip();
}

void OpenLayoutFrame::Close(wxCloseEvent&) {
	SaveSettings();
	Destroy();
}
void OpenLayoutFrame::Close(wxCommandEvent&) {
	SaveSettings();
	Destroy();
}
void OpenLayoutFrame::ShowSettings(wxCommandEvent&) {
	SettingsDialog dialog(this, settings);
	if(dialog.ShowModal() != wxID_OK)
		return;
	dialog.Get(settings);
	Refresh();
}
void OpenLayoutFrame::ShowAbout(wxCommandEvent&) {}
void OpenLayoutFrame::ShowProjectInfo(wxCommandEvent&) {
	pcb.info.ShowDialog(this);
}
void OpenLayoutFrame::NewBoard(wxCommandEvent&) {
	NewBoardDialog dialog(this);
	if(dialog.ShowModal() != wxID_OK)
		return;
	Board *board = dialog.CreateBoard(settings.originLeftTop);
	pcb.AddBoard(board);
	Refresh();
	wxMessageBox(wxString::Format(_("A new Board named \"%s\" was added."),
			board->GetName()), GetTitle(), wxICON_INFORMATION);
}
void OpenLayoutFrame::DeleteBoard(wxCommandEvent&) {
	wxMessageDialog dialog(this, wxString::Format(_("Delete board \"%s\"?"), 
		pcb.GetSelectedBoard()->GetName()), GetTitle(),
		wxOK | wxCANCEL | wxOK_DEFAULT | wxICON_QUESTION | wxCENTER);
	if(dialog.ShowModal() != wxID_OK)
		return;
	pcb.DeleteSelectedBoard();
	Refresh();
}
void OpenLayoutFrame::Group(wxCommandEvent&) {}
void OpenLayoutFrame::Ungroup(wxCommandEvent&) {}
void OpenLayoutFrame::SelectAll(wxCommandEvent&) {}
void OpenLayoutFrame::SetSelectionLayer(wxCommandEvent&) {}
void OpenLayoutFrame::Delete(wxCommandEvent&) {}
void OpenLayoutFrame::ShowImagesConfig(wxCommandEvent&) {
	pcb.GetSelectedBoard()->images.ShowDialog(this, ColorScheme());
}
void OpenLayoutFrame::ToggleTransparent(wxCommandEvent&) {
	settings.transparent = !settings.transparent;
	canvas->Refresh();
}
void OpenLayoutFrame::ZoomBoard(wxCommandEvent&) {
	pcb.GetSelectedBoard()->ZoomBoard(canvas->GetSize());
	canvas->Refresh();
}
void OpenLayoutFrame::ZoomObjects(wxCommandEvent&) {
	pcb.GetSelectedBoard()->ZoomObjects(canvas->GetSize());
	canvas->Refresh();
}
void OpenLayoutFrame::ZoomSelection(wxCommandEvent&) {
	pcb.GetSelectedBoard()->ZoomSelection(canvas->GetSize());
	canvas->Refresh();
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
	Layout();
}
void OpenLayoutFrame::ToggleDRCPanel(wxCommandEvent &e) {
	Layout();
}
void OpenLayoutFrame::ToggleMacrosPanel(wxCommandEvent &e) {
	macro->Show(e.IsChecked());
	Layout();
}

void OpenLayoutFrame::UpdateSelectorPanel(wxUpdateUIEvent &e) {
	settings.showSelectorPanel = selector->IsShown();
	e.Check(settings.showSelectorPanel);
}
void OpenLayoutFrame::UpdateComponentsPanel(wxUpdateUIEvent &e) {
	settings.showComponentsPanel = components->IsShown();
	e.Check(settings.showComponentsPanel);
}
void OpenLayoutFrame::UpdatePropertiesPanel(wxUpdateUIEvent &e) {
}
void OpenLayoutFrame::UpdateDRCPanel(wxUpdateUIEvent &e) {
}
void OpenLayoutFrame::UpdateMacrosPanel(wxUpdateUIEvent &e) {
	settings.showMacroPanel = macro->IsShown();
	e.Check(settings.showMacroPanel);
}

void OpenLayoutFrame::UpdateUIObjects(wxUpdateUIEvent &e) {
	e.Enable(!pcb.GetSelectedBoard()->IsEmpty());
}
void OpenLayoutFrame::UpdateUISelection(wxUpdateUIEvent &e) {
	e.Enable(pcb.GetSelectedBoard()->IsSelected());
}
void OpenLayoutFrame::UpdateUIGroup(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIUngroup(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIMultilayer(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIDeleteBoard(wxUpdateUIEvent &e) {
	e.Enable(!pcb.HasOneBoard());
}

