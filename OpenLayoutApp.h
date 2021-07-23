/***************************************************************
 * Name:      OpenLayoutApp.h
 * Purpose:   Defines Application Class
 * Author:    Nikita-yfh (nikita.yfh@gmail.com)
 * Created:   2021-07-21
 * Copyright: Nikita-yfh (https://github.com/nikita-yfh)
 * License:   MIT
 **************************************************************/

#ifndef OPENLAYOUTAPP_H
#define OPENLAYOUTAPP_H

#include <wx/app.h>

class OpenLayoutApp : public wxApp {
public:
    virtual bool OnInit();
private:
    wxLocale m_Locale;
};

#endif // OPENLAYOUTAPP_H
