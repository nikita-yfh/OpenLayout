#include "Utils.h"
#include <algorithm>
#include <cfloat>
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
Rect4::Rect4(float xp1,float yp1,float xp2,float yp2) {
	x1=xp1;
	x2=xp2;
	y1=yp1;
	y2=yp2;
}
Rect4::Rect4(Vec2 v1,Vec2 v2) {
	x1=v1.x;
	x2=v2.x;
	y1=v1.y;
	y2=v2.y;
}
void Rect4::Normalize() {
	*this=Rect4(
			  min(x1,x2),
			  min(y1,y2),
			  max(x1,x2),
			  max(y1,y2)
		  );

}
void Rect4::SetP1(Vec2 p) {
	x1=p.x;
	y1=p.y;
}
void Rect4::SetP2(Vec2 p) {
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
float Vec2::LengthSq() const {
	return x * x + y * y;
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
bool intersect_line_circle(Vec2 a,Vec2 b,Vec2 c, float r) {
	Vec2 d(a.x-c.x,a.y-c.y);
	Vec2 k(b.x-a.x,b.y-a.y);

}
bool intersect_circle_rect(Rect4 rect, Vec2 c, float r) {
	Vec2 nearest(
		min(rect.x2,max(rect.x1,c.x)),
		min(rect.y2,max(rect.y1,c.y))
	);
	return (nearest-c).LengthSq()<=r*r;
}
bool point_in_circle(Vec2 p, Vec2 c, float r) {
	return (p-c).LengthSq()<=r*r;
}
bool intersect_rect_rect(Rect4 a,Rect4 b) {
	return  !(
				(b.x2 < a.x1) ||
				(b.x1 > a.x2) ||
				(b.y2 < a.y1) ||
				(b.y1 > a.y2));
}
bool rect_in_rect(Rect4 r1,Rect4 r2) {
	return(r1.x1<=r2.x1 && r1.y1<=r2.y1&&
		   r1.x2>=r2.x2 && r1.y2>=r2.y2);
}
float point1_side(Vec2 a,Vec2 b,Vec2 c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

bool _intersect_line_line(float a, float b, float c, float d) {
	if (a > b)  swap (a, b);
	if (c > d)  swap (c, d);
	return max(a,c) <= min(b,d);
}
bool intersect_line_line(Vec2 a,Vec2 b,Vec2 c,Vec2 d) {
	return _intersect_line_line (a.x, b.x, c.x, d.x)
		   && _intersect_line_line (a.y, b.y, c.y, d.y)
		   && point1_side(a,b,c) * point1_side(a,b,d) <= 0.0f
		   && point1_side(c,d,a) * point1_side(c,d,b) <= 0.0f;
}
bool points_one_side(Vec2 a,Vec2 b,Vec2 c,Vec2 d) {
	return point1_side(a, b, c) * point1_side(a, b, d) >= 0.0f;
}
bool point_in_poly(const Vec2 *points,int num,Vec2 p) {
	for(int q=0; q<num; q++) {
		Vec2 p1=points[q];
		Vec2 p2=points[(q+1)%num];
		Vec2 p3=points[(q+2)%num];
		if(!points_one_side(p1,p2,p3,p))return false;
	}
	return true;
}

bool point_in_poly_c(const Vec2 *points,int num,Vec2 p) {
	int counter=0;
	for(int q=0; q<num; q++) {
		const Vec2 &p1=points[q];
		const Vec2 &p2=points[(q+1)%num];
		if(intersect_line_line(p1,p2,p,p+Vec2(10000000.0f,0.0f)))
			counter++;
	}
	return counter%2==1;
}

bool point_in_rect(Rect4 r,Vec2 p) {
	return p.x < r.x2 && p.y < r.y2 &&
		   p.x > r.x1 && p.y > r.y1;
}
bool intersect_poly_poly(const Vec2 *a,int an,const Vec2 *b,int bn) {
	for(int ac=0; ac<an; ac++) {
		const Vec2 &p1=a[ac];
		const Vec2 &p2=a[(ac+1)%an];
		for(int bc=0; bc<bn; bc++) {
			const Vec2 &p3=b[bc];
			const Vec2 &p4=b[(bc+1)%bn];
			if(intersect_line_line(p1,p2,p3,p4))
				return true;
		}
	}
	return false;
}
bool intersect_poly_rect(const Vec2 *points,int num,Rect4 r) {
	Vec2 rp[] {
		Vec2(r.x1,r.y2),
		Vec2(r.x2,r.y2),
		Vec2(r.x2,r.y1),
		Vec2(r.x1,r.y1)
	};
	return intersect_poly_poly(points,num,rp,4);
}
void expand_line(Vec2 p1,Vec2 p2,float width,bool s1,bool s2,Vec2 *res) {
	Vec2 c=(p2-p1);
	c.Normalize(width/2.0f);

	Vec2 d=c.Skew();

	Vec2 c1(0.0f,0.0f);
	Vec2 c2(0.0f,0.0f);

	if(s1)c1=c;
	if(s2)c2=c;


	res[0]=p1-d-c1;
	res[1]=p1+d-c1;
	res[2]=p2+d+c2;
	res[3]=p2-d+c2;
}
