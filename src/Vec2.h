#pragma once
#include <math.h>
#include <float.h>
#include "File.h"

#include <QPoint>
#include <QPointF>
#include <QSize>

struct Vec2 {
	float x;
	float y;

	inline Vec2() {}
	inline Vec2(float _x,float _y) {
		Set(_x, _y);
	}
	inline explicit Vec2(float angle) {
		x = cosf(angle);
		y = sinf(angle);
	}
	inline Vec2(const Vec2 &vec, float length) {
		float oldLength = vec.Length();
		float k = length / oldLength;
		Set(vec.x * k, vec.y * k);
	}
	inline Vec2(const QPoint &point) {
		Set(point.x(), point.y());
	}
	inline Vec2(const QPointF &point) {
		Set(point.x(), point.y());
	}
	inline Vec2(const QSize &size) {
		Set(size.width(), size.height());
	}

	inline void Set(float _x, float _y) {
		x = _x;
		y = _y;
	}
	inline void SetZero() {
		x = y = 0.0f;
	}
	inline float Length() const {
		return sqrt(x * x + y * y);
	}
	inline float LengthSq() const {
		return x * x + y * y;
	}
	inline void SetLength(float length) {
		float k = length / Length();
		x *= k;
		y *= k;
	}
	inline Vec2 InvY() const {
		return Vec2(x, -y);
	}
	inline Vec2 InvX() const {
		return Vec2(-x, y);
	}

	inline Vec2 Normal() const {
		return Vec2(-y, x);
	}

	inline Vec2 Normal(float length) const {
		Vec2 v(-y, x);
		v.SetLength(length);
		return v;
	}

	inline bool IsValid() const {
		return x == x && y == y;
	}
	static inline Vec2 Invalid() {
		return Vec2(NAN, NAN);
	}

	static inline Vec2 Mean(const Vec2 &a, const Vec2 &b) {
		return (a + b) * 0.5f;
	}

	static inline Vec2 Min(const Vec2 &a, const Vec2 &b) {
		return Vec2((a.x > b.x) ? b.x : a.x,
					(a.y > b.y) ? b.y : a.y);
	}

	static inline Vec2 Max(const Vec2 &a, const Vec2 &b) {
		return Vec2((a.x > b.x) ? a.x : b.x,
					(a.y > b.y) ? a.y : b.y);
	}

	static inline float Dot(const Vec2 &a, const Vec2 &b) {
		return a.x * b.x + a.y * b.y;
	}

    static inline Vec2 Proj(const Vec2 &a, const Vec2 &b) {
        float k = Dot(a, b) / Dot(b, b);
        return b * k;
    }

    static inline float DistanceSegmentToPoint(const Vec2 &a, const Vec2 &b, const Vec2 &point) {
        Vec2 ap = point - a;
        Vec2 ab = b - a;
        Vec2 d = Proj(ap, ab) + a;
        Vec2 ad = d - a;
        float koef = abs(ab.x) > abs(ab.y) ? ad.x / ab.x : ad.y / ad.y;
        if(koef <= 0.0f)
            return (point - a).Length();
        else if(koef >= 1.0f)
            return (point - b).Length();
        return (point - d).Length();
    }

	static inline float Angle(const Vec2 &a, const Vec2 &b) {
		return acos(Dot(a, b) / sqrt(a.LengthSq() * b.LengthSq()));
	}

	inline float Angle() const {
		if(y < 0.0f)
			return M_PI * 2.0f + atan2f(y, x);
		return atan2f(y, x);
	}

	inline Vec2 Rotate(float angle, Vec2 center = {0.0f, 0.0f}) const {
		Vec2 delta(x - center.x, y - center.y);
		return Vec2(
			delta.x * cosf(angle) - delta.y * sinf(angle),
			delta.x * sinf(angle) + delta.y * cosf(angle)
		) + center;
	}

	inline Vec2 Abs() const {
		return Vec2(x > 0.0f ? x : -x, y > 0.0f ? y : -y);
	}

	inline void MirrorHorizontal(float c) {
		x = c * 2.0f - x;
	}

	inline void MirrorVertical(float c) {
		y = c * 2.0f - y;
	}

	static inline void Rotate(Vec2 *array, int count, float angle, Vec2 center = {0.0f, 0.0f}) {
		float s = sinf(angle);
		float c = cosf(angle);

		for(int i = 0; i < count; i++) {
			array[i] -= center;
			array[i].Set(
				array[i].x * c - array[i].y * s,
				array[i].x * s + array[i].y * c
			);
			array[i] += center;
		}
	}

	inline bool operator==(const Vec2 &other) const {
		return other.x == x && other.y == y;
	}
	inline bool operator!=(const Vec2 &other) const {
		return other.x != x || other.y != y;
	}

	inline bool operator<(const Vec2 &other) const {
		if(x < other.x)
			return true;
		else if(x == other.x && y < other.y)
			return true;
		return false;
	}

	inline Vec2 operator-() const {
		return Vec2(-x, -y);
	}
	inline Vec2 operator+(const Vec2 &v) const {
		return Vec2(x + v.x, y + v.y);
	}
	inline Vec2 operator-(const Vec2 &v) const {
		return Vec2(x - v.x, y - v.y);
	}
	inline Vec2 operator*(float k) const {
		return Vec2(x * k, y * k);
	}
	inline Vec2 operator/(float k) const {
		return Vec2(x / k, y / k);
	}
	inline void operator+=(const Vec2 &v) {
		x += v.x;
		y += v.y;
	}
	inline void operator-=(const Vec2 &v) {
		x -= v.x;
		y -= v.y;
	}
	inline void operator*=(const float &v) {
		x *= v;
		y *= v;
	}
	inline void operator/=(const float &v) {
		x /= v;
		y /= v;
	}
	void SavePosition(File &file) const {
		file.WriteMm<float>(x);
		file.WriteMm<float>(-y);
	}
	void LoadPosition(File &file) {
		x = file.ReadMm<float>();
		y = -file.ReadMm<float>();
	}
	void Save(File &file) const {
		file.WriteMm<float>(x);
		file.WriteMm<float>(y);
	}
	void Load(File &file) {
		x = file.ReadMm<float>();
		y = file.ReadMm<float>();
	}
	void SaveInt(File &file) const {
		file.WriteMm<int32_t>(x);
		file.WriteMm<int32_t>(y);
	}
	void LoadInt(File &file) {
		x = file.ReadMm<int32_t>();
		y = file.ReadMm<int32_t>();
	}
};
