#pragma once
#include <wx/app.h>
#include "PCBFile.h"
#include "Settings.h"

class OpenLayoutApp : public wxApp {
public:
	virtual bool OnInit();
	PCBFile file;
	Settings settings;

	uint8_t selected_tool;

	Vec2 smd_size{1.6f,0.9f};
	Vec2 pad_size{1.8f,0.6f};
	float track_size=0.8f;
	Vec2 mouse_board_pos;
private:
	wxLocale m_Locale;
};

DECLARE_APP(OpenLayoutApp);

#define BOARD		(wxGetApp().file.GetSelectedBoard())
#define PCB			(wxGetApp().file)
#define SETTINGS	(wxGetApp().settings)
#define APP			(wxGetApp())
