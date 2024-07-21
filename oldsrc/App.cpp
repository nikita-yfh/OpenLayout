#include "App.h"
#include "OpenLayoutMain.h"
IMPLEMENT_APP(OpenLayoutApp);

bool OpenLayoutApp::OnInit() {
	OpenLayoutFrame *frame = new OpenLayoutFrame();
	frame->Show();
	return true;
}
