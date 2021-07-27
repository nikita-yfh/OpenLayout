#include "PCBFile.h"
#include <string.h>
#include <assert.h>

PCBFile::PCBFile(){
	SetDefaults();
}

PCBFile::~PCBFile(){}
void PCBFile::SetDefaults(){
	boards.clear();
	boards.push_back(Board());
	strcpy(info.title,"");
	strcpy(info.author,"");
	strcpy(info.company,"");
	strcpy(info.comment,"");
}
Board &PCBFile::GetSelectedBoard(){
	assert(selected_board<boards.size());
	return boards.at(selected_board);
}
void PCBFile::AddBoard(Board b){
	boards.push_back(b);
	selected_board=GetNumBoards()-1;
}
size_t PCBFile::GetNumBoards() const{
	return boards.size();
}
void PCBFile::SelectBoard(size_t v){
	assert(v<boards.size());
	selected_board=v;
}
