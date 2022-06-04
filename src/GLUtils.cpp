#include "GLUtils.h"

namespace glutils{

void DrawCircle(const Vec2 &position, float radius) {
	glBegin(GL_TRIANGLE_FAN);
	Vertex(position);
	for(float i = 0.0f; i < 2.0f * M_PI; i += stepAngle) {
		Vec2 rot = Vec2(i) * radius;
		Vertex(position + rot);
	}
	Vertex(position + Vec2(radius, 0.0f));
	glEnd();
}

void DrawSector(const Vec2 &position, float radius, float begin, float end) {
	glBegin(GL_TRIANGLE_FAN);
	Vertex(position);
	for(float i = begin; i < end; i += stepAngle) {
		Vec2 vec = Vec2(-i) * radius;
		Vertex(position + vec);
	}
	Vertex(position + Vec2(-end) * radius);
	glEnd();
}

void DrawTriangleFan(const Vec2 &to, const Vec2 &origin, const Vec2 &begin, const Vec2 &end, bool clockwise) {
	float angle1 = (begin - origin).Angle();
	float angle2 = (end - origin).Angle();

	if (clockwise) {
		if (angle1 > angle2)
			angle1 -= M_PI * 2.0f;
	} else {
		if (angle2 > angle1)
			angle2 -= M_PI * 2.0f;
	}
	
	float jointAngle = angle2 - angle1;
	int numTriangles = abs(jointAngle) / stepAngle + 1;
	float step = jointAngle / numTriangles;
	glBegin(GL_TRIANGLE_FAN);
	Vertex(to);
	Vertex(begin);
	for(int i = 1; i < numTriangles; i++)
		Vertex(origin + (begin - origin).Rotate(i * step));
	Vertex(end);
	glEnd();
}

void DrawRectangle(const Vec2 &halfsize) {
	glBegin(GL_QUADS);
	Vertex(halfsize);
	Vertex(halfsize.InvX());
	Vertex(-halfsize);
	Vertex(halfsize.InvY());
	glEnd();
}

};
