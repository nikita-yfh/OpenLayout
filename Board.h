#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>

using namespace std;

enum class BoardType {
    Empty,Rectangle,Round
};

struct PadSize {
    float radius1;
    float radius2; //smaller then radius1
    bool operator==(PadSize other) {
        return other.radius1==radius1 && other.radius2==radius2;
    }
    bool operator<(PadSize other) {
        return !(radius1>=other.radius1 || radius2>=other.radius2);
    }
};

struct SMDSize {
    float width;
    float height; //bigger then width
    SMDSize normal() {
        return {min(width,height),max(width,height)};
    }
    bool operator==(SMDSize other) {
        return other.width==width && other.height==height;
    }
    bool operator<(SMDSize other) {
        return !(width>=other.width || height>=other.height);
    }
};

struct Size {
    float w;
    float h;
};
struct Point{
	float x;
	float y;
};
class Board {
public:
    Board();

    Size size;

    float border;

    float zoom;
    Point camera;

    char name[30];
    float grid_size;

    BoardType type;
    bool isValid();
};

#endif
