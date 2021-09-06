#include "OpenLayoutApp.h"
#include "OpenLayoutMain.h"
IMPLEMENT_APP(OpenLayoutApp);

bool OpenLayoutApp::OnInit() {
	OpenLayoutFrame* frame;
	if(argc>1)
		frame=new OpenLayoutFrame(argv[1]);
	else
		frame=new OpenLayoutFrame();
	frame->Show();
	SetTopWindow(frame);
	return true;
}
