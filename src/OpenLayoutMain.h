#pragma once

#include "PCB.h"
#include "Settings.h"
#include "MainCanvas.h"
#include "MacroPanel.h"
#include "ComponentsPanel.h"
#include "SelectorPanel.h"
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/notebook.h>

class OpenLayoutFrame : public wxFrame{
public:
	OpenLayoutFrame();
private:
	wxToolBar *BuildToolBar();
	wxMenuBar *BuildMenuBar();

	static wxString GetDir();
	void SaveSettings() const;
	void LoadSettings();

	void Move(wxMoveEvent&);
	void Resize(wxSizeEvent&);
	void Close(wxCloseEvent&);
	void Close(wxCommandEvent&);
	void ShowSettings(wxCommandEvent&);
	void ShowAbout(wxCommandEvent&);
	void ShowProjectInfo(wxCommandEvent&);
	void NewBoard(wxCommandEvent&);
	void CopyBoard(wxCommandEvent&);
	void DeleteBoard(wxCommandEvent&);
	void MoveBoardLeft(wxCommandEvent&);
	void MoveBoardRight(wxCommandEvent&);
	void SetBoardLeft(wxCommandEvent&);
	void SetBoardRight(wxCommandEvent&);

	void Rotate(wxCommandEvent&);
	void MirrorHorizontal(wxCommandEvent&);
	void MirrorVertical(wxCommandEvent&);
	void AlignTop(wxCommandEvent&);
	void AlignBottom(wxCommandEvent&);
	void AlignLeft(wxCommandEvent&);
	void AlignRight(wxCommandEvent&);
	void AlignHCenter(wxCommandEvent&);
	void AlignVCenter(wxCommandEvent&);
	void SnapToGrid(wxCommandEvent&);
	void Group(wxCommandEvent&);
	void Ungroup(wxCommandEvent&);
	void ShowAlignMenu(wxCommandEvent&);
	void ShowZoomMenu(wxCommandEvent&);
	void DeleteSelected(wxCommandEvent&);
	void SelectAll(wxCommandEvent&);
	void SaveFile(wxCommandEvent&);
	void SaveFileAs(wxCommandEvent&);
	void OpenFile(wxCommandEvent&);
	void SetSelectionLayer(wxCommandEvent&);
	void Delete(wxCommandEvent&);
	void ShowImagesConfig(wxCommandEvent&);
	void ToggleTransparent(wxCommandEvent&);
	void ZoomBoard(wxCommandEvent&);
	void ZoomObjects(wxCommandEvent&);
	void ZoomSelection(wxCommandEvent&);

	void ToggleSelectorPanel(wxCommandEvent&);
	void ToggleComponentsPanel(wxCommandEvent&);
	void TogglePropertiesPanel(wxCommandEvent&);
	void ToggleDRCPanel(wxCommandEvent&);
	void ToggleMacrosPanel(wxCommandEvent&);

	void UpdateSelectorPanel(wxUpdateUIEvent&);
	void UpdateComponentsPanel(wxUpdateUIEvent&);
	void UpdatePropertiesPanel(wxUpdateUIEvent&);
	void UpdateDRCPanel(wxUpdateUIEvent&);
	void UpdateMacrosPanel(wxUpdateUIEvent&);

	void UpdateUIObjects(wxUpdateUIEvent&);
	void UpdateUISelection(wxUpdateUIEvent&);
	void UpdateUISelectionTwo(wxUpdateUIEvent&);
	void UpdateUIGroup(wxUpdateUIEvent&);
	void UpdateUIUngroup(wxUpdateUIEvent&);
	void UpdateUIMultilayer(wxUpdateUIEvent&);
	void UpdateUIDeleteBoard(wxUpdateUIEvent&);
	void UpdateUIMoveBoardLeft(wxUpdateUIEvent&);
	void UpdateUIMoveBoardRight(wxUpdateUIEvent&);

	void SetRotationAngle(wxCommandEvent&);
	void SelectPage(wxBookCtrlEvent&);
	MainCanvas *GetCanvas();
	void UpdatePages();

	PCB pcb;
	Settings settings;

	wxMenu *rotateMenu;
	wxNotebook *pages;
	SelectorPanel *selector;
	ComponentsPanel *components;
	MacroPanel *macro;

	wxString lastFile;
	wxDECLARE_EVENT_TABLE();
};
