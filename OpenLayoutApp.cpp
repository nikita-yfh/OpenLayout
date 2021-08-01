/***************************************************************
 * Name:      OpenLayoutApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:
 **************************************************************/

#include "OpenLayoutApp.h"

//(*AppHeaders
#include "OpenLayoutMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(OpenLayoutApp);
bool OpenLayoutApp::OnInit() {
    //m_Locale.Init(wxLANGUAGE_RUSSIAN);
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK ) {
        OpenLayoutFrame* Frame = new OpenLayoutFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)
    return wxsOK;
}
