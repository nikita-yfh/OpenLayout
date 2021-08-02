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
    OpenLayoutFrame* Frame = new OpenLayoutFrame();
    Frame->Show();
    SetTopWindow(Frame);
    return true;
}
