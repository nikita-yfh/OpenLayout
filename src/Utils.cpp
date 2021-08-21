#include "Utils.h"
float to_mm(float v) {
    return v*0.0254f;
}
float to_mil(float v) {
    return v/0.0254f;
}
string get_grid_str(float grid) {
    //39.6875µm, 79.375µm, 158.75µm, ..., 5.08mm
    char mm[40];
    if(to_str(grid).size()>6) //if µm more compact
        sprintf(mm,"%g %s",grid*1000.0f,"um");
    else
        sprintf(mm,"%g %s",grid,"mm");
    return mm;
}
Rect4::Rect4(float xp1,float yp1,float xp2,float yp2){
	x1=xp1;
	x2=xp2;
	y1=yp1;
	y2=yp2;
}
Rect4::Rect4(Vec2 v1,Vec2 v2){
	x1=v1.x;
	x2=v2.x;
	y1=v1.y;
	y2=v2.y;
}
void Rect4::Normalize(){
	*this=Rect4(
		min(x1,x2),
		min(y1,y2),
		max(x1,x2),
		max(y1,y2)
	);

}
void Rect4::SetP1(Vec2 p){
	x1=p.x;
	y1=p.y;
}
void Rect4::SetP2(Vec2 p){
	x2=p.x;
	y2=p.y;
}
float Vec2::Normalize(float d) {
    float length = Length();
    if (length < __FLT_MIN__)
        return 0.0f;
    float invLength = d / length;
    x *= invLength;
    y *= invLength;
    return length;
}

Vec2i::Vec2i(Vec2 v) {
    x=v.x;
    y=v.y;
}
Vec2i::Vec2i(int32_t xp,int32_t yp) {
    x=xp;
    y=yp;
}
Vec2::Vec2(float v1,float v2) {
    x=v1;
    y=v2;
}
Vec2::Vec2(Vec2i v) {
    x=(float)v.x;
    y=(float)v.y;
}
bool Vec2::operator==(Vec2 other) {
    return x==other.x && y==other.y;
}
bool Vec2::operator<(Vec2 other) {
    return x<other.x || y<other.y;
}

void Vec2::SetZero() {
    x = 0.0f;
    y = 0.0f;
}
void Vec2::Set(float x_, float y_) {
    x = x_;
    y = y_;
}
Vec2 Vec2::operator -() const {
    Vec2 v;
    v.Set(-x, -y);
    return v;
}
void Vec2::operator += (const Vec2& v) {
    x += v.x;
    y += v.y;
}
void Vec2::operator -= (const Vec2& v) {
    x -= v.x;
    y -= v.y;
}
void Vec2::operator *= (float a) {
    x *= a;
    y *= a;
}
float Vec2::Length() const {
    return sqrt(x * x + y * y);
}
Vec2 Vec2::Skew() const {
    return Vec2(-y, x);
}
Vec2 Vec2::SwapY() const {
    return Vec2(x,-y);
}
float cosr(float v) {
    return cosf(v*M_PI/180.0f);
}
float sinr(float v) {
    return sinf(v*M_PI/180.0f);
}
float get_angle_v(Vec2 v) {
    float a;
    if(v.x>=0)
        a= M_PI+atanf(v.y/v.x);
    else if(v.y>0)
        a= 2*M_PI+atanf(v.y/v.x);
    else
        a= atanf(v.y/v.x);
    return a;
}
float deg(float v) {
    return v/M_PI*180.0f;
}
float rad(float v) {
    return v*M_PI/180.0f;
}
void rotate_v(Vec2 &v,float a) {
    v=Vec2(
          v.x * cosr(a) + v.y * sinr(a),
          -v.x * sinr(a) + v.y * cosr(a)
      );
}
void rotate_v(Vec2 n,Vec2 &v,float angle) {
    Vec2 p=v-n;
    rotate_v(p,angle);
    v=p+n;
}
float delta_angle(float a1,float a2) {
    if(a2>a1)
        return a2-a1;
    return 360000.0f-a1+a2;
}
bool intersect_circle_rect(Rect4 rect, Vec2 c, float r) {
    if(intersect_rect_rect(rect,Rect4(c-Vec2(r,r),c+Vec2(r,r))))
		return true;

	const Vec2 points[]={
		Vec2(rect.x1,rect.y1),
		Vec2(rect.x1,rect.y2),
		Vec2(rect.x2,rect.y2),
		Vec2(rect.x2,rect.y1)
	};
	for(const Vec2 &p : points){
		if((p.x-c.x)*(p.x-c.x) + (p.y-c.y)*(p.y-c.y) <= r*r)
			return true;
	}
	return false;

}
bool intersect_rect_rect(Rect4 r1,Rect4 r2){
	return(r1.x2>r2.x1 && r1.y2>r2.y1&&
		r1.x1<r2.x2 && r1.y1<r2.y2);
}
