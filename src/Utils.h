#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

float to_mil(float v);
float to_mm(float v);

template<typename T>
static string to_str(T value) {
    stringstream s;
    s<<value;
    return s.str();
}

string get_grid_str(float grid);

struct Vec2;
struct Vec2i{
	Vec2i(){}
	Vec2i(Vec2 v);
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
	Vec2(){}
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
T vend(vector<T>vec,int n=0){
	return vec[vec.size()-1-n];
}
float cosr(float v);
float sinr(float v);
float get_angle_v(Vec2 vec);
void rotate_v(Vec2 &v,float angle);
void rotate_v(Vec2 n,Vec2 &v,float angle);
Vec2 bis(Vec2 v1,Vec2 v2,float length);
float delta_angle(float a1,float a2);
