#include "OpenLayoutMain.h"
#include "NewBoardDialog.h"
#include "BottomPanel.h"
#include "LeftPanel.h"
#include "SettingsDialog.h"
#include "GLUtils.h"
#include <wx/sysopt.h>
#include <wx/msgdlg.h>

extern "C" {
#include "toolbar/align_bottom.xpm"
#include "toolbar/align_hcenter.xpm"
#include "toolbar/align_left.xpm"
#include "toolbar/align_right.xpm"
#include "toolbar/align_top.xpm"
#include "toolbar/align_vcenter.xpm"
#include "toolbar/align.xpm"
#include "toolbar/bitmap.xpm"
#include "toolbar/components.xpm"
#include "toolbar/copy.xpm"
#include "toolbar/cut.xpm"
#include "toolbar/delete.xpm"
#include "toolbar/drc.xpm"
#include "toolbar/duplicate.xpm"
#include "toolbar/group_off.xpm"
#include "toolbar/group_on.xpm"
#include "toolbar/info.xpm"
#include "toolbar/macro.xpm"
#include "toolbar/mirror_h.xpm"
#include "toolbar/mirror_v.xpm"
#include "toolbar/new.xpm"
#include "toolbar/open.xpm"
#include "toolbar/paste.xpm"
#include "toolbar/print.xpm"
#include "toolbar/properties.xpm"
#include "toolbar/redo.xpm"
#include "toolbar/rotate.xpm"
#include "toolbar/save.xpm"
#include "toolbar/selector.xpm"
#include "toolbar/to_grid.xpm"
#include "toolbar/transparent.xpm"
#include "toolbar/undo.xpm"
#include "toolbar/zoom_any.xpm"
#include "toolbar/zoom_board.xpm"
#include "toolbar/zoom_objects.xpm"
#include "toolbar/zoom_selection.xpm"
}

enum {
	ID_SAVE_MACRO = 1,
	ID_AUTOSAVE,
	ID_GERBER_IMPORT,
	ID_GERBER_EXPORT,
	ID_DRILL_DATA,
	ID_ISOLATION,
	ID_SAVE_BMP,
	ID_SAVE_JPG,
	ID_SAVE_GIF,
	ID_SAVE_EMP,
	ID_SAVE_PNG,
	ID_DIRECTORIES,
	ID_BOARD_PROPERTIES,
	ID_BOARD_COPY,
	ID_BOARD_DELETE,
	ID_BOARD_SET_RIGHT,
	ID_BOARD_SET_LEFT,
	ID_BOARD_MOVE_RIGHT,
	ID_BOARD_MOVE_LEFT,
	ID_BOARD_SAVE,
	ID_BOARD_IMPORT,
	ID_ROTATE,
	ID_ROTATE_90,
	ID_ROTATE_45,
	ID_ROTATE_10,
	ID_ROTATE_5,
	ID_ROTATE_CUSTOM,
	ID_VMIRROR,
	ID_HMIRROR,
	ID_GROUP,
	ID_UNGROUP,
	ID_CHANGE_SIDE,
	ID_CHANGE_LAYER,
	ID_LAYER_C1,
	ID_LAYER_S1,
	ID_LAYER_C2,
	ID_LAYER_S2,
	ID_LAYER_I1,
	ID_LAYER_I2,
	ID_LAYER_O,
	ID_SNAP_GRID,
	ID_MASSIVE,
	ID_LIST_DRILLINGS,
	ID_SCANNED_COPY,
	ID_FOOTPRINT,
	ID_RESET_MASK,
	ID_REMOVE_CONNECTIONS,
	ID_DELETE_OUTSIDE,
	ID_ELEMENT_IMPORT,
	ID_ELEMENT_EXPORT,
	ID_DEFINE_PLUGIN,
	ID_RUN_PLUGIN,
	ID_PANEL_SELECTOR,
	ID_PANEL_COMPONENTS,
	ID_PANEL_PROPERTIES,
	ID_PANEL_DRC,
	ID_PANEL_MACRO,
	ID_ALIGN,
	ID_ALIGN_TOP,
	ID_ALIGN_BOTTOM,
	ID_ALIGN_LEFT,
	ID_ALIGN_RIGHT,
	ID_ALIGN_HCENTER,
	ID_ALIGN_VCENTER,
	ID_TRANSPARENT,
	ID_ZOOM,
	ID_ZOOM_BOARD,
	ID_ZOOM_OBJECTS,
	ID_ZOOM_SELECTION,
	ID_PAGES
};



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
	EVT_MENU(wxID_NEW,					OpenLayoutFrame::NewBoard)
	EVT_MENU(ID_BOARD_COPY,				OpenLayoutFrame::CopyBoard)
	EVT_MENU(ID_BOARD_DELETE,			OpenLayoutFrame::DeleteBoard)
	EVT_MENU(ID_BOARD_MOVE_LEFT,		OpenLayoutFrame::MoveBoardLeft)
	EVT_MENU(ID_BOARD_MOVE_RIGHT,		OpenLayoutFrame::MoveBoardRight)
	EVT_MENU(ID_BOARD_SET_LEFT,			OpenLayoutFrame::SetBoardLeft)
	EVT_MENU(ID_BOARD_SET_RIGHT,		OpenLayoutFrame::SetBoardRight)
	EVT_MENU(ID_SCANNED_COPY,			OpenLayoutFrame::ShowImagesConfig)
	EVT_MENU(ID_ROTATE,					OpenLayoutFrame::Rotate)
	EVT_MENU(ID_HMIRROR,				OpenLayoutFrame::MirrorHorizontal)
	EVT_MENU(ID_VMIRROR,				OpenLayoutFrame::MirrorVertical)
	EVT_MENU(ID_GROUP,					OpenLayoutFrame::Group)
	EVT_MENU(ID_UNGROUP,				OpenLayoutFrame::Ungroup)
	EVT_MENU(ID_ALIGN,					OpenLayoutFrame::ShowAlignMenu)
	EVT_MENU(ID_ZOOM,					OpenLayoutFrame::ShowZoomMenu)
	EVT_MENU(wxID_DELETE,				OpenLayoutFrame::DeleteSelected)
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
	EVT_UPDATE_UI(ID_BOARD_MOVE_LEFT,	OpenLayoutFrame::UpdateUIMoveBoardLeft)
	EVT_UPDATE_UI(ID_BOARD_MOVE_RIGHT,	OpenLayoutFrame::UpdateUIMoveBoardRight)
	EVT_UPDATE_UI(ID_BOARD_SET_LEFT,	OpenLayoutFrame::UpdateUIMoveBoardLeft)
	EVT_UPDATE_UI(ID_BOARD_SET_RIGHT,	OpenLayoutFrame::UpdateUIMoveBoardRight)
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

	EVT_MENU_RANGE(ID_ROTATE_90, ID_ROTATE_CUSTOM,
										OpenLayoutFrame::SetRotationAngle)
	EVT_NOTEBOOK_PAGE_CHANGED(ID_PAGES,	OpenLayoutFrame::SelectPage)
wxEND_EVENT_TABLE()

static void AddMenuItem(wxMenu *parent, const wxString &text, int id, const wxBitmap &bitmap){
	wxMenuItem *item = new wxMenuItem(parent, id, text);
	item->SetBitmap(bitmap);
	parent->Append(item);
}

OpenLayoutFrame::OpenLayoutFrame() {
	LoadSettings();
	Create(nullptr, wxID_ANY, "OpenLayout", settings.windowPos, settings.windowSize);
	SetMenuBar(BuildMenuBar());
	SetToolBar(BuildToolBar());

	wxBoxSizer *content = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer *panels = new wxBoxSizer(wxHORIZONTAL);

		LeftPanel *right = new LeftPanel(this, pcb, settings);
		panels->Add(right, 0, wxEXPAND);

		pages = new wxNotebook(this, ID_PAGES, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
		panels->Add(pages, 1, wxEXPAND);

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

	pcb.AddBoard(new Board(_("New board"), Board::Type::Empty, Vec2(160.0f, 100.0f), 0.0f, settings.originLeftTop));
	UpdatePages();
}
wxMenuBar *OpenLayoutFrame::BuildMenuBar() {
	wxMenuBar *menuBar = new wxMenuBar();
	{
		//File
		wxMenu* menu = new wxMenu();
		menu->Append(wxID_NEW, _("New\tCtrl+N"));
		menu->Append(wxID_OPEN, _("Open\tCtrl+O"));
		menu->Append(wxID_SAVE, _("Save\tCtrl+S"));
		menu->Append(wxID_SAVEAS, _("Save as\tCtrl+Shift+S"));
		menu->AppendSeparator();
		menu->Append(ID_SAVE_MACRO, _("Save as macro"));
		menu->AppendSeparator();
		menu->Append(ID_AUTOSAVE, _("AutoSave"));
		menu->AppendSeparator();
		menu->Append(ID_GERBER_IMPORT, _("Gerber-Import"));
		menu->AppendSeparator();
		{
			//File->Export
			wxMenu *submenu = new wxMenu();
			submenu->Append(ID_GERBER_EXPORT, _("Gerber Export"));
			submenu->Append(ID_DRILL_DATA, _("Drill data (Excellon)"));
			submenu->AppendSeparator();
			submenu->Append(ID_ISOLATION, _("Isolation milling (HPGL, *.plt)"));
			submenu->AppendSeparator();
			submenu->Append(ID_SAVE_BMP, _("Bitmap (*.bmp)"));
			submenu->Append(ID_SAVE_JPG, _("JPG (*.jpg)"));
			submenu->Append(ID_SAVE_GIF, _("GIF (*.gif)"));
			submenu->Append(ID_SAVE_EMP, _("EMP (*.emp)"));
			submenu->Append(ID_SAVE_PNG, _("PNG (*.png)"));
			menu->Append(wxID_ANY, _("Export"), submenu);
		}
		menu->AppendSeparator();
		menu->Append(ID_DIRECTORIES, _("Directories"));
		menu->AppendSeparator();
		menu->Append(wxID_PRINT_SETUP, _("Printer setup"));
		menu->Append(wxID_PRINT, _("Print\tCtrl+P"));
		menu->AppendSeparator();
		menu->Append(wxID_EXIT, _("Exit\tCtrl+Q"));
		menuBar->Append(menu, _("&File"));
	}
	{
		//Edit
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"));
		menu->Append(wxID_REDO, _("&Redo\tCtrl+Y"));
		menu->AppendSeparator();
		menu->Append(wxID_COPY, _("C&opy\tCtrl+C"));
		menu->Append(wxID_CUT, _("&Cut\tCtrl+X"));
		menu->Append(wxID_PASTE, _("&Paste\tCtrl+V"));
		menu->Append(wxID_DUPLICATE, _("Dup&licate\tCtrl+D"));
		menu->Append(wxID_DELETE, _("&Delete\tDelete"));
		menu->AppendSeparator();
		menu->Append(wxID_SELECTALL, _("Select &all\tCtrl+A"));
		menuBar->Append(menu, _("&Edit"));
	}
	{
		//Board
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_NEW, _("&Add new board"));
		menu->Append(ID_BOARD_PROPERTIES, _("&Properties"));
		menu->Append(ID_BOARD_COPY, _("&Copy board"));
		menu->Append(ID_BOARD_DELETE, _("&Delete board"));
		menu->AppendSeparator();
		menu->Append(ID_BOARD_SET_RIGHT, _("Set board to &right"));
		menu->Append(ID_BOARD_SET_LEFT, _("Set board to &left"));
		menu->AppendSeparator();
		menu->Append(ID_BOARD_MOVE_RIGHT, _("&Move board to right"));
		menu->Append(ID_BOARD_MOVE_LEFT, _("M&ove board to left"));
		menu->Append(ID_BOARD_IMPORT, _("&Import pages from file"));
		menu->Append(ID_BOARD_SAVE, _("&Save pages to file"));
		menuBar->Append(menu, _("&Board"));
	}
	{
		//Functions
		wxMenu *menu = new wxMenu();
		menu->Append(ID_ROTATE, _("&Rotate\tCtrl+R"));
		menu->AppendSeparator();
		menu->Append(ID_HMIRROR, _("Mirror &horisontal\tCtrl+H"));
		menu->Append(ID_VMIRROR, _("Mirror &vertical\tCtrl+T"));
		menu->AppendSeparator();
		menu->Append(ID_GROUP, _("Build &group\tCtrl+G"));
		menu->Append(ID_UNGROUP, _("Split gro&up\tCtrl+U"));
		menu->AppendSeparator();
		menu->Append(ID_CHANGE_SIDE, _("&Change board side\tCtrl+W"));
		{
			//Functions->Set to layer
			wxMenu *submenu = new wxMenu();
			submenu->Append(ID_LAYER_C1, _("&C1"));
			submenu->Append(ID_LAYER_S1, _("S&1"));
			submenu->Append(ID_LAYER_C2, _("C&2"));
			submenu->Append(ID_LAYER_S2, _("&S2"));
			submenu->Append(ID_LAYER_I1, _("I1"));
			submenu->Append(ID_LAYER_I2, _("&I2"));
			submenu->Append(ID_LAYER_O, _("&O"));
			menu->Append(ID_CHANGE_LAYER, _("&Set to layer"), submenu);
		}
		menu->AppendSeparator();
		menu->Append(ID_SNAP_GRID, _("S&nap to grid"));
		menu->Append(ID_MASSIVE, _("&Tile / Arrange circular"));
		menuBar->Append(menu, _("F&unctions"));
	}
	{
		//Extras
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_INFO, _("&Project info"));
		menu->Append(ID_LIST_DRILLINGS, _("&List drillings"));
		menu->AppendSeparator();
		menu->Append(ID_SCANNED_COPY, _("&Scanned copy"));
		menu->AppendSeparator();
		menu->Append(ID_FOOTPRINT, _("&Footprint-Wizard"));
		menu->AppendSeparator();
		menu->Append(ID_RESET_MASK, _("R&eset solder mask"));
		menu->Append(ID_REMOVE_CONNECTIONS, _("&Remove connections (rubberbands)"));
		menu->Append(ID_DELETE_OUTSIDE, _("&Delete elements outside the board"));
		menu->AppendSeparator();
		menu->Append(ID_ELEMENT_IMPORT, _("&Text-IO: Import elements"));
		menu->Append(ID_ELEMENT_EXPORT, _("Te&xt-IO: Export elements"));
		menuBar->Append(menu, _("E&xtras"));
	}
	{
		//Options
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_PROPERTIES, _("&General settings"));
		menu->AppendSeparator();
		menu->Append(ID_PANEL_MACRO, _("&Macro-Library"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_PROPERTIES, _("&Properties-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_DRC, _("&DRC-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_COMPONENTS, _("&Components-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->Append(ID_PANEL_SELECTOR, _("S&elector-Panel"), wxEmptyString, wxITEM_CHECK);
		menu->AppendSeparator();
		menu->Append(ID_ZOOM_BOARD, _("Zoom &board"));
		menu->Append(ID_ZOOM_OBJECTS, _("Zoom &objects"));
		menu->Append(ID_ZOOM_SELECTION, _("Zoom &selection"));
		menuBar->Append(menu, _("&Options"));
	}
	{
		//Help
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_ABOUT, _("&About"));
		menuBar->Append(menu, _("&Help"));
	}
	return menuBar;
}

wxToolBar *OpenLayoutFrame::BuildToolBar() {
	wxToolBar *toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxTB_FLAT);
	toolBar->AddTool(wxID_NEW, _("New"), new_xpm, _("New "));
	toolBar->AddTool(wxID_OPEN, _("Open"), open_xpm, _("Open "));
	toolBar->AddTool(wxID_SAVE, _("Save"), save_xpm, _("Save "));
	toolBar->AddTool(wxID_PRINT, _("Print"), print_xpm, _("Print"));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_UNDO, _("Undo"), undo_xpm, _("Undo"));
	toolBar->AddTool(wxID_REDO, _("Redo"), redo_xpm, _("Redo"));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_CUT, _("Cut"), cut_xpm, _("Cut to clipboard"));
	toolBar->AddTool(wxID_COPY, _("Copy"), copy_xpm, _("Copy to clipboard"));
	toolBar->AddTool(wxID_PASTE, _("Paste"), paste_xpm, _("Paste from clipboard"));
	toolBar->AddTool(wxID_DELETE, _("Delete"), delete_xpm, _("Delete"));
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_DUPLICATE, _("Duplicate"), duplicate_xpm, _("Duplicate"));
	toolBar->AddTool(ID_ROTATE, _("Rotate"), rotate_xpm, _("Rotate"), wxITEM_DROPDOWN);
	{
		rotateMenu = new wxMenu();
		rotateMenu->AppendRadioItem(ID_ROTATE_90,	L"90\x00b0");
		rotateMenu->AppendRadioItem(ID_ROTATE_45,	L"45\x00b0");
		rotateMenu->AppendRadioItem(ID_ROTATE_10,	L"10\x00b0");
		rotateMenu->AppendRadioItem(ID_ROTATE_5,	L"5\x00b0");
		rotateMenu->AppendRadioItem(ID_ROTATE_CUSTOM,	wxString::Format(L"%g\x00b0...",
					glutils::RadToDeg(settings.customRotationAngle)));
		rotateMenu->Check(ID_ROTATE_90 + settings.rotationAngleSel, true);
		toolBar->SetDropdownMenu(ID_ROTATE, rotateMenu);
	}
	toolBar->AddTool(ID_HMIRROR, _("Mirror horizontal"), mirror_h_xpm, _("Mirror horisontal"));
	toolBar->AddTool(ID_VMIRROR, _("Mirror vertical"), mirror_v_xpm, _("Mirror vertical"));
	toolBar->AddTool(ID_ALIGN, _("Align elements..."), align_xpm, _("Align elements..."));
	toolBar->AddTool(ID_SNAP_GRID, _("Snap to grid"), to_grid_xpm, _("Snap to grid"));
	toolBar->AddSeparator();
	toolBar->AddTool(ID_GROUP, _("Build group"), group_on_xpm, _("Build group"));
	toolBar->AddTool(ID_UNGROUP, _("Split group"), group_off_xpm, _("Split group"));
	toolBar->AddSeparator();
	toolBar->AddTool(ID_ZOOM, _("Zoom"), zoom_any_xpm, _("Zoom functions"));
	toolBar->AddSeparator();
	toolBar->AddCheckTool(ID_TRANSPARENT, _("Transparent"), transparent_xpm, wxNullBitmap, _("Toggle transparent mode"));
	toolBar->AddTool(wxID_INFO, _("Info"), info_xpm, _("Show project info"));
	toolBar->AddSeparator();
	toolBar->AddTool(ID_SCANNED_COPY, _("Scanned copy"), bitmap_xpm, _("Scanned copy"));
	toolBar->AddStretchableSpace();
	toolBar->AddCheckTool(ID_PANEL_SELECTOR,		_("Selector-Panel"), selector_xpm, wxNullBitmap, _("Show/hide Selector-Panel"));
	toolBar->AddCheckTool(ID_PANEL_COMPONENTS,	_("Component-Panel"), components_xpm, wxNullBitmap, _("Show/hide Component-Panel"));
	toolBar->AddCheckTool(ID_PANEL_PROPERTIES,	_("Properties-Panel"), properties_xpm, wxNullBitmap, _("Show/hide Properties-Panel"));
	toolBar->AddCheckTool(ID_PANEL_DRC,			_("DRC-Panel"), drc_xpm, wxNullBitmap, _("Show/hide DRC-Panel"));
	toolBar->AddCheckTool(ID_PANEL_MACRO,		_("Macro-Library"), macro_xpm, wxNullBitmap, _("Show/hide Macro-Library"));
	toolBar->Realize();
	return toolBar;
}

void OpenLayoutFrame::OpenFile(wxCommandEvent &e) {
	wxFileDialog dialog(this, _("Open layout file"), "", "",
		_("Layout files (*.lay*)|*.lay*|All files (*.*)|*.*"), wxFD_OPEN);

	if (dialog.ShowModal() == wxID_CANCEL)
		return;
	File file(dialog.GetPath().c_str(), "rb");
	if(file.IsOk() && pcb.Load(file)) {
		lastFile = dialog.GetPath();
		UpdatePages();
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
	GetCanvas()->Refresh();
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
	UpdatePages();
	wxMessageBox(wxString::Format(_("A new Board named \"%s\" was added."),
			board->GetName()), GetTitle(), wxICON_INFORMATION);
}
void OpenLayoutFrame::CopyBoard(wxCommandEvent&) {
	Board *copy = new Board(*pcb.GetSelectedBoard());
	char name[30];
	snprintf(name, 30, _("Copy of %s"), copy->GetName());
	copy->SetName(name);
	pcb.AddBoard(copy);
	UpdatePages();
}
void OpenLayoutFrame::DeleteBoard(wxCommandEvent&) {
	wxMessageDialog dialog(this, wxString::Format(_("Delete board \"%s\"?"), 
		pcb.GetSelectedBoard()->GetName()), GetTitle(),
		wxOK | wxCANCEL | wxOK_DEFAULT | wxICON_QUESTION | wxCENTER);
	if(dialog.ShowModal() != wxID_OK)
		return;
	pcb.DeleteSelectedBoard();
	UpdatePages();
}
void OpenLayoutFrame::MoveBoardLeft(wxCommandEvent&) {
	pcb.MoveSelectedBoardLeft();
	UpdatePages();
}
void OpenLayoutFrame::MoveBoardRight(wxCommandEvent&) {
	pcb.MoveSelectedBoardRight();
	UpdatePages();
}
void OpenLayoutFrame::SetBoardLeft(wxCommandEvent&) {
	pcb.SetSelectedBoardLeft();
	UpdatePages();
}
void OpenLayoutFrame::SetBoardRight(wxCommandEvent&) {
	pcb.SetSelectedBoardRight();
	UpdatePages();
}
void OpenLayoutFrame::Rotate(wxCommandEvent&) {
	pcb.GetSelectedBoard()->RotateSelected(settings.GetRotationAngle());
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::MirrorHorizontal(wxCommandEvent&) {
	pcb.GetSelectedBoard()->MirrorSelectedHorizontal();
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::MirrorVertical(wxCommandEvent&) {
	pcb.GetSelectedBoard()->MirrorSelectedVertical();
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::Group(wxCommandEvent&) {
	pcb.GetSelectedBoard()->GroupSelected();
}
void OpenLayoutFrame::Ungroup(wxCommandEvent&) {
	pcb.GetSelectedBoard()->UngroupSelected();
}
void OpenLayoutFrame::ShowAlignMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	AddMenuItem(menu, _("Align top"),				ID_ALIGN_TOP,		align_top_xpm);
	AddMenuItem(menu, _("Align bottom"),			ID_ALIGN_BOTTOM,	align_bottom_xpm);
	AddMenuItem(menu, _("Align left"),				ID_ALIGN_LEFT,		align_left_xpm);
	AddMenuItem(menu, _("Align right"),				ID_ALIGN_RIGHT,		align_right_xpm);
	AddMenuItem(menu, _("Align horizontal-center"),	ID_ALIGN_HCENTER, 	align_hcenter_xpm);
	AddMenuItem(menu, _("Align vertical-center"),	ID_ALIGN_VCENTER, 	align_vcenter_xpm);
	PopupMenu(menu);
}
void OpenLayoutFrame::ShowZoomMenu(wxCommandEvent&) {
	wxMenu *menu = new wxMenu();
	AddMenuItem(menu, _("Zoom board"),		ID_ZOOM_BOARD,		zoom_board_xpm);
	AddMenuItem(menu, _("Zoom objects"),	ID_ZOOM_OBJECTS,	zoom_objects_xpm);
	AddMenuItem(menu, _("Zoom selection"),	ID_ZOOM_SELECTION,	zoom_selection_xpm);
	PopupMenu(menu);
}
void OpenLayoutFrame::DeleteSelected(wxCommandEvent&) {
	pcb.GetSelectedBoard()->DeleteSelected();
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::SelectAll(wxCommandEvent&) {
	pcb.GetSelectedBoard()->SelectAll();
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::SetSelectionLayer(wxCommandEvent&) {}
void OpenLayoutFrame::Delete(wxCommandEvent&) {}
void OpenLayoutFrame::ShowImagesConfig(wxCommandEvent&) {
	pcb.GetSelectedBoard()->images.ShowDialog(this, ColorScheme());
}
void OpenLayoutFrame::ToggleTransparent(wxCommandEvent&) {
	settings.transparent = !settings.transparent;
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::ZoomBoard(wxCommandEvent&) {
	pcb.GetSelectedBoard()->ZoomBoard(GetCanvas()->GetSize());
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::ZoomObjects(wxCommandEvent&) {
	pcb.GetSelectedBoard()->ZoomObjects(GetCanvas()->GetSize());
	GetCanvas()->Refresh();
}
void OpenLayoutFrame::ZoomSelection(wxCommandEvent&) {
	pcb.GetSelectedBoard()->ZoomSelection(GetCanvas()->GetSize());
	GetCanvas()->Refresh();
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
void OpenLayoutFrame::UpdateUIGroup(wxUpdateUIEvent &e) {
	e.Enable(pcb.GetSelectedBoard()->CanGroup());
}
void OpenLayoutFrame::UpdateUIUngroup(wxUpdateUIEvent &e) {
	e.Enable(pcb.GetSelectedBoard()->CanUngroup());
}
void OpenLayoutFrame::UpdateUIMultilayer(wxUpdateUIEvent&) {}
void OpenLayoutFrame::UpdateUIDeleteBoard(wxUpdateUIEvent &e) {
	e.Enable(pcb.Size() > 1);
}
void OpenLayoutFrame::UpdateUIMoveBoardLeft(wxUpdateUIEvent &e) {
	e.Enable(pcb.CanMoveLeft());
}
void OpenLayoutFrame::UpdateUIMoveBoardRight(wxUpdateUIEvent &e) {
	e.Enable(pcb.CanMoveRight());
}

void OpenLayoutFrame::SetRotationAngle(wxCommandEvent &e) {
	if(e.GetId() == ID_ROTATE_CUSTOM) {
		wxDialog *dialog = new wxDialog(this, wxID_ANY, _("Rotational angle"));
		wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);
		wxSpinCtrlDouble *input = new wxSpinCtrlDouble(dialog, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -360.0f, 360.0f,
			glutils::RadToDeg(settings.customRotationAngle), 0.05);
		box->Add(input, 1, wxEXPAND | wxALL, 5);
		box->Add(dialog->CreateSeparatedButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL);
		input->SetFocus();
		dialog->SetSizerAndFit(box);
		if(dialog->ShowModal() == wxID_OK) {
			settings.customRotationAngle = glutils::DegToRad(input->GetValue());
			settings.rotationAngleSel = ANGLE_CUSTOM;
			rotateMenu->SetLabel(ID_ROTATE_CUSTOM, wxString::Format(L"%g\x00b0...", input->GetValue()));
		}
	}
	else
		settings.rotationAngleSel = e.GetId() - ID_ROTATE_90;
}

void OpenLayoutFrame::SelectPage(wxBookCtrlEvent &e) {
	pcb.SetTab(e.GetSelection());
	GetCanvas()->Refresh();
}

MainCanvas *OpenLayoutFrame::GetCanvas() {
	return (MainCanvas*) pages->GetCurrentPage();
}

void OpenLayoutFrame::UpdatePages() {
	uint32_t tab = pcb.GetTab();
	pages->DeleteAllPages();
	for(int i = 0; i < pcb.Size(); i++) {
		MainCanvas *canvas = new MainCanvas(pages, pcb[i], settings);
		pages->AddPage(canvas, pcb[i]->GetName(), tab == i);
	}
}

