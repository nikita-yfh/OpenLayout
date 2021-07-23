#include "Board.h"
#include <string.h>

Board::Board(){
	grid_size=1.27f;
	type=BoardType::Empty;
	border=200000;
	width=160000;
	height=100000;
	strcpy(name,"New board");
}
bool Board::isValid(){ //check size
	if(width>=10000 && height>=10000 && width<=5000000 && height<=5000000){
		if(type!=BoardType::Empty)
			return width+border*2<=5000000 && height+border*2<=5000000;
		return true;
	}
	return false;
}
