#ifndef PROJECTINFO_H
#define PROJECTINFO_H
#include "PCBFile.h"

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

class ProjectInfoDialog: public wxDialog {
public:
    void Get(ProjectInfo &info);
    ProjectInfoDialog(wxWindow* parent,const ProjectInfo &info);

    wxTextCtrl* TextAuthor;
    wxTextCtrl* TextComment;
    wxTextCtrl* TextCompany;
    wxTextCtrl* TextTitle;
};

#endif
