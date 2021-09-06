#pragma once
#include <wx/app.h>
#include "PCBFile.h"
#include "Settings.h"

class OpenLayoutApp : public wxApp {
public:
	virtual bool OnInit();
	PCBFile file;
	Settings settings;
private:
	wxLocale m_Locale;
};

DECLARE_APP(OpenLayoutApp);

#define BOARD wxGetApp().file.GetSelectedBoard()
#define PCB wxGetApp().file
#define SETTINGS wxGetApp().settings
