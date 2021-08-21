#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include "Utils.h"

using namespace std;


enum {
    LAY_C1=1,
    LAY_S1,
    LAY_C2,
    LAY_S2,
    LAY_I1,
    LAY_I2,
    LAY_O,
};

enum{
	END_ROUND,
	END_SQUARE
};


struct ImageConfig {
    string path;
    uint32_t dpi;
    uint8_t show;
    Vec2i pos;
    void set_default();
};

enum{
	POS_LEFT_UP,
	POS_LEFT_BOTTOM,
	POS_RIGHT_UP,
	POS_RIGHT_BOTTOM
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
    Vec2 offset;
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
	Object();
    uint8_t type;
    Vec2 pos;
    Vec2 size;

    union {
        uint32_t line_width;
        uint32_t end_angle;
    };
    uint8_t __pad1;
    uint8_t layer;
    union{
		uint8_t tht_shape;
		uint8_t line_ending;
		/*
			Line ending styles:
			0: O####O
			1: #####O
			2: O#####
			3: ######
		*/
    };
    uint8_t __pad1_1[4];
    uint16_t component_id;
    uint8_t unk1;

    union {
        uint8_t th_style[4];
        uint32_t start_angle;
    };

    uint8_t __pad1_2[5];
    uint8_t fill;


    uint32_t ground_distance;
    uint8_t __pad2[5];
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
    vector<Vec2> poly_points;
    vector<Object> text_objects;
    Component component;

    void load(FILE *f,bool text_child=false);
    void save(FILE *f,bool text_child=false);
    void load_con(FILE *f);//load connections
    void save_con(FILE *f);//save connections

    bool is_copper() const;
    bool can_connect() const;
    uint8_t get_begin_style() const; //return style for line begin
    uint8_t get_end_style() const; //return style for line end
    float get_angle() const;
    void rotate(float angle);
    bool point_in(Vec2) const;
    bool rect_in(Rect4) const;

    bool selected;

    Vec2 get_position() const;
    void set_position(Vec2);
    void move(Vec2);

};


struct Board {
    string name;
    uint8_t __pad0[4];
    Vec2i size;
    uint8_t ground_pane[7];
    double active_grid_val;
    double zoom;
    Vec2i camera;
    uint32_t active_layer;
    uint8_t layer_visible[7];
    ImageConfig images[2];
    uint8_t __pad1[8];
    Vec2i anchor;
    uint8_t is_multilayer;
    vector<Object>objects;

    void load(FILE *f);
    void save(FILE *f);

    Object &first_selected();
    const Object &first_selected()const;

    Vec2 to_grid(Vec2 v,bool shift,bool ctrl);
    void select(Object &o);
    void select(Rect4 r);
    bool is_selected()const;
    size_t get_selected_count()const;
    bool can_group()const;
    bool can_ungroup()const;
    void group();
    void ungroup();
    int32_t get_free_group() const;
    void set_default();
    void build_empty(Vec2i wsize,string name,Vec2i size);
    void build_rectangle(Vec2i wsize,string name,Vec2i size,int distance);
    void build_round(Vec2i wsize,string name,int diameter,int distance);
    void fit(Vec2i wsize);
    void set_anchor(uint8_t pos,int border=0);
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

    void set_default();
};

extern PCBFile file;
