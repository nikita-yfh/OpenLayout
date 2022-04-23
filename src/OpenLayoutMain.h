#pragma once

#include "PCB.h"
#include "MacroPanel.h"
#include "ComponentPanel.h"
#include "SelectorPanel.h"
#include <wx/frame.h>

enum {
	ID_SAVE_MACRO=1,
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
	ID_BOARD_NEW,
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
	ID_ROTATE_CUSTOM_CH,
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
	ID_ZOOM_PREVIOUS,
	ID_ZOOM_BOARD,
	ID_ZOOM_OBJECTS,
	ID_ZOOM_SELECTION,
};

class OpenLayoutFrame : public wxFrame{
public:
	OpenLayoutFrame();
private:
	void Close(wxCommandEvent&);
	void ShowSettings(wxCommandEvent&);
	void ShowAbout(wxCommandEvent&);
	void ShowProjectInfo(wxCommandEvent&);
	void NewBoard(wxCommandEvent&);
	void Group(wxCommandEvent&);
	void Ungroup(wxCommandEvent&);
	void SelectAll(wxCommandEvent&);
	void SaveFile(wxCommandEvent&);
	void SaveFileAs(wxCommandEvent&);
	void OpenFile(wxCommandEvent&);
	void SetSelectionLayer(wxCommandEvent&);
	void Delete(wxCommandEvent&);
	void ShowImagesConfig(wxCommandEvent&);

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

	void UpdateUIEdit(wxUpdateUIEvent&);
	void UpdateUIGroup(wxUpdateUIEvent&);
	void UpdateUIUngroup(wxUpdateUIEvent&);
	void UpdateUIMultilayer(wxUpdateUIEvent&);

	PCB pcb;

	SelectorPanel *selector;
	ComponentPanel *components;
	MacroPanel *macros;
	wxDECLARE_EVENT_TABLE();
};
