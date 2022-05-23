#include "PolygonBase.h"
#include "GLUtils.h"

PolygonBase::PolygonBase() {
	points = nullptr;
	count = 0;
}

PolygonBase::~PolygonBase() {
	if(count)
		free(points);
}

void PolygonBase::DrawLine(float halhWidth, bool loop, bool begin, bool end) const {
	float angles[count];
	glBegin(GL_QUADS);
	for(int i = 0; i < count - !loop; i++) {
		const Vec2 &a = points[i];
		const Vec2 &b = points[(i + 1) % count];

		Vec2 delta = b - a;
		delta.SetLength(halhWidth);
		delta = Vec2(delta.y, -delta.x);
		angles[i] = delta.Angle();

		glutils::Vertex(a - delta);
		glutils::Vertex(a + delta);
		glutils::Vertex(b + delta);
		glutils::Vertex(b - delta);
	}
	glEnd();
	for(int i = !begin; i < count - !end; i++)
		glutils::DrawCircle(points[i], halhWidth);
}

void PolygonBase::LoadPoints(File &file, bool onlySoldermask) {
	count = file.Read<uint32_t>() - onlySoldermask;
	points = (Vec2*) malloc(sizeof(Vec2) * count);
	for(int i = 0; i < count; i++)
		points[i].LoadPosition(file);
}

void PolygonBase::SavePoints(File &file, bool onlySoldermask) const {
	file.Write<uint32_t>(count + onlySoldermask);
	for(int i = 0; i < count; i++)
		points[i].SavePosition(file);
}


