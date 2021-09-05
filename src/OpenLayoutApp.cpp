/***************************************************************
 * Name:      OpenLayoutApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:
 **************************************************************/

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
