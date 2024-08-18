#include "Poly.h"
#include "GLUtils.h"
#include "Utils.h"

Poly *Poly::Clone() const {
	return new Poly(*this);
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
	file.Write<uint8_t>(cutout);
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
	cutout = file.Read<uint8_t>();
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
	if(cutout)
		Draw(width / 2.0f);
	else
		Draw(width / 2.0f + groundDistance);
}
void Poly::DrawObject() const {
	if(!cutout)
		Draw(width / 2.0f);
}

void Poly::Draw(float halfWidth) const {
	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

	if(isRect || !IsPlaced()) {
		glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < points.Size(); i++)
			glutils::Vertex(points[i]);
		glEnd();
	}

	if(halfWidth != 0.0f) {
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glBegin(GL_QUADS);
		for(int i = 0; i < points.Size(); i++) {
			if(i == points.Size() - 1 && IsPlaced() && !isRect)
				break;

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

	glRectf(-500.0f, -500.0f, 500.0f, 500.0f);

	glDisable(GL_STENCIL_TEST);
}

void Poly::BuildRect(const Vec2 &mousePosition) {
    points.Resize(4);
    points[1].Set(points[0].x, mousePosition.y);
    points[2] = mousePosition;
    points[3].Set(mousePosition.x, points[0].y);
}
