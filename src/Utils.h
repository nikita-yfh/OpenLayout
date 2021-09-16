#pragma once
#include <wx/string.h>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;
float range(float v,float a,float b);
float to_mil(float v);
float to_mm(float v);

template<typename T>
static string to_str(T value) {
	stringstream s;
	s<<value;
	return s.str();
}

wxString get_grid_str(float grid);

struct Vec2;
struct Rect4 {
	Rect4() {}
	Rect4(float xp1,float yp1,float xp2,float yp2);
	Rect4(Vec2 v1,Vec2 v2);
	float x1;
	float y1;
	float x2;
	float y2;
	void Normalize();
	void SetP1(Vec2 p);
	void SetP2(Vec2 p);
	Vec2 GetP1() const;
	Vec2 GetP2() const;
};

struct Vec2i {
	Vec2i() {}
	Vec2i(Vec2 v);
	Vec2i(int32_t xp,int32_t yp);
	union {
		int32_t width;
		int32_t x;
	};
	union {
		int32_t height;
		int32_t y;
	};
};
struct Vec2 {
	Vec2() {}
	Vec2(float v1,float v2);
	Vec2(Vec2i v);
	bool operator==(Vec2 other);
	bool operator<(Vec2 other);
	void SetZero();
	void Set(float x_, float y_);
	Vec2 operator -() const;
	void operator += (const Vec2& v);
	void operator -= (const Vec2& v);
	void operator *= (float a);
	float Length() const;
	float LengthSq() const;
	Vec2 Skew() const;
	Vec2 SwapY() const;
	float Normalize(float d=1.0f);
	union {
		float width;
		float inner;
		float x;
	};
	union {
		float height;
		float outer;
		float y;
	};
};
inline Vec2 operator + (const Vec2& a, const Vec2& b) {
	return Vec2(a.x + b.x, a.y + b.y);
}
inline Vec2 operator - (const Vec2& a, const Vec2& b) {
	return Vec2(a.x - b.x, a.y - b.y);
}
inline Vec2 operator * (float s, const Vec2& a) {
	return Vec2(s * a.x, s * a.y);
}
inline Vec2 operator * (const Vec2& a,float s) {
	return Vec2(s * a.x, s * a.y);
}
inline Vec2 operator / (const Vec2& a,float s) {
	return Vec2(a.x/s, a.y/s);
}
inline bool operator == (const Vec2& a, const Vec2& b) {
	return a.x == b.x && a.y == b.y;
}
inline bool operator != (const Vec2& a, const Vec2& b) {
	return a.x != b.x || a.y != b.y;
}
inline float Distance(const Vec2& a, const Vec2& b) {
	Vec2 c = a - b;
	return c.Length();
}

template<typename T>
T vend(vector<T>vec,int n=0) {
	return vec[vec.size()-1-n];
}
float delta_angle(float a1,float a2);
float cosr(float v);
float sinr(float v);
float get_angle_v(Vec2 vec);
void rotate_v(Vec2 &v,float angle);
void rotate_v(Vec2 n,Vec2 &v,float angle);
Vec2 bis(Vec2 v1,Vec2 v2,float length);
float deg(float v);
float rad(float v);
bool intersect_circle_rect(Rect4 rect, Vec2 c, float r);
bool intersect_rect_rect(Rect4 a,Rect4 b);
bool intersect_poly_rect(const Vec2 *points,int num,Rect4 r);
bool rect_in_rect(Rect4 r1,Rect4 r2);
bool intersect_line_line(Vec2 a,Vec2 b,Vec2 c,Vec2 d);
bool point_in_poly(const Vec2 *points,int num,Vec2 p);
bool point_in_poly_c(const Vec2 *points,int num,Vec2 p); //non-convex polygons
bool point_in_rect(Rect4 r,Vec2 p);
bool point_in_circle(Vec2 p, Vec2 c, float r);
bool points_one_side(Vec2 a,Vec2 b,Vec2 c,Vec2 d);
void expand_line(Vec2 p1,Vec2 p2,float width,bool s1,bool s2, Vec2 *res);
