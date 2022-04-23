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

	inline Vec2 GetCenter() const {
		return (lower + upper) * 0.5f;
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

	Vec2 lower;
	Vec2 upper;
};


