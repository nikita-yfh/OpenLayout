#include "PCBFile.h"
#include <string.h>
#include <assert.h>
#include <cfloat>

void PCBFile::AddBoard(Board b) {
    boards.push_back(b);
    active_board_tab=GetNumBoards()-1;
}
size_t PCBFile::GetNumBoards() const {
    return boards.size();
}
template<typename T>
void writev(FILE *file,const T &val,size_t size=sizeof(T)) {
    fwrite(&val,size,1,file);
}
template<typename T>
void readv(FILE *file,T &val,size_t size=sizeof(T)) {
    fread(&val,size,1,file);
}
void readstr(FILE *file,size_t size,string &sstr) {
    uint8_t str_l;
    readv(file,str_l);
    char *str=new char[size+1];
    str[size]='\0';
    fread(str,size,1,file);
    sstr=str;
    delete[]str;
}
void readstr(FILE *file,string &sstr) {
    uint32_t length;
    readv(file,length);
    char *str=new char[length+1];
    fread(str,length,1,file);
    str[length]='\0';
    sstr=str;
    delete str;
}
void writestr(FILE *file,size_t size,const string&sstr) {
    writev(file,(uint8_t)sstr.size());
    fwrite(sstr.c_str(),size,1,file);
}
void writestr(FILE *file,const string&sstr) {
    writev(file,(uint32_t)sstr.size());
    fwrite(sstr.c_str(),sstr.size(),1,file);
}
void Component::load(FILE *file) {
    readv(file,offset.x);
    readv(file,offset.y);
    readstr(file,package);
    readstr(file,comment);
    readv(file,use);
    valid = true;
}
void Component::save(FILE *file) {
    writev(file,offset.x);
    writev(file,offset.y);
    writestr(file,package);
    writestr(file,comment);
    writev(file,use);
}

void Object::load(FILE *file,bool text_child) {
    readv(file,type);
    readv(file,pos.x);
    readv(file,pos.y);
    readv(file,size.outer);
    readv(file,size.inner);
    readv(file,line_width);
    readv(file,__pad1);
    readv(file,layer);
    readv(file,tht_shape);
    readv(file,__pad1_1,4);
    readv(file,component_id);
    readv(file,unk1);
    readv(file,th_style,4);
    readv(file,__pad1_2,5);
    readv(file,fill);
    readv(file,ground_distance);
    readv(file,__pad2,5);
    readv(file,thermobarier);
    readv(file,flip_vertical);
    readv(file,cutoff);
    readv(file,thsize);
    readv(file,metalisation);
    readv(file,soldermask);
    readv(file,__pad4,18);
    if(!text_child) {
        readstr(file,text);
        readstr(file,marker);
        uint32_t groups_c;
        readv(file,groups_c);
        groups.resize(groups_c);
        for(uint32_t q=0; q<groups_c; q++)
            readv(file,groups[q]);
    }
    if(type==OBJ_CIRCLE)
        return;
    if(type==OBJ_TEXT) {
        uint32_t count;
        readv(file,count);
        text_objects.resize(count);
        for (uint32_t q = 0; q < count; q++)
            text_objects[q].load(file, true);

        if (tht_shape == 1)
            component.load(file);

        return;
    }

    uint32_t poly_points_c;
    readv(file,poly_points_c);
    poly_points.resize(poly_points_c);
    for(uint32_t q=0; q<poly_points_c; q++) {
        readv(file,poly_points[q].x);
        readv(file,poly_points[q].y);
    }
    selected=false;
}
void Object::save(FILE *file,bool text_child) {
    writev(file,type);
    writev(file,pos.x);
    writev(file,pos.y);
    writev(file,size.outer);
    writev(file,size.inner);
    writev(file,line_width);
    writev(file,__pad1);
    writev(file,layer);
    writev(file,tht_shape);
    writev(file,__pad1_1,4);
    writev(file,component_id);
    writev(file,unk1);
    writev(file,th_style,4);
    writev(file,__pad1_2,5);
    writev(file,fill);
    writev(file,ground_distance);
    writev(file,__pad2,5);
    writev(file,thermobarier);
    writev(file,flip_vertical);
    writev(file,cutoff);
    writev(file,thsize);
    writev(file,metalisation);
    writev(file,soldermask);
    writev(file,__pad4,18);
    if(!text_child) {
        writestr(file,text);
        writestr(file,marker);
        writev(file,(uint32_t)groups.size());
        for(uint32_t q=0; q<groups.size(); q++)
            writev(file,groups[q]);
    }
    if(type==OBJ_CIRCLE)
        return;
    if(type==OBJ_TEXT) {
        writev(file,(uint32_t)text_objects.size());
        for (uint32_t q = 0; q < text_objects.size(); q++)
            text_objects[q].save(file, true);

        if (tht_shape == 1)
            component.save(file);

        return;
    }

    writev(file,(uint32_t)poly_points.size());
    for(uint32_t q=0; q<poly_points.size(); q++) {
        writev(file,poly_points[q].x);
        writev(file,poly_points[q].y);
    }
}


void Board::load(FILE* file) {
    readstr(file,30,name);

    readv(file,__pad0,4);

    readv(file,size.width);
    readv(file,size.height);

    readv(file,ground_pane,7);

    readv(file,active_grid_val);
    readv(file,zoom);

    readv(file,camera.x);
    readv(file,camera.y);

    readv(file,active_layer);

    readv(file,layer_visible,7);

    readv(file,images[0].show);
    readv(file,images[1].show);
    readstr(file,200,images[0].path);
    readstr(file,200,images[1].path);
    readv(file,images[0].dpi);
    readv(file,images[1].dpi);
    readv(file,images[0].pos.x);
    readv(file,images[1].pos.x);
    readv(file,images[0].pos.y);
    readv(file,images[1].pos.y);

    readv(file,__pad1,8);

    readv(file,anchor.x);
    readv(file,anchor.y);

    readv(file,is_multilayer);

    uint32_t obj_count;
    readv(file,obj_count);
    objects.resize(obj_count);

    for(uint32_t q=0; q<obj_count; q++)
        objects[q].load(file);
    for(uint32_t q=0; q<objects.size(); q++) {
        if(objects[q].type==OBJ_THT_PAD ||objects[q].type == OBJ_SMD_PAD)
            objects[q].load_con(file);
    }
}

void Board::save(FILE* file) {
    writestr(file,30,name);

    writev(file,__pad0,4);

    writev(file,size.width);
    writev(file,size.height);
    writev(file,ground_pane,7);

    writev(file,active_grid_val);
    writev(file,zoom);

    writev(file,camera.x);
    writev(file,camera.y);

    writev(file,active_layer);

    writev(file,layer_visible,7);

    writev(file,images[0].show);
    writev(file,images[1].show);
    writestr(file,200,images[0].path);
    writestr(file,200,images[1].path);
    writev(file,images[0].dpi);
    writev(file,images[1].dpi);
    writev(file,images[0].pos.x);
    writev(file,images[1].pos.x);
    writev(file,images[0].pos.y);
    writev(file,images[1].pos.y);

    writev(file,__pad1,8);

    writev(file,anchor.x);
    writev(file,anchor.y);

    writev(file,is_multilayer);

    writev(file,(uint32_t)objects.size());

    for(uint32_t q=0; q<objects.size(); q++)
        objects[q].save(file);
    for(uint32_t q=0; q<objects.size(); q++) {
        if(objects[q].type==OBJ_THT_PAD ||objects[q].type == OBJ_SMD_PAD)
            objects[q].save_con(file);
    }
}
void Object::load_con(FILE *file) {
    uint32_t count;
    readv(file,count);
    connections.resize(count);
    for(uint32_t q=0; q<count; q++)
        readv(file,connections[q]);
}
void Object::save_con(FILE *file) {
    writev(file,(uint32_t)connections.size());
    for(uint32_t q=0; q<connections.size(); q++)
        writev(file,connections[q]);
}
bool Object::is_copper() const {
    return layer==LAY_C1 || layer==LAY_C2 || layer==LAY_I1 || layer==LAY_I2;
}
bool Object::can_connect() const {
    return type==OBJ_THT_PAD || type==OBJ_SMD_PAD;
}
uint8_t Object::get_begin_style() const {
    return line_ending%2;
}
uint8_t Object::get_end_style() const {
    return line_ending>1;
}
float Object::get_angle() const {
    if(type==OBJ_THT_PAD && tht_shape%3==1) {
        if(poly_points.size()==2) {
            Vec2 v=pos-poly_points[1];
            return deg(get_angle_v(v));
        }
    } else if(type==OBJ_THT_PAD && tht_shape%3==2) {
        if(poly_points.size()==8) {
            Vec2 v=pos-(poly_points[3]+poly_points[4])/2.0f;
            return deg(get_angle_v(v));
        }
    } else if((type==OBJ_THT_PAD && tht_shape%3==0) || type==OBJ_SMD_PAD) {
        if(poly_points.size()==4) {
            Vec2 v=pos-(poly_points[1]+poly_points[2])/2.0f;
            return deg(get_angle_v(v));
        }
    }
    return 0.0f;
}
/*void Object::set_pad_shape(uint8_t shape,float angle){
	poly_points.clear();
	if(tht_shape%3==1){//circle shape
		float
		poly_points.emplace_back()
	}
}*/
void Object::rotate(float angle) {
    if(type==OBJ_THT_PAD) {
        for(Vec2 &v : poly_points) {
            rotate_v(pos,v,angle);
        }
    }
}
bool Object::point_in(Vec2 point) const {
    if(type==OBJ_THT_PAD && tht_shape%3==1) {
        return (point-pos).Length()<=size.outer;
    } else if((type==OBJ_THT_PAD && tht_shape%3!=1) || type==OBJ_SMD_PAD) {
        bool c = false;
        for (int i = 0, j = poly_points.size() - 1; i < poly_points.size(); j = i++) {
            if ((((poly_points[i].y <= point.y) && (point.y < poly_points[j].y)) ||
				((poly_points[j].y <= point.y) && (point.y < poly_points[i].y))) &&
				(((poly_points[j].y - poly_points[i].y) != 0) &&
				(point.x > ((poly_points[j].x - poly_points[i].x) * (point.y - poly_points[i].y) /
				(poly_points[j].y - poly_points[i].y) + poly_points[i].x)))) c = !c;
        }
        return c;
    }
}
bool Object::rect_in(Rect4 r) const {
    if(type==OBJ_THT_PAD && tht_shape%3==1) {
		return intersect_circle_rect(r,pos,size.outer);
        //return (point-pos).Length()<=size.outer;
    }
}
Vec2 Object::get_position() const{
	if(type==OBJ_THT_PAD||type==OBJ_CIRCLE||type==OBJ_SMD_PAD)
		return pos;
	else if(type==OBJ_POLY){
		Vec2 minv(FLT_MAX,FLT_MAX);
		Vec2 maxv(-FLT_MAX,-FLT_MAX);
		for(const Vec2 &v : poly_points){
			minv.x=min(minv.x,v.x);
			minv.y=min(minv.y,v.y);
			maxv.x=max(maxv.x,v.x);
			maxv.y=max(maxv.y,v.y);
		}
		Vec2 pos=(minv+maxv)/2.0f;
		return pos;
	}
}
void Object::move(Vec2 d){
	if(type==OBJ_THT_PAD||type==OBJ_CIRCLE||type==OBJ_SMD_PAD)
		pos+=d;
	for(Vec2 &v : poly_points)
		v+=d;
}
void Object::set_position(Vec2 p){
	if(type==OBJ_THT_PAD||type==OBJ_CIRCLE||type==OBJ_SMD_PAD)
		pos=p;
	else if(type==OBJ_POLY||type==OBJ_LINE)
		move(p-get_position());
}
Object &Board::first_selected(){
	for(Object &o : objects)
		if(o.selected)
			return o;
}
const Object &Board::first_selected() const{
	for(const Object &o : objects)
		if(o.selected)
			return o;
}
Vec2 Board::to_grid(Vec2 v,bool shift,bool ctrl){
	if(ctrl)return v;
	double grid=active_grid_val;
	if(shift)grid/=2.0;
	return Vec2(std::round(v.x/grid)*grid,
				std::round(v.y/grid)*grid);
}
void Board::select(Object &o1){
	o1.selected=true;
	for(Object &o2 : objects)
		for(uint32_t group2 : o2.groups)
			for(uint32_t group1 : o1.groups)
				if(group1==group2)
					o2.selected=true;
}
void Board::select(Rect4 rect){
	rect.Normalize();
	for(Object &o : objects)
		if(o.rect_in(rect))
			select(o);
}
void ProjectInfo::load(FILE *file) {
    readstr(file,100,title);
    readstr(file,100,author);
    readstr(file,100,company);
    readstr(file,comment);
}
void ProjectInfo::save(FILE *file) {
    writestr(file,100,title);
    writestr(file,100,author);
    writestr(file,100,company);
    writestr(file,comment);
}

int PCBFile::load(const char *filename) {
    FILE *file=fopen(filename,"rb");
    if(!file)
        return 1;

    uint32_t magic_r;
    readv(file,magic_r);
    assert(magic_r==magic);

    uint32_t boards_count;
    readv(file,boards_count);
    assert(boards_count!=0);
    boards.resize(boards_count);

    for(uint32_t q=0; q<boards_count; q++)
        boards[q].load(file);

    readv(file,active_board_tab);
    info.load(file);
    fclose(file);
    return 0;
}
int PCBFile::save(const char *filename) {
    FILE *file=fopen(filename,"wb");
    if(!file)
        return 1;

    writev(file,magic);

    writev(file,(uint32_t)boards.size());

    for(uint32_t q=0; q<boards.size(); q++)
        boards[q].save(file);

    writev(file,active_board_tab);
    info.save(file);
    fclose(file);
    return 0;
}
Board &PCBFile::GetSelectedBoard() {
    assert(active_board_tab<boards.size());
    return boards[active_board_tab];
}

PCBFile file;
