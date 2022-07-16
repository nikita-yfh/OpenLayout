#pragma once
#include <wx/glcanvas.h>
#include "Board.h"
#include "Settings.h"

class SimpleCanvas : public wxGLCanvas {
public:
	SimpleCanvas(wxWindow *parent, ObjectGroup *objects, const Settings &settings);
private:
	ObjectGroup *objects;
	const Settings &settings;

	void Draw(wxPaintEvent&);

	wxDECLARE_EVENT_TABLE();
};
