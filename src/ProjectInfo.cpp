#include "ProjectInfo.h"

ProjectInfo::ProjectInfo(){
	title[0] = '\0';
	author[0] = '\0';
	company[0] = '\0';
	comment[0] = '\0';
}
void ProjectInfo::Save(File &file) const{
	file.WriteString(title, 100);
	file.WriteString(author, 100);
	file.WriteString(company, 100);
	file.WriteString(comment);
}
void ProjectInfo::Load(File &file){
	file.ReadString(title, 100);
	file.ReadString(author, 100);
	file.ReadString(company, 100);
	file.ReadString(comment);
}

