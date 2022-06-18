#pragma once

#include "Vec2.h"

class AABB {
public:
	AABB() {}

	AABB(const Vec2 &_lower, const Vec2 &_upper) {
		Set(_lower, _upper);
	}

	inline void Set(const Vec2 &_lower, const Vec2 _upper) {
		lower = _lower;
		upper = _upper;
	}

	inline Vec2 Size() const {
		return upper - lower;
	}
	
	inline float Width() const {
		return Size().x;
	}

	inline float Height() const {
		return Size().y;
	}

	inline Vec2 GetCenter() const {
		return (lower + upper) * 0.5f;
	}

	bool TestPoint(const Vec2 &point) const {
		return
			point.x <= upper.x &&
			point.y <= upper.y &&
			point.x >= lower.x &&
			point.y >= lower.y;
	}

	inline bool Contains(const AABB& other) const {
		return
			lower.x <= other.lower.x &&
			lower.y <= other.lower.y &&
			other.upper.x <= upper.x &&
			other.upper.y <= upper.y;
	}

	inline bool TestOverlap(const AABB &other) {
		Vec2 d1 = other.lower - upper;
		Vec2 d2 = lower - other.upper;

		if (d1.x > 0.0f || d1.y > 0.0f)
			return false;

		if (d2.x > 0.0f || d2.y > 0.0f)
			return false;

		return true;
	}

	inline AABB Expand(float f) {
		return Expand(Vec2(f, f));
	}
	inline AABB Expand(const Vec2 &vec) {
		return AABB(lower - vec, upper + vec);
	}
	inline AABB operator+(const Vec2 &vec) {
		return AABB(lower + vec, upper + vec);
	}
	inline AABB operator-(const Vec2 &vec) {
		return AABB(lower - vec, upper - vec);
	}
	inline void operator+=(const Vec2 &vec) {
		lower += vec;
		upper += vec;
	}
	inline void operator-=(const Vec2 &vec) {
		lower -= vec;
		upper -= vec;
	}

	inline AABB operator|(const AABB &other) {
		return AABB(Vec2::Min(lower, other.lower), Vec2::Max(upper, other.upper));
	}
	inline void operator|=(const AABB &other) {
		lower = Vec2::Min(lower, other.lower);
		upper = Vec2::Max(upper, other.upper);
	}
	inline AABB operator|(const Vec2 &vec) {
		return AABB(Vec2::Min(lower, vec), Vec2::Max(upper, vec));
	}
	inline void operator|=(const Vec2 &vec) {
		lower = Vec2::Min(lower, vec);
		upper = Vec2::Max(upper, vec);
	}

	Vec2 lower;
	Vec2 upper;
};
