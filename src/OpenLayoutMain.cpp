#include "OpenLayoutMain.h"
#include "MenuBar.h"
#include "ToolBar.h"
#include "NewBoardDialog.h"

wxBEGIN_EVENT_TABLE(OpenLayoutFrame, wxFrame)
	EVT_MENU(wxID_EXIT, OpenLayoutFrame::Close)
	EVT_MENU(wxID_PROPERTIES, OpenLayoutFrame::ShowSettings)
	EVT_MENU(wxID_ABOUT, OpenLayoutFrame::ShowAbout)
	EVT_MENU(wxID_INFO, OpenLayoutFrame::ShowProjectInfo)
	EVT_MENU(ID_BOARD_NEW, OpenLayoutFrame::NewBoard)
	EVT_MENU(ID_SCANNED_COPY, OpenLayoutFrame::ShowImagesConfig)
	EVT_MENU(ID_GROUP, OpenLayoutFrame::Group)
	EVT_MENU(ID_UNGROUP, OpenLayoutFrame::Ungroup)
	EVT_MENU(wxID_SELECTALL, OpenLayoutFrame::SelectAll)
	EVT_MENU(wxID_SAVE, OpenLayoutFrame::SaveFile)
	EVT_MENU(wxID_SAVEAS, OpenLayoutFrame::SaveFileAs)
	EVT_MENU(wxID_OPEN, OpenLayoutFrame::OpenFile)
	EVT_MENU_RANGE(ID_LAYER_C1, ID_LAYER_O, OpenLayoutFrame::SetSelectionLayer)
	EVT_UPDATE_UI(wxID_COPY, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(wxID_CUT, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(wxID_DELETE, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(wxID_DUPLICATE, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_ROTATE, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_HMIRROR, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_VMIRROR, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_SNAP_GRID, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI(ID_GROUP, OpenLayoutFrame::UpdateUIGroup)
	EVT_UPDATE_UI(ID_UNGROUP, OpenLayoutFrame::UpdateUIUngroup)
	EVT_UPDATE_UI_RANGE(ID_LAYER_C1, ID_LAYER_S2, OpenLayoutFrame::UpdateUIEdit)
	EVT_UPDATE_UI_RANGE(ID_LAYER_I1, ID_LAYER_I2, OpenLayoutFrame::UpdateUIMultilayer)
	EVT_UPDATE_UI(ID_LAYER_O, OpenLayoutFrame::UpdateUIEdit)
wxEND_EVENT_TABLE()

OpenLayoutFrame::OpenLayoutFrame()
		:wxFrame(0, wxID_ANY, "OpenLayout",
		wxDefaultPosition, wxSize(800, 600)){
	MenuBar *menubar = new MenuBar();
	SetMenuBar(menubar);
	ToolBar *toolbar = new ToolBar(this);
	SetToolBar(toolbar);
}

void OpenLayoutFrame::Close(wxCommandEvent&){}
void OpenLayoutFrame::ShowSettings(wxCommandEvent&){}
void OpenLayoutFrame::ShowAbout(wxCommandEvent&){}
void OpenLayoutFrame::ShowProjectInfo(wxCommandEvent&){}
void OpenLayoutFrame::NewBoard(wxCommandEvent&){
	NewBoardDialog dialog(this);
	dialog.ShowModal();
}
void OpenLayoutFrame::Group(wxCommandEvent&){}
void OpenLayoutFrame::Ungroup(wxCommandEvent&){}
void OpenLayoutFrame::SelectAll(wxCommandEvent&){}
void OpenLayoutFrame::SaveFile(wxCommandEvent&){}
void OpenLayoutFrame::SaveFileAs(wxCommandEvent&){}
void OpenLayoutFrame::OpenFile(wxCommandEvent&){}
void OpenLayoutFrame::SetSelectionLayer(wxCommandEvent&){}
void OpenLayoutFrame::Delete(wxCommandEvent&){}
void OpenLayoutFrame::ShowImagesConfig(wxCommandEvent&){}

void OpenLayoutFrame::UpdateUIEdit(wxUpdateUIEvent&){}
void OpenLayoutFrame::UpdateUIGroup(wxUpdateUIEvent&){}
void OpenLayoutFrame::UpdateUIUngroup(wxUpdateUIEvent&){}
void OpenLayoutFrame::UpdateUIMultilayer(wxUpdateUIEvent&){}
