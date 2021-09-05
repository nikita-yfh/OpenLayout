#include "PCBFile.h"
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <cfloat>
#include "Settings.h"

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
void ImageConfig::set_default(){
	path="";
	dpi=600;
	show=false;
	pos=Vec2i(0,0);
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

Object::Object(){
	type=0;
	pos=Vec2(0,0);
	size=Vec2(0,0);
	line_width=0;
	layer=0;
	tht_shape=0;
	component_id=0;
	start_angle=0;
	fill=0;
	ground_distance=0;
	thermobarier=0;
	flip_vertical=0;
	cutoff=0;
	thsize=0;
	metalisation=0;
	soldermask=0;
	selected=false;
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
float Object::get_radius()const{
	return (size.inner+size.outer)/2.0f;
}
void Object::get_ending_circles(Vec2 *c)const{
	float angles[2]{start_angle/1000.0f,end_angle/1000.0f};
	float radius=get_radius();
	for(int q=0;q<2;q++){
		c[q].x=pos.x+cosr(angles[q])*radius;
		c[q].y=pos.y+sinr(angles[q])*radius;
	}
}
bool Object::point_in(Vec2 point) const {
    if(type==OBJ_THT_PAD && tht_shape==1)
        return point_in_circle(point,pos,size.outer);
	else if(type==OBJ_THT_PAD && (tht_shape==4 || tht_shape==7)) {
		for(int q=0;q<poly_points.size();q++)
			if(point_in_circle(point,poly_points[q],size.outer))
				return true;
		Vec2 d=(poly_points[1]-poly_points[0]).Skew();
		d.Normalize(size.outer);
		const Vec2 points[]={
			poly_points[0]-d,
			poly_points[1]-d,
			poly_points[1]+d,
			poly_points[0]+d
		};
		if(point_in_poly(points,4,point))
			return true;
    } else if((type==OBJ_THT_PAD && tht_shape%3!=1) || type==OBJ_SMD_PAD) {
		if(point_in_poly(poly_points.data(),poly_points.size(),point))
			return true;
    }else if(type==OBJ_LINE|| type==OBJ_POLY){
    	if(type==OBJ_POLY)
			if(point_in_poly_c(poly_points.data(),poly_points.size(),point))
				return true;
		if(line_width>0){
			for(int q=0;q<poly_points.size()-1;q++){
				const Vec2&p1=poly_points[q];
				const Vec2&p2=poly_points[(q+1)%poly_points.size()];

				Vec2 c=(p2-p1);
				c.Normalize(line_width/2.0f);
				Vec2 d=c.Skew();
				Vec2 c1(0.0f,0.0f);
				Vec2 c2(0.0f,0.0f);
				if(get_begin_style()==END_SQUARE)c1=c;
				if(get_end_style()==END_SQUARE)c2=c;
				Vec2 points[]={
					p1-d-c1,
					p1+d-c1,
					p2+d+c2,
					p2-d+c2
				};
				if(point_in_poly(points,4,point))
					return true;
				if(point_in_circle(point,p1,line_width/2.0f))
					return true;
			}
		}
	}else if(type==OBJ_CIRCLE) {
		Vec2 circles[2]; get_ending_circles(circles);
		float _line_width=size.outer-size.inner;
		for(int q=0;q<2;q++)
			if(point_in_circle(point,circles[q],_line_width/2.0f))
				return true;

		if(!point_in_circle(point,pos,size.outer))
			return false;
		if(fill){
			float arc_center_a=(start_angle+delta_angle(start_angle,end_angle)/2.0f)/1000.0f; //center between start and end
			Vec2 arc_center=Vec2(cosr(arc_center_a),sinr(arc_center_a))*size.outer;
			Vec2 d=arc_center; d.Normalize(_line_width/2.0f);

			if(!points_one_side(circles[0]-d,circles[1]-d,point,pos+arc_center))
				return false;
        }else{
			if(point_in_circle(point,pos,size.inner))
				return false;
			float angle=deg(get_angle_v(pos-point))*1000.0f;
			if(start_angle<end_angle){
				if(angle<start_angle || angle>end_angle)
					return false;
			}else{
				if(angle<start_angle && angle>end_angle)
					return false;
			}
        }
		return true;
    }
    return false;
}
bool Object::rect_cross(Rect4 r) const {
    if(type==OBJ_THT_PAD && tht_shape==1)
		return intersect_circle_rect(r,pos,size.outer);
    else if(type==OBJ_THT_PAD && (tht_shape==4 || tht_shape==7)){
    	for(int q=0;q<poly_points.size();q++)
			if(intersect_circle_rect(r,poly_points[q],size.outer))
				return true;
		Vec2 d=(poly_points[1]-poly_points[0]).Skew();
		d.Normalize(size.outer);
		const Vec2 points[]={
			poly_points[0]-d,
			poly_points[1]-d,
			poly_points[1]+d,
			poly_points[0]+d
		};
		if(intersect_poly_rect(points,4,r))
			return true;

	}else if((type==OBJ_THT_PAD && tht_shape%3!=1) || type==OBJ_SMD_PAD) {
		if(intersect_poly_rect(poly_points.data(),poly_points.size(),r))
			return true;
	}else if(type==OBJ_POLY || type==OBJ_LINE){
		if(type==OBJ_POLY && intersect_poly_rect(poly_points.data(),poly_points.size(),r))
			return true;
		uint8_t styles[2]={get_begin_style(),get_end_style()};
		if(line_width>0){
			for(int q=0;q<poly_points.size()-1;q++){
				const Vec2 &p1 = poly_points[q];
				const Vec2 &p2 = poly_points[(q+1)%poly_points.size()];
				Vec2 points[4];
				expand_line(p1,p2,line_width,q==0&&styles[0]==END_SQUARE,
					q==poly_points.size()-2 &&styles[1]==END_SQUARE,points);
				if(intersect_poly_rect(points,4,r))
					return true;
			}
			for(int q=0;q<poly_points.size();q++){
				const Vec2 &p = poly_points[q];
				if((q==0 && styles[0]==END_ROUND ||
					q==poly_points.size()-1 && styles[1]==END_ROUND ||
					q!=0 && q!=poly_points.size()-1) &&
							intersect_circle_rect(r,p,line_width/2.0f))
					return true;
			}
		}
	}else if(type==OBJ_CIRCLE){
		Vec2 circles[2]; get_ending_circles(circles);
		Vec2 nearest(min(r.x2,max(r.x1,pos.x)),
					min(r.y2,max(r.y1,pos.y)));
		if(point_in(nearest))return true;

		//WORKAROUND, need to fix
		float total_angle=delta_angle(start_angle,end_angle)/1000.0f;
		float radius=get_radius();
		for(float theta = start_angle; theta <= start_angle+total_angle; theta+=(total_angle/s.circle_quality)) {
			Vec2 point(pos.x+radius * cosr(-theta),
						pos.y+radius * sinr(-theta));

			Vec2 n(min(r.x2,max(r.x1,point.x)),
						min(r.y2,max(r.y1,point.y)));
			if(point_in(n))return true;
		}
		//end WORKAROUND
	}
    return false;
}
Rect4 Object::get_aabb()const{
	if((type==OBJ_THT_PAD && tht_shape==1) || type==OBJ_CIRCLE){
		return Rect4(pos.x-size.outer,
					pos.y-size.outer,
					pos.x+size.outer,
					pos.y+size.outer);
	}else if(type==OBJ_THT_PAD && (tht_shape==4 || tht_shape==7)){
		return Rect4(min(poly_points[0].x,poly_points[1].x)-size.outer,
					min(poly_points[0].y,poly_points[1].y)-size.outer,
					max(poly_points[0].x,poly_points[1].x)+size.outer,
					max(poly_points[0].y,poly_points[1].y)+size.outer);
	}else if((type==OBJ_THT_PAD && tht_shape%3!=1) || type==OBJ_SMD_PAD) {
		float minx=FLT_MAX;
		float miny=FLT_MAX;
		float maxx=-FLT_MAX;
		float maxy=-FLT_MAX;
		for(const Vec2 &p : poly_points){
			minx=min(minx,p.x);
			miny=min(miny,p.y);
			maxx=max(maxx,p.x);
			maxy=max(maxy,p.y);
		}
		return Rect4(minx,miny,maxx,maxy);
	}else if(type==OBJ_POLY || type==OBJ_LINE){
		float minx=FLT_MAX;
		float miny=FLT_MAX;
		float maxx=-FLT_MAX;
		float maxy=-FLT_MAX;
		for(const Vec2 &p : poly_points){
			minx=min(minx,p.x);
			miny=min(miny,p.y);
			maxx=max(maxx,p.x);
			maxy=max(maxy,p.y);
		}
		return Rect4(minx-line_width,
					miny-line_width,
					maxx+line_width,
					maxy+line_width);
	}
	return Rect4(0.0f,0.0f,0.0f,0.0f);
}
Vec2 Object::get_position() const{
	if(type==OBJ_POLY || type==OBJ_LINE){
		Vec2 minv(FLT_MAX,FLT_MAX);
		Vec2 maxv(-FLT_MAX,-FLT_MAX);
		for(const Vec2 &v : poly_points){
			minv.x=min(minv.x,v.x);
			minv.y=min(minv.y,v.y);
			maxv.x=max(maxv.x,v.x);
			maxv.y=max(maxv.y,v.y);
		}
		Vec2 p=(minv+maxv)/2.0f;
		return p;
	}
	return pos;
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
	for(Object &o : objects){
		const Rect4 aabb=o.get_aabb();
		if(rect_in_rect(rect,aabb) ||(intersect_rect_rect(rect,aabb) && o.rect_cross(rect)))
			select(o);
	}
}
void Board::select_all(){
	for(Object &o : objects)
		o.selected=true;
}
bool Board::is_selected()const{
	for(const Object &o : objects)
		if(o.selected)
			return true;
	return false;
}
size_t Board::get_selected_count()const{
	size_t count=0;
	for(const Object &o : objects)
		if(o.selected)
			count++;
	return count;
}
bool Board::can_group()const{
	int selected_count=get_selected_count();
	if(selected_count<=1)return false;
	int32_t free_group=get_free_group();
	for(int32_t group=1;group<free_group;group++){
		int count=0;
		for(const Object &o : objects)
			if(o.selected)
				for(const int32_t &g : o.groups)
					if(g==group)count++;
		if(count>1 && count==selected_count)
			return false;
	}
	return true;
}
bool Board::can_ungroup()const{
	int selected_count=get_selected_count();
	if(selected_count<=1)return false;
	int32_t free_group=get_free_group();
	for(int32_t group=1;group<free_group;group++){
		int count=0;
		for(const Object &o : objects)
			if(o.selected)
				for(const int32_t &g : o.groups)
					if(g==group)count++;
		if(count>1)
			return true;
	}
	return false;
}
int32_t Board::get_free_group() const{
	int m=1;
	for(const Object &o : objects)
		for(const int32_t &g : o.groups)
			m=max(m,g);
	return m+1;
}
void Board::set_sel_layer(uint8_t layer){
	for(Object &o : objects)
		if(o.selected)
			o.layer=layer;
}
void Board::group(){
	int32_t free_group=get_free_group();
	for(Object &o : objects)
		if(o.selected)
			o.groups.push_back(free_group);
}
void Board::ungroup(){
	int32_t free_group=get_free_group();
	vector<int32_t>groups(free_group+1);
	for(int32_t group=1;group<free_group;group++){
		int count=0;
		for(const Object &o : objects)
			if(o.selected)
				for(const int32_t &g : o.groups)
					if(g==group)count++;
		groups[group]=count;
	}
	int32_t max_group=*max_element(groups.begin(),groups.end());
	for(Object &o : objects)
		if(o.selected)
			for(int q=0;q<o.groups.size();q++)
				if(groups[o.groups[q]]==max_group){
					o.groups.erase(o.groups.begin()+q);
					break;
				}

}
void Board::set_default(){
	name="New board";
	size=Vec2i(1600000,1000000);
	for(int q=0;q<7;q++){
		ground_pane[q]=0;
		layer_visible[q]=1;
	}
	active_grid_val=12700;
	camera=Vec2i(0,0);
	zoom=0.001;
	active_layer=LAY_C2;
	for(int q=0;q<2;q++)
		images[q].set_default();
	set_anchor(POS_LEFT_BOTTOM);
	is_multilayer=false;
	objects.clear();
}
void Board::build_empty(Vec2i wsize,string n,Vec2i s){
	set_default();
	size=s;
	name=n;
	set_anchor(POS_LEFT_BOTTOM);
	fit(wsize);
}
void Board::build_round(Vec2i wsize,string n,int diameter,int distance){
	set_default();
	size=Vec2i(diameter+distance*2,diameter+distance*2);
	name=n;
	set_anchor(POS_LEFT_BOTTOM,distance);
	fit(wsize);
	Object object;
	object.type=OBJ_CIRCLE;
	object.layer=LAY_O;
	object.pos=Vec2(diameter/2+distance,-diameter/2-distance);
	object.size.inner=object.size.outer=diameter/2;
	objects.push_back(object);
}
void Board::build_rectangle(Vec2i wsize,string n,Vec2i s,int distance){
	set_default();
	size=Vec2i(s.width+distance*2,s.height+distance*2);
	name=n;
	set_anchor(POS_LEFT_BOTTOM,distance);
	fit(wsize);
	Object object;
	object.type=OBJ_LINE;
	object.layer=LAY_O;
	object.line_width=0;
	object.poly_points={
		Vec2(distance,-distance),
		Vec2(distance,-distance-s.height),
		Vec2(distance+s.width,-distance-s.height),
		Vec2(distance+s.width,-distance),
		Vec2(distance,-distance),
	};
	objects.push_back(object);
}
void Board::fit(Vec2i wsize){
	camera=Vec2i(0,0);
	zoom=double(wsize.width)/size.width;
	zoom=min(zoom,double(wsize.width)/size.width);
}
void Board::set_anchor(uint8_t pos,int border){
	switch(pos){
	case POS_LEFT_UP:
		anchor=Vec2i(border,-border);
		break;
	case POS_LEFT_BOTTOM:
		anchor=Vec2i(border,-size.height+border);
		break;
	case POS_RIGHT_UP:
		anchor=Vec2i(size.width-border,-border);
		break;
	case POS_RIGHT_BOTTOM:
		anchor=Vec2i(size.width-border,-size.height+border);
		break;
	}
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
