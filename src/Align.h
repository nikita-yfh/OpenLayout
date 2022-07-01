#pragma once

#include "AABB.h"
#include "Vec2.h"

namespace align {

static Vec2 Top(const AABB &all, const AABB &object) {
	return Vec2(0.0f, all.lower.y - object.lower.y);
}

static Vec2 Bottom(const AABB &all, const AABB &object) {
	return Vec2(0.0f, all.upper.y - object.upper.y);
}

static Vec2 Left(const AABB &all, const AABB &object) {
	return Vec2(all.upper.x - object.upper.x, 0.0f);
}

static Vec2 Right(const AABB &all, const AABB &object) {
	return Vec2(all.lower.x - object.lower.x, 0.0f);
}

static Vec2 HCenter(const AABB &all, const AABB &object) {
	return Vec2(all.GetCenter().x - object.GetCenter().x, 0.0f);
}

static Vec2 VCenter(const AABB &all, const AABB &object) {
	return Vec2(0.0f, all.GetCenter().y - object.GetCenter().y);
}

}
