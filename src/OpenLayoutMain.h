#pragma once
#include <vector>
#include <wx/dcclient.h>
#include "PCBFile.h"
#include "Settings.h"


using namespace std;

enum class Tool {
    Edit,
    Zoom,
    Track,
    Pad,
    SMDPad,
    Circle,
    Rectangle,
    Zone,
    SpecialForm,
    Text,
    Connection,
    Autoroute,
    Test,
    Measure,
    Photoview,
    SolderMask
};

#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/radiobut.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/tglbtn.h>
#include <wx/toolbar.h>
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
    ID_VMIRROR,
    ID_HMIRROR,
    ID_GROUP,
    ID_UNGROUP,
    ID_CHANGE_SIDE,
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
    ID_PANEL_MACRO,
    ID_PANEL_PROPERTIES,
    ID_PANEL_DRC,
    ID_PANEL_COMPONENTS,
    ID_PANEL_SELECTOR,
    ID_ALIGN,
    ID_TRANSPARENT,
    ID_ZOOM,
    ID_ZOOM_BOARD,
    ID_ZOOM_OBJECTS,
    ID_ZOOM_SELECTION,
    ID_ZOOM_PREVIOUS,
    ID_TOOL_EDIT,
    ID_TOOL_ZOOM,
    ID_TOOL_TRACK,
    ID_TOOL_PAD,
    ID_TOOL_SMD_PAD,
    ID_TOOL_CIRCLE,
    ID_TOOL_RECTANGLE,
    ID_TOOL_ZONE,
    ID_TOOL_FORM,
    ID_TOOL_TEXT,
    ID_TOOL_SOLDER_MASK,
    ID_TOOL_CONNECTIONS,
    ID_TOOL_AUTOROUTE,
    ID_TOOL_TEST,
    ID_TOOL_MEASURE,
    ID_TOOL_PHOTOVIEW,

    ID_TRACK_SEL,
    ID_TRACK_ADD=ID_TRACK_SEL+100,
    ID_TRACK_DEL,
    ID_PAD_SEL=ID_TRACK_DEL+100,
    ID_PAD_ADD=ID_PAD_SEL+100,
    ID_PAD_DEL,
    ID_SMD_SEL=ID_PAD_DEL+100,
    ID_SMD_ADD=ID_SMD_SEL+100,
    ID_SMD_DEL,

    ID_GRID_NORMAL,
    ID_GRID_METRIC=ID_GRID_NORMAL+8,
    ID_GRID_USER=ID_GRID_METRIC+11,
    ID_GRID_USER_DEL=ID_GRID_USER+100,
    ID_GRID_USER_NEW=ID_GRID_USER_DEL+100,

    ID_GRID_HOTKEYS,

    ID_GRID_LINES,
    ID_GRID_DOTS,

    ID_SUBGRID_OFF,
    ID_SUBGRID_2,
    ID_SUBGRID_4,
    ID_SUBGRID_5,
    ID_SUBGRID_10,

    ID_SHOW_GRID,

    ID_ANGLE_90,
    ID_ANGLE_45,
    ID_ANGLE_10,
    ID_ANGLE_5,
    ID_ANGLE_CUSTOM,
    ID_ANGLE_CUSTOM_CH
};
class OpenLayoutFrame: public wxFrame {
public:

    OpenLayoutFrame();
private:

    wxButton *grid_button;

    uint8_t tool;

    Vec2 smd_size{1.6f,0.9f};
    Vec2 pad_size{1.8f,0.6f};
    float track_size=0.8f;
    void set_smd_size(Vec2 size);
    void set_pad_size(Vec2 size);
    void set_track_size(float size);
    void set_grid_size(double grid);

    wxSpinCtrlDouble *w_smd_w;
    wxSpinCtrlDouble *w_smd_h;
    wxSpinCtrlDouble *w_pad_outer;
    wxSpinCtrlDouble *w_pad_inner;
    wxSpinCtrlDouble *w_track_size;

    void build_smd_menu();
    void build_pad_menu();
    void build_track_menu();
    void build_grid_menu();
    void init_menu_bar();
    void init_tool_bar(wxBoxSizer*);
    void init_left_panel(wxBoxSizer*);
    void close(wxCommandEvent&);
    void show_settings(wxCommandEvent&);
    void show_about(wxCommandEvent&);
    void show_project_info(wxCommandEvent&);
    void new_board(wxCommandEvent&);
    void group(wxCommandEvent&);
    void ungroup(wxCommandEvent&);
    void show_scan_properties(wxCommandEvent&);
    void swap_smd_size(wxCommandEvent&);
    void set_grid(float val,bool metric=true);

    void updateui_edit(wxUpdateUIEvent&);
    void updateui_group(wxUpdateUIEvent&);
    void updateui_ungroup(wxUpdateUIEvent&);
    wxDECLARE_EVENT_TABLE();
};
