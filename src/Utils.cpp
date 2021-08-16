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
float Vec2::Normalize(){
	float length = Length();
	if (length < __FLT_MIN__)
		return 0.0f;
	float invLength = 1.0f / length;
	x *= invLength;
	y *= invLength;
	return length;
}

Vec2::Vec2(float v1,float v2){
	x=v1;
	y=v2;
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
float get_angle(Vec2 v){
	float a;
	if(v.x>=0)
		a= M_PI+atanf(v.y/v.x);
	else if(v.y>0)
		a= 2*M_PI+atanf(v.y/v.x);
	else
		a= atanf(v.y/v.x);
	return a;
}
float to_deg(float rad){
	return rad/M_PI*180.0f;
}
