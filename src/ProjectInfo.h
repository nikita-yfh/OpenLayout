#pragma once
#include <wx/window.h>
#include "File.h"

class ProjectInfo {
public:
	ProjectInfo();

	virtual void Save(File &file) const;
	virtual void Load(File &file);

	void ShowDialog(wxWindow *parent);
private:
	char title[100];
	char author[100];
	char company[100];
	char comment[4096];
};
