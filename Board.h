#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>

using namespace std;

enum class BoardType {
    Empty,Rectangle,Round
};

struct PadSize{
	float radius1;
	float radius2; //smaller then radius1
};

struct SMDSize{
	float width;
	float height; //bigger then width
};
class Board {
public:
    Board();

    uint32_t width;
    uint32_t height;

    uint32_t border;

    char name[30];
    float grid_size;

    BoardType type;
    bool isValid();
};

#endif
