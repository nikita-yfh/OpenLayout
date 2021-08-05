#include "Board.h"
#include <string.h>

Board::Board() {
    grid_size=1.27f;
    type=BoardType::Empty;
    border=20.0f;
    size={160.0f,100.0f};

    zoom=1.0f;

    strcpy(name,"New board");
}
bool Board::isValid() { //check size
    if(size.w>=1.0f && size.h>=1.0f && size.w<=500.0f && size.h<=500.0f) {
        if(type!=BoardType::Empty)
            return size.w+border*2<=500.0f && size.h+border*2<=500.0f;
        return true;
    }
    return false;
}
