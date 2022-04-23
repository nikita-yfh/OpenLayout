#pragma once
#include <math.h>
#include "File.h"

struct Vec2 {
	float x;
	float y;

	Vec2() {}
	Vec2(float _x,float _y) {
		Set(_x, _y);
	}

	void Set(float _x,float _y) {
		x = _x;
		y = _y;
	}
	void SetZero() {
		x = y =0.0f;
	}
	float Length() const {
		return sqrt(x*x + y*y);
	}
	float LengthSq() const {
		return x*x + y*y;
	}

	Vec2 operator-() const {
		return Vec2(-x, -y);
	}
	Vec2 operator~() const {
		return Vec2(-y, x);
	}
	Vec2 operator+(const Vec2 &v) const {
		return Vec2(x+v.x, y+v.y);
	}
	Vec2 operator-(const Vec2 &v) const {
		return Vec2(x-v.x, y-v.y);
	}
	Vec2 operator*(float k) const {
		return Vec2(x*k, y*k);
	}
	Vec2 operator/(float k) const {
		return Vec2(x/k, y/k);
	}
	void operator+=(const Vec2 &v) {
		x += v.x;
		y += v.y;
	}
	void operator-=(const Vec2 &v) {
		x -= v.x;
		y -= v.y;
	}
	void operator*=(const float &v) {
		x *= v;
		y *= v;
	}
	void operator/=(const float &v) {
		x /= v;
		y /= v;
	}
	template<typename T>
	void Save(File &file) const {
		file.WriteMm<T>(x);
		file.WriteMm<T>(y);
	}
	template<typename T>
	void Load(File &file) {
		x = file.ReadMm<T>();
		y = file.ReadMm<T>();
	}
};
