#pragma once
#include "File.h"

class ProjectInfo {
public:
	ProjectInfo();

	virtual void Save(File &file) const;
	virtual void Load(File &file);

	//void ShowDialog(wxWindow *parent);
private:
	char title[101];
	char author[101];
	char company[101];
	char comment[4096];
};
