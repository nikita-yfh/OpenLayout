#pragma once
#include "Vec2.h"
#include <wx/glcanvas.h>
#include <math.h>

namespace glutils {

constexpr uint8_t numSegments = 60;
constexpr float stepAngle = M_PI * 2.0f / numSegments;

void DrawCircle(const Vec2 &position, float radius);
void DrawSector(const Vec2 &position, float radius, float begin, float end);
void DrawTriangleFan(const Vec2 &to, const Vec2 &origin, const Vec2 &begin, const Vec2 &end, bool clockwise);
void DrawRectangle(const Vec2 &size);

inline float RadToDeg(float rad){
	return rad / M_PI * 180.0f;
}
inline float DegToRad(float deg){
	return deg / 180.0f * M_PI;
}
inline float AngleMod(float rad) {
	return fmod(rad, M_PI * 2.0f);
}
inline void Vertex(const Vec2 &p){
	glVertex2f(p.x, p.y);
}
inline void Translate(const Vec2 &p){
	glTranslatef(p.x, p.y, 0.0f);
}
inline void Rotate(float angle){
	glRotatef(RadToDeg(angle), 0.0f, 0.0f, 1.0f);
}
inline void Scale(float s){
	glScalef(s, s, 1.0f);
}
inline void Scale(float x, float y){
	glScalef(x, y, 1.0f);
}

};
