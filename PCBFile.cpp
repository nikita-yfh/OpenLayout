#include "PCBFile.h"
#include <string.h>
#include <assert.h>

void PCBFile::AddBoard(Board b) {
    boards.push_back(b);
    active_board_tab=GetNumBoards()-1;
}
size_t PCBFile::GetNumBoards() const {
    return boards.size();
}
template<typename T>
void writev(FILE *file,T *val,size_t size=sizeof(T)) {
    fwrite(val,size,1,file);
}
template<typename T>
void readv(FILE *file,T *val,size_t size=sizeof(T)) {
    fread(val,size,1,file);
}
void readstr(FILE *file,size_t size,string &sstr) {
    uint8_t str_l;
    readv(file,&str_l);
    char *str=new char[size+1];
    str[size]='\0';
    fread(str,size,1,file);
    sstr=str;
    delete[]str;
}
void readstr(FILE *file,string &sstr) {
    uint32_t length;
    readv(file,&length);
    char *str=new char[length+1];
    fread(str,length,1,file);
    sstr=str;
    delete str;
}

void Component::load(FILE *file) {
    uint32_t len;
    readv(file,&offset.x);
    readv(file,&offset.y);
    readstr(file,package);
    readstr(file,comment);
    readv(file,&use);
    valid = true;
}

void Object::load(FILE *file,bool text_child) {
    readv(file,&type);
    readv(file,&pos.x);
    readv(file,&pos.y);
    readv(file,&size.outer);
    readv(file,&size.inner);
    readv(file,&line_width);
    readv(file,&__pad1);
    readv(file,&layer);
    readv(file,&tht_shape);
    readv(file,__pad1_1,4);
    readv(file,&component_id);
    readv(file,&unk1);
    readv(file,th_style,4);
    readv(file,__pad1_2,5);
    readv(file,&th_style_custom);
    readv(file,__pad2,9);
    readv(file,&thermobarier);
    readv(file,&flip_vertical);
    readv(file,&cutoff);
    readv(file,&thsize);
    readv(file,&metalisation);
    readv(file,&soldermask);
    readv(file,__pad4,18);
    if(!text_child) {
        readstr(file,text);
        readstr(file,marker);
        uint32_t groups_c;
        readv(file,&groups_c);
        groups.resize(groups_c);
        for(int q=0; q<groups_c; q++)
            readv(file,&groups[q]);
    }
    if(type==OBJ_CIRCLE)
        return;
    if(type==OBJ_TEXT) {
        uint32_t count;
        readv(file,&count);
        text_objects.resize(count);
        for (int q = 0; q < count; q++)
            text_objects[q].load(file, true);

        if (tht_shape == 1)
            component.load(file);

        return;
    }

    uint32_t poly_points_c;
    readv(file,&poly_points_c);
    poly_points.resize(poly_points_c);
    for(int q=0; q<poly_points_c; q++) {
        readv(file,&poly_points[q].x);
        readv(file,&poly_points[q].y);
    }
}

void Board::load(FILE* file) {
    readstr(file,30,name);

    fread(&__pad0,1,4,file);

    readv(file,&size.width);
    readv(file,&size.height);

    fread(&ground_pane,1,7,file);

    readv(file,&active_grid_val);
    readv(file,&zoom);

    readv(file,&camera.x);
    readv(file,&camera.y);

    readv(file,&active_layer);

    readv(file,&layer_visible,7);

    readv(file,&images[0].show);
    readv(file,&images[1].show);
    readstr(file,200,images[0].path);
    readstr(file,200,images[1].path);
    readv(file,&images[0].dpi);
    readv(file,&images[1].dpi);
    readv(file,&images[0].pos.x);
    readv(file,&images[1].pos.x);
    readv(file,&images[0].pos.y);
    readv(file,&images[1].pos.y);

    readv(file,&__pad1,8);

    readv(file,&anchor.x);
    readv(file,&anchor.y);

    readv(file,&is_multilayer);

    uint32_t obj_count;
    readv(file,&obj_count);
    objects.resize(obj_count);

    vector<int32_t>con;

    for(int q=0; q<obj_count; q++){
        objects[q].load(file);
        if(objects[q].type==OBJ_THT_PAD ||objects[q].type == OBJ_SMD_PAD)
			con.push_back(q);
    }
    for(int q=0;q<con.size();q++){
		Connections c;
		c.load(file);
    }


}
void Connections::load(FILE *file) {
    uint32_t count;
    readv(file,&count);
    connections.resize(count);
    for(int q=0; q<count; q++)
        readv(file,&connections[q]);
}
void ProjectInfo::load(FILE *file) {
    readstr(file,100,title);
    readstr(file,100,author);
    readstr(file,100,company);
    readstr(file,comment);
}

int PCBFile::load(const char *filename) {
    FILE *file=fopen(filename,"rb");
    if(!file)
        return 1;

    uint32_t magic_r;
    readv(file,&magic_r);
    assert(magic_r==magic);

    uint32_t boards_count;
    readv(file,&boards_count);
    assert(boards_count!=0);
    boards.resize(boards_count);

    for(int q=0; q<boards_count; q++)
        boards[q].load(file);

    readv(file,&active_board_tab);
    info.load(file);
    fclose(file);
    return 0;
}
Board &PCBFile::GetSelectedBoard() {
    assert(active_board_tab<boards.size());
    return boards[active_board_tab];
}
