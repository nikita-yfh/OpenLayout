#pragma once
#include <wx/app.h>

class OpenLayoutApp : public wxApp {
public:
	virtual bool OnInit();
private:
	wxLocale m_Locale;
};

DECLARE_APP(OpenLayoutApp);
