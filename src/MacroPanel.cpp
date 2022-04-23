#include "MacroPanel.h"
#include <dirent.h>
#include <wx/sizer.h>

extern "C" {
#include "xpm/macropanel/save.xpm"
#include "xpm/macropanel/delete.xpm"
#include "xpm/macropanel/rotate.xpm"
#include "xpm/macropanel/metallization.xpm"
#include "xpm/macropanel/top.xpm"
#include "xpm/macropanel/bottom.xpm"
}

enum {
	ID_MACRO_SAVE = 100,
	ID_MACRO_DELETE,
	ID_MACRO_ROTATE,
	ID_MACRO_METALLIZATION,
	ID_MACRO_SIDE
};

wxBEGIN_EVENT_TABLE(MacroPanel, wxSplitterWindow)
	EVT_MENU(ID_MACRO_SAVE, MacroPanel::Save)
	EVT_MENU(ID_MACRO_DELETE, MacroPanel::Delete)
	EVT_MENU(ID_MACRO_ROTATE, MacroPanel::Rotate)
	EVT_MENU(ID_MACRO_METALLIZATION, MacroPanel::ToggleMetallization)
	EVT_MENU(ID_MACRO_SIDE, MacroPanel::ToggleSide)
wxEND_EVENT_TABLE()

MacroPanel::MacroPanel(wxWindow *parent)
		: wxSplitterWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(200, -1), wxSP_LIVE_UPDATE | wxBORDER_SUNKEN) {
	tree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT | wxTR_DEFAULT_STYLE);
	wxTreeItemId root = tree->AddRoot("Root");
	ScanDir(root, "macros");
	wxPanel *panel = new wxPanel(this);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	{
		component = new wxCheckBox(panel, wxID_ANY, _("Add as component"));
		component->SetValue(true);
		sizer->Add(component, 0, wxALL, 5);
	}
	{
		buttons = new wxToolBar(panel, wxID_ANY,
			wxDefaultPosition, wxDefaultSize, wxTB_HORZ_TEXT | wxTB_HORIZONTAL | wxTB_FLAT);

		buttons->AddTool(ID_MACRO_SAVE, wxEmptyString, save_xpm, _("Save as macro"));
		buttons->AddTool(ID_MACRO_DELETE, wxEmptyString, delete_xpm, _("Delete macro"));
		buttons->AddStretchableSpace();
		buttons->AddTool(ID_MACRO_ROTATE, wxEmptyString, rotate_xpm, _("Rotate macro"));
		buttons->AddCheckTool(ID_MACRO_METALLIZATION, wxEmptyString, metallization_xpm, wxNullBitmap, _("Through-Pads (vias)"));
		buttons->AddTool(ID_MACRO_SIDE, _("TOP"), top_xpm, _("Toggle board-side"));

		sizer->Add(buttons, 0, wxEXPAND);
	}
	{
		preview = new MacroPreview(panel);
		sizer->Add(preview, 1, wxEXPAND);
	}
	panel->SetSizer(sizer);
	SetAutoLayout(true);
	SplitHorizontally(tree, panel);
	SetSashGravity(0.5);
	side = true;
	metallization = false;
	rotate = 0;
}

void MacroPanel::Save(wxCommandEvent&) {
}

void MacroPanel::Delete(wxCommandEvent&) {
}

void MacroPanel::Rotate(wxCommandEvent&) {
	rotate++;
	if(rotate >= 4)
		rotate = 0;
}

void MacroPanel::ToggleMetallization(wxCommandEvent&) {
	metallization = !metallization;
}

void MacroPanel::ToggleSide(wxCommandEvent &e) {
	side = !side;
	buttons->DeleteTool(ID_MACRO_SIDE);
	buttons->AddTool(ID_MACRO_SIDE, side ? _("TOP") : _("BOT"), side ? top_xpm : bottom_xpm, _("Toggle board-side"));
}

void MacroPanel::ScanDir(wxTreeItemId parent, const char *dirPath) {
	struct dirent *de;
	DIR *dir = opendir(dirPath);

	while(de = readdir(dir)){
		if(*de->d_name == '.')
			continue;

		char *str = de->d_name;
		while(*str){
			if(*str == '.')
				*str = '\0';
			else
				str++;
		}

		wxTreeItemId id = tree->AppendItem(parent, de->d_name);
		if(de->d_type == DT_DIR){
			char path[1024];
			snprintf(path, 1024, "%s/%s", dirPath, de->d_name);
			ScanDir(id, path);
		}
	}
}

