#pragma once

#include <stdint.h>
#include <vector>
#include <string>

using namespace std;


enum {
    LAY_C1,
    LAY_S1,
    LAY_C2,
    LAY_S2,
    LAY_I1,
    LAY_I2,
    LAY_O,
};

template<typename T>
struct Pair {
    union {
        T width;
        T inner;
        T x;
    };
    union {
        T height;
        T outer;
        T y;
    };
    bool operator==(Pair<T>other) {
        return x==other.x && y==other.y;
    }
    bool operator<(Pair<T>other) {
        return x<other.x || y<other.y;
    }
};

struct ImageConfig {
    string path;
    uint32_t dpi;
    uint8_t show;
    Pair<int32_t>pos;
};



enum {
    OBJ_THT_PAD = 2,
    OBJ_POLY = 4,
    OBJ_CIRCLE = 5,
    OBJ_LINE = 6,
    OBJ_TEXT = 7,
    OBJ_SMD_PAD = 8,
};

enum {
    THT_SHAPE_CIRCLE = 1,
    THT_SHAPE_OCT = 2,
    THT_SHAPE_SQUARE = 3,
};

enum {
    TH_STYLE_C1,
    TH_STYLE_C2,
    TH_STYLE_I1,
    TH_STYLE_I2,
};

struct Component {
    Pair<float>offset;
    uint8_t center_mode;
    double rotation;
    bool valid=false;
    string package;
    string comment;
    uint8_t use;

    void load(FILE *f);
    void save(FILE *f);
};



struct Object {
    uint8_t type;
    Pair<float>pos;
    Pair<float>size;

    union {
        uint32_t line_width;
        uint32_t end_angle;
    };
    uint8_t __pad1;
    uint8_t layer;
    uint8_t tht_shape;

    uint8_t __pad1_1[4];
    uint16_t component_id;
    uint8_t unk1;

    union {
        uint8_t th_style[4];
        uint32_t start_angle;
    };

    uint8_t __pad1_2[5];
    uint8_t th_style_custom; // also fill

    uint8_t __pad2[9];
    uint8_t thermobarier;
    uint8_t flip_vertical;
    uint8_t cutoff;
    uint32_t thsize; // rotation
    uint8_t metalisation;
    uint8_t soldermask;

    uint8_t __pad4[18];

    string text;
    string marker;
    vector<int32_t> groups;
    vector<int32_t>connections;
    vector<Pair<float>> poly_points;
    vector<Object> text_objects;
    Component component;

    void load(FILE *f,bool text_child=false);
    void save(FILE *f,bool text_child=false);

    void load_con(FILE *f);
    void save_con(FILE *f);
};


struct Board {
    string name;

    uint8_t __pad0[4];

    Pair<uint32_t>size;

    uint8_t ground_pane[7];
    double active_grid_val;

    double zoom;
    Pair<uint32_t> camera;

    uint32_t active_layer;

    uint8_t layer_visible[7];

    ImageConfig images[2];

    uint8_t __pad1[8];

    Pair<int32_t>anchor;

    uint8_t is_multilayer;

    vector<Object>objects;

    void load(FILE *f);
    void save(FILE *f);
};


struct ProjectInfo {
    string title;
    string author;
    string company;
    string comment;
    void load(FILE *f);
    void save(FILE *f);
};

struct PCBFile {
    const uint32_t magic=0xFFAA3306;
    vector<Board>boards;
    uint32_t active_board_tab;
    ProjectInfo info;

    void AddBoard(Board b);
    size_t GetNumBoards() const;
    Board &GetSelectedBoard();

    int load(const char *filename);
    int save(const char *filename);
};
