#include "Poly.h"
#include "GLUtils.h"
#include "Utils.h"

Poly *Poly::Clone() const {
	return new Poly(*this);
}

AABB Poly::GetAABB() const {
	Vec2 max(0.0f, 0.0f);
	Vec2 min(500.0f, 500.0f);

	for(int i = 0; i < points.Size(); i++) {
		const Vec2 &point = points[i];
		if(point.x < min.x)
			min.x = point.x;
		if(point.y < min.y)
			min.y = point.y;
		if(point.x > max.x)
			max.x = point.x;
		if(point.y > max.y)
			max.y = point.y;
	}
	Vec2 size(width / 2.0f, width / 2.0f);
	return AABB(min - size, max + size);
}

bool Poly::TestPoint(const Vec2 &point) const {
	if(utils::PointInConcavePolygon(point, points.Size(), &points[0]))
		return true;
	for(int i = 0; i < points.Size(); i++) {
		if(utils::PointInPolySegment(point, points[i], points[(i + 1) % points.Size()], width / 2.0f))
			return true;
		if(utils::PointInCircle(point, points[i], width / 2.0f))
			return true;
	}
	return false;
}

void Poly::SaveObject(File &file) const {
	file.Write<uint8_t>(Object::POLY);
	file.WriteNull(16);
	file.WriteMm<uint32_t>(width);
	file.WriteNull(1);
	file.Write<uint8_t>(layer + 1);
	file.WriteNull(5);
	file.Write<uint16_t>(componentID);
	file.WriteNull(11);
	file.WriteMm<uint32_t>(groundDistance);
	file.WriteNull(5);
	file.Write<uint8_t>(hatched);
	file.Write<uint8_t>(customHatch);
	file.Write<uint8_t>(cutoff);
	file.WriteMm<uint32_t>(hatchSize);
	file.WriteNull(1);
	file.Write<uint8_t>(soldermask);
	file.WriteNull(15);
	file.Write<uint8_t>(onlySoldermask);
	file.WriteNull(6);
	file.WriteString(marker);

	SaveGroups(file);
	SavePoints(file, onlySoldermask);
}


void Poly::LoadObject(File &file) {
	file.ReadNull(16);
	width = file.ReadMm<uint32_t>();
	file.ReadNull(1);
	layer = file.Read<uint8_t>() - 1;
	file.ReadNull(5);
	componentID = file.Read<uint16_t>();
	file.ReadNull(11);
	groundDistance = file.ReadMm<uint32_t>();
	file.ReadNull(5);
	hatched = file.Read<uint8_t>();
	customHatch = file.Read<uint8_t>();
	cutoff = file.Read<uint8_t>();
	hatchSize = file.ReadMm<uint32_t>();
	file.ReadNull(1);
	soldermask = file.ReadMm<uint8_t>();
	file.ReadNull(15);
	onlySoldermask = file.Read<uint8_t>();
	file.ReadNull(6);
	file.ReadString(marker);

	LoadGroups(file);
	LoadPoints(file, onlySoldermask);
}

void Poly::DrawGroundDistance() const {
	if(cutoff)
		Draw(width / 2.0f);
	else
		Draw(width / 2.0f + groundDistance);
}
void Poly::DrawObject() const {
	if(!cutoff)
		Draw(width / 2.0f);
}

void Poly::Draw(float halfWidth) const {
	if(points.Size() < 3)
		return;

	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < points.Size(); i++)
		glutils::Vertex(points[i]);
	glEnd();

	if(width != 0.0f) {
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glBegin(GL_QUADS);
		for(int i = 0; i < points.Size(); i++) {
			const Vec2 &a = points[i];
			const Vec2 &b = points[(i + 1) % points.Size()];

			Vec2 delta = (b - a).Normal(halfWidth);

			glutils::Vertex(a - delta);
			glutils::Vertex(a + delta);
			glutils::Vertex(b + delta);
			glutils::Vertex(b - delta);
		}
		glEnd();
		for(int i = 0; i < points.Size(); i++)
			glutils::DrawCircle(points[i], halfWidth);
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

	glRectf(0.0f, 0.0f, 500.0f, 500.0f);

	glDisable(GL_STENCIL_TEST);
}

