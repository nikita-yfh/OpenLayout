#pragma once
#include "File.h"
#include <string.h>

class ProjectInfo {
public:
	ProjectInfo();

	virtual void Save(File &file) const;
	virtual void Load(File &file);

	const char *GetTitle() const   { return title; }
	const char *GetAuthor() const  { return author; }
	const char *GetCompany() const { return company; }
	const char *GetComment() const { return comment; }

	void SetTitle(const char *s)   { strncpy(title, s, 100);    title[100] = '\0'; }
	void SetAuthor(const char *s)  { strncpy(author, s, 100);   author[100] = '\0'; }
	void SetCompany(const char *s) { strncpy(company, s, 100);  company[100] = '\0'; }
	void SetComment(const char *s) { strncpy(comment, s, 4095); comment[4095] = '\0'; }

	//void ShowDialog(wxWindow *parent);
private:
	char title[101];
	char author[101];
	char company[101];
	char comment[4096];
};
