#include "MenuBar.h"
#include "OpenLayoutMain.h"

MenuBar::MenuBar(){
	{
		//File
		wxMenu* menu= new wxMenu();
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
		Append(menu, _("&File"));
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
		Append(menu, _("&Edit"));
	}
	{
		//Board
		wxMenu *menu = new wxMenu();
		menu->Append(ID_BOARD_NEW, _("Add new board"));
		menu->Append(ID_BOARD_PROPERTIES, _("Properties"));
		menu->Append(ID_BOARD_COPY, _("Copy board"));
		menu->Append(ID_BOARD_DELETE, _("Delete board"));
		menu->AppendSeparator();
		menu->Append(ID_BOARD_SET_RIGHT, _("Set board to &right"));
		menu->Append(ID_BOARD_SET_LEFT, _("Set board to &left"));
		menu->AppendSeparator();
		menu->Append(ID_BOARD_MOVE_RIGHT, _("&Move board to right"));
		menu->Append(ID_BOARD_MOVE_LEFT, _("M&ove board to left"));
		menu->Append(ID_BOARD_IMPORT, _("&Import boards from "));
		menu->Append(ID_BOARD_SAVE, _("&Save boards to "));
		Append(menu, _("&Board"));
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
		Append(menu, _("F&unctions"));
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
		menu->AppendSeparator();
		menu->Append(ID_DEFINE_PLUGIN, _("Def&ine plugin"));
		menu->Append(ID_RUN_PLUGIN, _("R&un plugin"));
		Append(menu, _("E&xtras"));
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
		menu->Append(ID_PANEL_SELECTOR, _("S&elector-Panel"));
		menu->AppendSeparator();
		menu->Append(ID_ZOOM_BOARD, _("Zoom &board"));
		menu->Append(ID_ZOOM_OBJECTS, _("Zoom &objects"));
		menu->Append(ID_ZOOM_SELECTION, _("Zoom &selection"));
		menu->Append(ID_ZOOM_PREVIOUS, _("&Zoom previous"));
		Append(menu, _("&Options"));
	}
	{
		//Help
		wxMenu *menu = new wxMenu();
		menu->Append(wxID_ABOUT, _("&About"));
		Append(menu, _("&Help"));
	}
}
