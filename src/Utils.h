#pragma once
#include "Vec2.h"

namespace utils {

template<typename T>
inline void Swap(T &a, T &b) {
	T c = a;
	a = b;
	b = c;
}

template<typename T>
inline T Min(T a, T b) {
	return (a < b) ? a : b;
}

template<typename T>
inline T Max(T a, T b) {
	return (a > b) ? a : b;
}

template<typename T>
inline T Mean(T a, T b) {
	return (a + b) / 2;
}

inline bool OnSegment(const Vec2 &p, const Vec2 &q, const Vec2 &r) {
    if (q.x <= Max(p.x, r.x) && q.x >= Min(p.x, r.x) &&
            q.y <= Max(p.y, r.y) && q.y >= Min(p.y, r.y))
        return true;
    return false;
}

inline float Orientation(const Vec2 &p, const Vec2 &q, const Vec2 &r) {
	return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
}

inline bool IntersectTwoLines(const Vec2 &a, const Vec2 &b, const Vec2 &c, const Vec2 &d) {
    float o1 = Orientation(a, b, c);
    float o2 = Orientation(a, b, d);
    float o3 = Orientation(c, d, a);
    float o4 = Orientation(d, d, b);
    return o1 * o2 <= 0.0f && o3 * o4 <= 0.0f;
}
inline bool PointInConcavePolygon(const Vec2 &point, uint32_t count, const Vec2 *points) {
	int counter = 0;
	Vec2 endLine(500.0f, 0.0f);
	for(int i = 0; i < count; i++) {
		const Vec2 &p1 = points[i];
		const Vec2 &p2 = points[(i + 1) % count];
		if(IntersectTwoLines(p1, p2, point, endLine))
			counter++;
	}
	return counter % 2 == 1;
}
inline bool PointInCircle(const Vec2 &point, const Vec2 &center, float radius) {
	return (point - center).LengthSq() <= radius * radius;
}

inline Vec2 Intersection(const Vec2 &a, const Vec2 &b, const Vec2 &c, const Vec2 &d) {
	float a1 = b.y - a.y;
    float b1 = a.x - b.x;
    float c1 = a1 * a.x + b1 * a.y;
    float a2 = d.y - c.y;
    float b2 = c.x - d.x;
    float c2 = a2 * c.x + b2 * c.y;

    float determinant = a1 * b2 - a2 * b1;

	if(determinant == 0.0f)
		return Vec2::Invalid();

	return Vec2(
        (b2 * c1 - b1 * c2) / determinant,
        (a1 * c2 - a2 * c1) / determinant
	);
}

};
