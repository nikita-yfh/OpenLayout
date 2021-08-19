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
float Vec2::Normalize(float d){
	float length = Length();
	if (length < __FLT_MIN__)
		return 0.0f;
	float invLength = d / length;
	x *= invLength;
	y *= invLength;
	return length;
}

Vec2i::Vec2i(Vec2 v){
	x=v.x;
	y=v.y;
}
Vec2::Vec2(float v1,float v2){
	x=v1;
	y=v2;
}
Vec2::Vec2(Vec2i v){
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
float cosr(float v){
	return cosf(v*M_PI/180.0f);
}
float sinr(float v){
	return sinf(v*M_PI/180.0f);
}
float get_angle_v(Vec2 v){
	float a;
	if(v.x>=0)
		a= M_PI+atanf(v.y/v.x);
	else if(v.y>0)
		a= 2*M_PI+atanf(v.y/v.x);
	else
		a= atanf(v.y/v.x);
	return a/M_PI*180.0f; //degress only
}
void rotate_v(Vec2 &v,float a){
	v=Vec2(
		v.x * cosr(a) + v.y * sinr(a),
		-v.x * sinr(a) + v.y * cosr(a)
   );
}
void rotate_v(Vec2 n,Vec2 &v,float angle){
	Vec2 p=v-n;
	rotate_v(p,angle);
	v=p+n;
}
Vec2 bis(Vec2 v1,Vec2 v2,float length) {
	v1.Normalize();
	if(v1.Length()==0) {
		v2=v2.Skew();
		v2.Normalize();
		return length*v2;
	}
	v2.Normalize();
	if(v1==-v2)
		v1+=0.001*v1.Skew();
	Vec2 sum=v1+v2;
	sum.Normalize();
	if(v1.x*v2.y-v1.y*v2.x<0)
		sum=-sum;

	sum*=length;

	return sum;
}
