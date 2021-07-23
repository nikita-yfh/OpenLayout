#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H
#include "Settings.h"

//(*Headers(SettingsDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dialog.h>
#include <wx/filepicker.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

class SettingsDialog: public wxDialog {
public:
    SettingsDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
    virtual ~SettingsDialog();

    void Set(const Settings s);
    void Get(Settings &s);

    Settings s;

    //(*Declarations(SettingsDialog)
    wxButton* Button2;
    wxButton* Button3;
    wxButton* ResetColor;
    wxCheckBox* AutosaveButton;
    wxCheckBox* CheckBox10;
    wxCheckBox* CheckBox11;
    wxCheckBox* CheckBox13;
    wxCheckBox* CheckBox15;
    wxCheckBox* CheckBox16;
    wxCheckBox* CheckBox1;
    wxCheckBox* CheckBox2;
    wxCheckBox* CheckBox3;
    wxCheckBox* CheckBox4;
    wxCheckBox* CheckBox5;
    wxCheckBox* CheckBox6;
    wxCheckBox* CheckBox7;
    wxCheckBox* CheckBox8;
    wxCheckBox* CheckBox9;
    wxCheckBox* CrosshairCoord;
    wxCheckBox* SameDir;
    wxChoice* Choice1;
    wxChoice* Choice2;
    wxChoice* ColorSchemeChoise;
    wxChoice* HotkeyChoise;
    wxColourPickerCtrl* ColorBgr;
    wxColourPickerCtrl* ColorC1;
    wxColourPickerCtrl* ColorC2;
    wxColourPickerCtrl* ColorCon;
    wxColourPickerCtrl* ColorDots;
    wxColourPickerCtrl* ColorI1;
    wxColourPickerCtrl* ColorI2;
    wxColourPickerCtrl* ColorLines;
    wxColourPickerCtrl* ColorO;
    wxColourPickerCtrl* ColorS1;
    wxColourPickerCtrl* ColorS2;
    wxColourPickerCtrl* ColorVia;
    wxDirPickerCtrl* DirPickerCtrl1;
    wxDirPickerCtrl* DirPickerCtrl2;
    wxDirPickerCtrl* DirPickerCtrl3;
    wxDirPickerCtrl* DirPickerCtrl4;
    wxDirPickerCtrl* DirPickerCtrl5;
    wxDirPickerCtrl* DirPickerCtrl6;
    wxListBox* HotkeyList;
    wxNotebook* Notebook1;
    wxPanel* ColorSchemePanel;
    wxPanel* OtherDirsPanel;
    wxPanel* Panel10;
    wxPanel* Panel11;
    wxPanel* Panel13;
    wxPanel* Panel1;
    wxPanel* Panel2;
    wxPanel* Panel3;
    wxPanel* Panel4;
    wxPanel* Panel5;
    wxPanel* Panel6;
    wxPanel* Panel7;
    wxPanel* Panel8;
    wxPanel* Panel9;
    wxRadioButton* RadioButton1;
    wxRadioButton* RadioButton2;
    wxSpinCtrl* SpinCtrl1;
    wxSpinCtrl* SpinCtrl2;
    wxSpinCtrl* SpinCtrl3;
    wxSpinCtrl* SpinCtrl4;
    wxStaticText* HotkeyLabel;
    wxStaticText* StaticText10;
    wxStaticText* StaticText11;
    wxStaticText* StaticText12;
    wxStaticText* StaticText13;
    wxStaticText* StaticText14;
    wxStaticText* StaticText15;
    wxStaticText* StaticText16;
    wxStaticText* StaticText17;
    wxStaticText* StaticText18;
    wxStaticText* StaticText19;
    wxStaticText* StaticText1;
    wxStaticText* StaticText20;
    wxStaticText* StaticText21;
    wxStaticText* StaticText22;
    wxStaticText* StaticText23;
    wxStaticText* StaticText24;
    wxStaticText* StaticText25;
    wxStaticText* StaticText26;
    wxStaticText* StaticText27;
    wxStaticText* StaticText28;
    wxStaticText* StaticText29;
    wxStaticText* StaticText2;
    wxStaticText* StaticText30;
    wxStaticText* StaticText31;
    wxStaticText* StaticText32;
    wxStaticText* StaticText33;
    wxStaticText* StaticText3;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;
    wxStaticText* StaticText6;
    wxStaticText* StaticText7;
    wxStaticText* StaticText8;
    wxStaticText* StaticText9;
    //*)

protected:

    //(*Identifiers(SettingsDialog)
    static const long ID_STATICTEXT1;
    static const long ID_CHOICE1;
    static const long ID_STATICTEXT2;
    static const long ID_CHOICE2;
    static const long ID_CHECKBOX1;
    static const long ID_CHECKBOX2;
    static const long ID_CHECKBOX3;
    static const long ID_CHECKBOX4;
    static const long ID_CHECKBOX5;
    static const long ID_CHECKBOX6;
    static const long ID_CHECKBOX7;
    static const long ID_CHECKBOX8;
    static const long ID_CHECKBOX9;
    static const long ID_CHECKBOX10;
    static const long ID_CHECKBOX11;
    static const long ID_PANEL1;
    static const long ID_STATICTEXT3;
    static const long ID_CHOICE3;
    static const long ID_COLOURPICKERCTRL1;
    static const long ID_STATICTEXT4;
    static const long ID_COLOURPICKERCTRL2;
    static const long ID_STATICTEXT5;
    static const long ID_COLOURPICKERCTRL3;
    static const long ID_STATICTEXT6;
    static const long ID_COLOURPICKERCTRL4;
    static const long ID_STATICTEXT7;
    static const long ID_COLOURPICKERCTRL5;
    static const long ID_STATICTEXT8;
    static const long ID_COLOURPICKERCTRL6;
    static const long ID_STATICTEXT9;
    static const long ID_COLOURPICKERCTRL7;
    static const long ID_STATICTEXT10;
    static const long ID_COLOURPICKERCTRL8;
    static const long ID_STATICTEXT11;
    static const long ID_COLOURPICKERCTRL9;
    static const long ID_STATICTEXT12;
    static const long ID_COLOURPICKERCTRL10;
    static const long ID_STATICTEXT13;
    static const long ID_COLOURPICKERCTRL11;
    static const long ID_STATICTEXT15;
    static const long ID_COLOURPICKERCTRL12;
    static const long ID_STATICTEXT14;
    static const long ID_BUTTON1;
    static const long ID_PANEL_COLORS;
    static const long ID_PANEL2;
    static const long ID_STATICTEXT16;
    static const long ID_DIRPICKERCTRL4;
    static const long ID_STATICTEXT17;
    static const long ID_DIRPICKERCTRL2;
    static const long ID_STATICTEXT18;
    static const long ID_DIRPICKERCTRL3;
    static const long ID_STATICTEXT19;
    static const long ID_DIRPICKERCTRL5;
    static const long ID_STATICTEXT20;
    static const long ID_DIRPICKERCTRL1;
    static const long ID_PANEL_DIRS;
    static const long ID_STATICTEXT21;
    static const long ID_CHECKBOX12;
    static const long ID_PANEL3;
    static const long ID_STATICTEXT22;
    static const long ID_DIRPICKERCTRL6;
    static const long ID_BUTTON2;
    static const long ID_STATICTEXT23;
    static const long ID_BUTTON3;
    static const long ID_STATICTEXT24;
    static const long ID_PANEL4;
    static const long ID_SPINCTRL1;
    static const long ID_STATICTEXT25;
    static const long ID_PANEL5;
    static const long ID_STATICTEXT27;
    static const long ID_SPINCTRL2;
    static const long ID_STATICTEXT28;
    static const long ID_SPINCTRL3;
    static const long ID_STATICTEXT26;
    static const long ID_PANEL6;
    static const long ID_LISTBOX1;
    static const long ID_STATICTEXT29;
    static const long ID_CHOICE4;
    static const long ID_PANEL7;
    static const long ID_CHECKBOX13;
    static const long ID_CHECKBOX14;
    static const long ID_CHECKBOX15;
    static const long ID_CHECKBOX16;
    static const long ID_RADIOBUTTON1;
    static const long ID_RADIOBUTTON2;
    static const long ID_STATICTEXT30;
    static const long ID_STATICTEXT31;
    static const long ID_PANEL10;
    static const long ID_PANEL_CROSSHAIR;
    static const long ID_PANEL8;
    static const long ID_STATICTEXT32;
    static const long ID_CHECKBOX17;
    static const long ID_STATICTEXT33;
    static const long ID_SPINCTRL4;
    static const long ID_STATICTEXT34;
    static const long ID_PANEL_AUTOSAVE;
    static const long ID_PANEL9;
    static const long ID_NOTEBOOK1;
    //*)

private:
	void GetColorScheme(ColorScheme &c);
	void SetColorScheme(const ColorScheme c);

	void UpdateColorPanel(wxUpdateUIEvent& event);
	void UpdateDirsPanel(wxUpdateUIEvent& event);
	void UpdateCrosshairPanel(wxUpdateUIEvent& event);
	void UpdateAutosavePanel(wxUpdateUIEvent& event);
    //(*Handlers(SettingsDialog)
    void OnColorSchemeChoiseSelect(wxCommandEvent& event);
    void OnResetColorClick(wxCommandEvent& event);
    void OnColorSchemeChanged(wxColourPickerEvent& event);
    void OnSameDirClick(wxCommandEvent& event);
    //*)

    DECLARE_EVENT_TABLE()
};

#endif
