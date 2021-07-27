/***************************************************************
 * Name:      OpenLayoutMain.h
 * Purpose:   Defines Application Frame
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:
 **************************************************************/

#ifndef OPENLAYOUTMAIN_H
#define OPENLAYOUTMAIN_H

#include <vector>
#include "Board.h"
#include "PCBFile.h"
#include "Settings.h"

using namespace std;


enum class Tool{
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

//(*Headers(OpenLayoutFrame)
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
//*)

class OpenLayoutFrame: public wxFrame {
public:

    OpenLayoutFrame(wxWindow* parent=nullptr,wxWindowID id = -1);
    virtual ~OpenLayoutFrame();
private:

    //(*Handlers(OpenLayoutFrame)
    void OnQuit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnGridSize1Selected(wxCommandEvent& event);
    void OnGridSize2Selected(wxCommandEvent& event);
    void OnGridSize3Selected(wxCommandEvent& event);
    void OnGridSize4Selected(wxCommandEvent& event);
    void OnGridSize5Selected(wxCommandEvent& event);
    void OnGridSize6Selected(wxCommandEvent& event);
    void OnGridSize7Selected(wxCommandEvent& event);
    void OnGridSize8Selected(wxCommandEvent& event);
    void OnGridSize9Selected(wxCommandEvent& event);
    void OnGridSize10Selected(wxCommandEvent& event);
    void OnGridSize11Selected(wxCommandEvent& event);
    void OnGridSize12Selected(wxCommandEvent& event);
    void OnGridSize13Selected(wxCommandEvent& event);
    void OnGridSize14Selected(wxCommandEvent& event);
    void OnGridSize15Selected(wxCommandEvent& event);
    void OnGridSize16Selected(wxCommandEvent& event);
    void OnGridSize17Selected(wxCommandEvent& event);
    void OnGridSize18Selected(wxCommandEvent& event);
    void OnGridSize19Selected(wxCommandEvent& event);
    void OnTrackSizeVChange(wxSpinDoubleEvent& event);
    void OnPadSize1Change(wxSpinDoubleEvent& event);
    void OnPadSize2Change(wxSpinDoubleEvent& event);
    void OnPadSMDWChange(wxSpinDoubleEvent& event);
    void OnPadSMDHChange(wxSpinDoubleEvent& event);
    void OnGridSizeClick(wxCommandEvent& event);
    void AddNewBoard(wxCommandEvent& event);
    void OnPanel2Paint(wxPaintEvent& event);
    void OnBoardChoiceSelect(wxCommandEvent& event);
    void OnLayerHelpButtonClick(wxCommandEvent& event);
    void OnProjectInfoButtonClick(wxCommandEvent& event);
    void OnSettingsButtonClick(wxCommandEvent& event);
    //*)

    //(*Identifiers(OpenLayoutFrame)
    static const long ID_RADIOBOX1;
    static const long ID_BITMAPBUTTON4;
    static const long ID_BUTTON1;
    static const long ID_BITMAPBUTTON1;
    static const long ID_SPINCTRLDOUBLE1;
    static const long ID_BITMAPBUTTON2;
    static const long ID_SPINCTRLDOUBLE3;
    static const long ID_SPINCTRLDOUBLE2;
    static const long ID_BITMAPBUTTON3;
    static const long ID_SPINCTRLDOUBLE4;
    static const long ID_SPINCTRLDOUBLE5;
    static const long ID_PANEL1;
    static const long ID_CHOICE1;
    static const long ID_PANEL2;
    static const long ID_SCROLLEDWINDOW1;
    static const long ID_STATICTEXT1;
    static const long ID_TOGGLEBUTTON1;
    static const long ID_TOGGLEBUTTON2;
    static const long ID_TOGGLEBUTTON3;
    static const long ID_TOGGLEBUTTON4;
    static const long ID_TOGGLEBUTTON5;
    static const long ID_TOGGLEBUTTON6;
    static const long ID_TOGGLEBUTTON7;
    static const long ID_STATICTEXT2;
    static const long ID_RADIOBUTTON1;
    static const long ID_RADIOBUTTON2;
    static const long ID_RADIOBUTTON3;
    static const long ID_RADIOBUTTON4;
    static const long ID_RADIOBUTTON5;
    static const long ID_RADIOBUTTON6;
    static const long ID_RADIOBUTTON7;
    static const long ID_PANEL4;
    static const long ID_BUTTON2;
    static const long ID_MENUITEM1;
    static const long ID_MENUITEM2;
    static const long ID_MENUITEM3;
    static const long ID_MENUITEM4;
    static const long ID_MENUITEM5;
    static const long ID_MENUITEM6;
    static const long ID_MENUITEM7;
    static const long ID_MENUITEM9;
    static const long ID_MENUITEM10;
    static const long ID_MENUITEM11;
    static const long ID_MENUITEM12;
    static const long ID_MENUITEM13;
    static const long ID_MENUITEM14;
    static const long ID_MENUITEM15;
    static const long ID_MENUITEM16;
    static const long ID_MENUITEM8;
    static const long ID_MENUITEM17;
    static const long ID_MENUITEM18;
    static const long ID_MENUITEM19;
    static const long ID_MENUITEM20;
    static const long ID_MENUITEM21;
    static const long ID_MENUITEM22;
    static const long ID_MENUITEM23;
    static const long ID_MENUITEM24;
    static const long ID_MENUITEM25;
    static const long ID_MENUITEM26;
    static const long ID_MENUITEM27;
    static const long ID_MENUITEM28;
    static const long ID_MENUITEM29;
    static const long ID_MENUITEM30;
    static const long ID_MENUITEM31;
    static const long ID_MENUITEM32;
    static const long ID_MENUITEM33;
    static const long ID_MENUITEM34;
    static const long ID_MENUITEM35;
    static const long ID_MENUITEM36;
    static const long ID_MENUITEM37;
    static const long ID_MENUITEM38;
    static const long ID_MENUITEM39;
    static const long ID_MENUITEM40;
    static const long ID_MENUITEM41;
    static const long ID_MENUITEM42;
    static const long ID_MENUITEM43;
    static const long ID_MENUITEM44;
    static const long ID_MENUITEM45;
    static const long ID_MENUITEM46;
    static const long ID_MENUITEM47;
    static const long ID_MENUITEM48;
    static const long ID_MENUITEM49;
    static const long ID_MENUITEM50;
    static const long ID_MENUITEM51;
    static const long ID_MENUITEM52;
    static const long ID_MENUITEM53;
    static const long ID_MENUITEM54;
    static const long ID_MENUITEM55;
    static const long ID_MENUITEM56;
    static const long ID_MENUITEM57;
    static const long ID_MENUITEM58;
    static const long ID_MENUITEM59;
    static const long ID_MENUITEM60;
    static const long ID_MENUITEM61;
    static const long ID_MENUITEM62;
    static const long ID_MENUITEM63;
    static const long ID_MENUITEM64;
    static const long ID_MENUITEM65;
    static const long ID_MENUITEM66;
    static const long ID_MENUITEM67;
    static const long ID_MENUITEM68;
    static const long ID_MENUITEM69;
    static const long ID_TOOLBARITEM1;
    static const long OpenFile;
    static const long SaveFile;
    static const long ID_TOOLBARITEM2;
    static const long ID_TOOLBARITEM3;
    static const long ID_TOOLBARITEM4;
    static const long ID_TOOLBARITEM5;
    static const long ID_TOOLBARITEM6;
    static const long ID_TOOLBARITEM7;
    static const long ID_TOOLBARITEM8;
    static const long ID_TOOLBARITEM9;
    static const long ID_TOOLBARITEM10;
    static const long ID_TOOLBARITEM11;
    static const long ID_TOOLBARITEM12;
    static const long ID_TOOLBARITEM13;
    static const long ID_TOOLBARITEM14;
    static const long ID_TOOLBARITEM15;
    static const long ID_TOOLBARITEM16;
    static const long ID_TOOLBARITEM17;
    static const long ID_TOOLBARITEM18;
    static const long ID_TOOLBARITEM19;
    static const long ID_TOOLBARITEM20;
    static const long ID_TOOLBAR1;
    static const long ID_MENUITEM70;
    static const long ID_MENUITEM71;
    static const long ID_MENUITEM72;
    static const long ID_MENUITEM73;
    static const long ID_MENUITEM74;
    static const long ID_MENUITEM75;
    static const long ID_MENUITEM76;
    static const long ID_MENUITEM77;
    static const long ID_MENUITEM79;
    static const long ID_MENUITEM80;
    static const long ID_MENUITEM81;
    static const long ID_MENUITEM82;
    static const long ID_MENUITEM83;
    static const long ID_MENUITEM84;
    static const long ID_MENUITEM85;
    static const long ID_MENUITEM86;
    static const long ID_MENUITEM87;
    static const long ID_MENUITEM88;
    static const long ID_MENUITEM89;
    static const long ID_MENUITEM78;
    //*)

    //(*Declarations(OpenLayoutFrame)
    wxBitmapButton* GridSizeB;
    wxBitmapButton* PadSMDV;
    wxBitmapButton* PadSizeB;
    wxBitmapButton* TrackSizeB;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxButton* GridSizeV;
    wxButton* LayerHelpButton;
    wxChoice* BoardChoice;
    wxMenu GridMenu;
    wxMenu* Menu2;
    wxMenu* Menu3;
    wxMenu* Menu4;
    wxMenu* Menu5;
    wxMenu* Menu6;
    wxMenu* Menu7;
    wxMenu* MenuItem78;
    wxMenu* MenuItem9;
    wxMenuItem* MenuItem10;
    wxMenuItem* MenuItem11;
    wxMenuItem* MenuItem12;
    wxMenuItem* MenuItem13;
    wxMenuItem* MenuItem14;
    wxMenuItem* MenuItem15;
    wxMenuItem* MenuItem16;
    wxMenuItem* MenuItem17;
    wxMenuItem* MenuItem18;
    wxMenuItem* MenuItem19;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem20;
    wxMenuItem* MenuItem21;
    wxMenuItem* MenuItem22;
    wxMenuItem* MenuItem23;
    wxMenuItem* MenuItem24;
    wxMenuItem* MenuItem25;
    wxMenuItem* MenuItem26;
    wxMenuItem* MenuItem27;
    wxMenuItem* MenuItem28;
    wxMenuItem* MenuItem29;
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem30;
    wxMenuItem* MenuItem31;
    wxMenuItem* MenuItem32;
    wxMenuItem* MenuItem33;
    wxMenuItem* MenuItem34;
    wxMenuItem* MenuItem35;
    wxMenuItem* MenuItem36;
    wxMenuItem* MenuItem37;
    wxMenuItem* MenuItem38;
    wxMenuItem* MenuItem39;
    wxMenuItem* MenuItem3;
    wxMenuItem* MenuItem40;
    wxMenuItem* MenuItem41;
    wxMenuItem* MenuItem42;
    wxMenuItem* MenuItem43;
    wxMenuItem* MenuItem44;
    wxMenuItem* MenuItem45;
    wxMenuItem* MenuItem46;
    wxMenuItem* MenuItem47;
    wxMenuItem* MenuItem48;
    wxMenuItem* MenuItem49;
    wxMenuItem* MenuItem4;
    wxMenuItem* MenuItem50;
    wxMenuItem* MenuItem51;
    wxMenuItem* MenuItem52;
    wxMenuItem* MenuItem53;
    wxMenuItem* MenuItem54;
    wxMenuItem* MenuItem55;
    wxMenuItem* MenuItem56;
    wxMenuItem* MenuItem57;
    wxMenuItem* MenuItem58;
    wxMenuItem* MenuItem59;
    wxMenuItem* MenuItem5;
    wxMenuItem* MenuItem60;
    wxMenuItem* MenuItem61;
    wxMenuItem* MenuItem62;
    wxMenuItem* MenuItem63;
    wxMenuItem* MenuItem64;
    wxMenuItem* MenuItem65;
    wxMenuItem* MenuItem66;
    wxMenuItem* MenuItem67;
    wxMenuItem* MenuItem68;
    wxMenuItem* MenuItem69;
    wxMenuItem* MenuItem6;
    wxMenuItem* MenuItem70;
    wxMenuItem* MenuItem71;
    wxMenuItem* MenuItem72;
    wxMenuItem* MenuItem73;
    wxMenuItem* MenuItem74;
    wxMenuItem* MenuItem75;
    wxMenuItem* MenuItem76;
    wxMenuItem* MenuItem77;
    wxMenuItem* MenuItem79;
    wxMenuItem* MenuItem7;
    wxMenuItem* MenuItem80;
    wxMenuItem* MenuItem81;
    wxMenuItem* MenuItem82;
    wxMenuItem* MenuItem83;
    wxMenuItem* MenuItem84;
    wxMenuItem* MenuItem85;
    wxMenuItem* MenuItem86;
    wxMenuItem* MenuItem87;
    wxMenuItem* MenuItem88;
    wxMenuItem* MenuItem89;
    wxMenuItem* MenuItem8;
    wxPanel* Canvas;
    wxPanel* Panel1;
    wxPanel* Panel3;
    wxRadioBox* RadioBox1;
    wxRadioButton* RadioButton1;
    wxRadioButton* RadioButton2;
    wxRadioButton* RadioButton3;
    wxRadioButton* RadioButton4;
    wxRadioButton* RadioButton5;
    wxRadioButton* RadioButton6;
    wxRadioButton* RadioButton7;
    wxScrolledWindow* Scroll;
    wxSpinCtrlDouble* PadSMDH;
    wxSpinCtrlDouble* PadSMDW;
    wxSpinCtrlDouble* PadSize1;
    wxSpinCtrlDouble* PadSize2;
    wxSpinCtrlDouble* TrackSizeV;
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;
    wxToggleButton* ToggleButton1;
    wxToggleButton* ToggleButton2;
    wxToggleButton* ToggleButton3;
    wxToggleButton* ToggleButton4;
    wxToggleButton* ToggleButton5;
    wxToggleButton* ToggleButton6;
    wxToggleButton* ToggleButton7;
    wxToolBar* ToolBar;
    wxToolBarToolBase* ToolBarItem10;
    wxToolBarToolBase* ToolBarItem11;
    wxToolBarToolBase* ToolBarItem12;
    wxToolBarToolBase* ToolBarItem13;
    wxToolBarToolBase* ToolBarItem14;
    wxToolBarToolBase* ToolBarItem15;
    wxToolBarToolBase* ToolBarItem16;
    wxToolBarToolBase* ToolBarItem17;
    wxToolBarToolBase* ToolBarItem18;
    wxToolBarToolBase* ToolBarItem19;
    wxToolBarToolBase* ToolBarItem1;
    wxToolBarToolBase* ToolBarItem20;
    wxToolBarToolBase* ToolBarItem21;
    wxToolBarToolBase* ToolBarItem22;
    wxToolBarToolBase* ToolBarItem2;
    wxToolBarToolBase* ToolBarItem3;
    wxToolBarToolBase* ToolBarItem4;
    wxToolBarToolBase* ToolBarItem5;
    wxToolBarToolBase* ToolBarItem6;
    wxToolBarToolBase* ToolBarItem7;
    wxToolBarToolBase* ToolBarItem8;
    wxToolBarToolBase* ToolBarItem9;
    //*)


    float track_size;

    float pad_r1;
    float pad_r2;

    float pad_smd_w;
    float pad_smd_h;

    void SetDefaults();
    void SwapSMDSize();

    Settings s;

	void SetGridSize(float,bool micro=false);
	void SetTrackSize(float,bool set=false);
	void SetPadSize1(float,bool set=false);
	void SetPadSize2(float,bool set=false);
	void SetPadSMDW(float,bool set=false);
	void SetPadSMDH(float,bool set=false);

	PCBFile file;

    DECLARE_EVENT_TABLE()
};

#endif
