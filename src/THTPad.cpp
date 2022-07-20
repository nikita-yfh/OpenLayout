#include "THTPad.h"
#include "GLUtils.h"
#include "Utils.h"

THTPad *THTPad::Clone() const {
	return new THTPad(*this);
}

AABB THTPad::GetAABB() const {
	if(shape == S_CIRCLE)
		return AABB(position, position).Expand(size.out * 0.5f);
	if(shape == S_CIRCLE_E)
		return AABB(position, position).Expand(Vec2(angle).Abs() * size.out * 0.5f).Expand(size.out * 0.5f);
	Vec2 points[4];
	uint8_t count = CalcPoints(points);
	Vec2::Rotate(points, count, angle);
	Vec2 max(0.0f, 0.0f);
	for(uint8_t i = 0; i < count; i++)
		max = Vec2::Max(max, points[i].Abs());
	return AABB(position, position).Expand(max);
}

bool THTPad::TestPoint(const Vec2 &point) const {
	float radius = size.out * 0.5f;
	Vec2 _point = (point - position).Rotate(-angle);
	switch(shape) {
	case S_CIRCLE:
		return utils::PointInCircle(point, position, radius);
	case S_CIRCLE_E:
		if(utils::PointInCircle(_point, Vec2(-radius, 0.0f), radius))
			return true;
		if(utils::PointInCircle(_point, Vec2( radius, 0.0f), radius))
			return true;
	case S_SQUARE:
		return abs(_point.x) < radius && abs(_point.y) < radius;
	case S_SQUARE_E:
		return abs(_point.x) < size.out && abs(_point.y) < radius;
	case S_OCTAGON:
		if(abs(_point.x) > radius || abs(_point.y) > radius)
			return false;
		return abs(_point.x) + abs(_point.y) < radius * sqrtf(2.0f);
	case S_OCTAGON_E:
		if(abs(_point.x) > size.out || abs(_point.y) > radius)
			return false;
		return abs(_point.x) + abs(_point.y) < radius * (sqrtf(2.0f) + 1.0f);
	}
	return false;
}

void THTPad::SaveObject(File &file) const {
	file.Write<uint8_t>(THT_PAD);
	position.SavePosition(file);
	size.Save(file);
	file.WriteNull(5);
	file.Write<uint8_t>(layer + 1);
	file.Write<uint8_t>(shape + 1);
	file.WriteNull(4);
	file.Write<uint16_t>(componentID);
	file.WriteNull(1);
	file.Write(thermalStyle, 4);
	file.WriteNull(6);
	file.WriteMm<uint32_t>(groundDistance);
	file.WriteNull(5);
	file.Write<uint8_t>(thermal);
	file.WriteNull(2);
	file.Write<uint32_t>(thermalSize / size.out * 300.0f);
	file.Write<uint8_t>(through);
	file.Write<uint8_t>(soldermask);
	file.WriteNull(3);
	file.Write<uint32_t>(123456789); //WTF?
	file.WriteNull(15);
	file.WriteString(marker);

	SaveGroups(file);

	// It fill only half of points, another half
	// is symmetrical to this
	
	Vec2 points[4];
	uint8_t count = CalcPoints(points);

	if(count) {
		Vec2::Rotate(points, count, angle);
		WriteSymmetricalArray(file, points, count, position);
	}
}

void THTPad::LoadObject(File &file) {
	position.LoadPosition(file);
	size.Load(file);
	file.ReadNull(5);
	layer = file.Read<uint8_t>() - 1;
	shape = file.Read<uint8_t>() - 1;
	file.ReadNull(4);
	componentID = file.Read<uint16_t>();
	file.ReadNull(1);
	file.Read(thermalStyle, 4);
	file.ReadNull(6);
	groundDistance = file.ReadMm<uint32_t>();
	file.ReadNull(5);
	thermal = file.Read<uint8_t>();
	file.ReadNull(2);
	thermalSize = file.Read<uint32_t>() / 300.0f * size.out;
	through = file.Read<uint8_t>();
	soldermask = file.Read<uint8_t>();
	file.ReadNull(22);
	file.ReadString(marker);

	LoadGroups(file);

	Vec2 points[8];
	uint8_t count = ReadArray(file, points, position);

	switch(shape) {
		case S_CIRCLE:
		case S_CIRCLE_E:
		case S_CIRCLE_E + 3:
			angle = points[1].Angle();
			break;
		case S_OCTAGON:
		case S_OCTAGON_E:
			angle = Vec2::Mean(points[3], points[4]).Angle();
			break;
		case S_OCTAGON_E + 3:
			angle = Vec2::Mean(points[5], points[6]).Angle();
			break;
		case S_SQUARE:
		case S_SQUARE_E:
			angle = Vec2::Mean(points[1], points[2]).Angle();
			break;
		case S_SQUARE_E + 3:
			angle = Vec2::Mean(points[2], points[3]).Angle();
			break;
	}
	if(shape > S_SQUARE_E) {
		for(int i = 0; i < 4; i++)
			thermalStyle[i] = (thermalStyle[i] >> 2) | (thermalStyle[i] << 6);
		shape -= 3;
	}
}

void THTPad::Draw(float halfSize, float distance) const {
	switch(shape) {
	case S_CIRCLE:
		glutils::DrawCircle(position, halfSize + distance);
		break;
	case S_SQUARE:
		glPushMatrix();
		glutils::Translate(position);
		glutils::Rotate(angle);
		glutils::DrawRectangle(Vec2(halfSize + distance, halfSize + distance));
		glPopMatrix();
		break;
	case S_SQUARE_E:
		glPushMatrix();
		glutils::Translate(position);
		glutils::Rotate(angle);
		glutils::DrawRectangle(Vec2(halfSize * 2.0f + distance, halfSize + distance));
		glPopMatrix();
		break;
	case S_CIRCLE_E:
		glBegin(GL_TRIANGLE_FAN);
		glutils::Vertex(position);
		for(float i = angle + M_PI / 2.0f; i < angle + M_PI * 3.0f / 2.0f; i += glutils::stepAngle)
			glutils::Vertex(Vec2(i) * (halfSize + distance) + position - Vec2(angle) * halfSize);
		for(float i = angle + M_PI * 3.0f / 2.0f; i < angle + M_PI * 5.0f / 2.0f; i += glutils::stepAngle)
			glutils::Vertex(Vec2(i) * (halfSize + distance) + position + Vec2(angle) * halfSize);
		glutils::Vertex(Vec2(angle + M_PI / 2.0f) * (halfSize + distance) + position - Vec2(angle) * halfSize);
		glEnd();
		break;
	case S_OCTAGON:
		glPushMatrix();
		glutils::Translate(position);
		glutils::Rotate(angle);
		{
			float s1 = halfSize + distance;
			float s2 = s1 * 0.41421356; // tg(M_PI / 8)
			glBegin(GL_TRIANGLE_FAN);
			glutils::Vertex(Vec2(0.0f, 0.0f));
			glutils::Vertex(Vec2(-s1,  s2));
			glutils::Vertex(Vec2(-s2,  s1));
			glutils::Vertex(Vec2( s2,  s1));
			glutils::Vertex(Vec2( s1,  s2));
			glutils::Vertex(Vec2( s1, -s2));
			glutils::Vertex(Vec2( s2, -s1));
			glutils::Vertex(Vec2(-s2, -s1));
			glutils::Vertex(Vec2(-s1, -s2));
			glutils::Vertex(Vec2(-s1,  s2));
			glEnd();
		}
		glPopMatrix();
		break;
	case S_OCTAGON_E:
		glPushMatrix();
		glutils::Translate(position);
		glutils::Rotate(angle);
		{
			float s1 = halfSize + distance;
			float s2 = s1 * 0.41421356; // tg(M_PI / 8)
			float s3 = halfSize * 2.0f + distance;
			float s4 = halfSize + s2;
			glBegin(GL_TRIANGLE_FAN);
			glutils::Vertex(Vec2(0.0f, 0.0f));
			glutils::Vertex(Vec2(-s3,  s2));
			glutils::Vertex(Vec2(-s4,  s1));
			glutils::Vertex(Vec2( s4,  s1));
			glutils::Vertex(Vec2( s3,  s2));
			glutils::Vertex(Vec2( s3, -s2));
			glutils::Vertex(Vec2( s4, -s1));
			glutils::Vertex(Vec2(-s4, -s1));
			glutils::Vertex(Vec2(-s3, -s2));
			glutils::Vertex(Vec2(-s3,  s2));
			glEnd();
		}
		glPopMatrix();
		break;
	}
}

void THTPad::DrawGroundDistance() const {
	Draw(size.out / 2.0f, groundDistance);
}
void THTPad::DrawObject() const {
	Draw(size.out / 2.0f, 0.0f);
}
void THTPad::DrawDrillings() const {
	glutils::DrawCircle(position, size.in / 2.0f);
}

uint8_t THTPad::CalcPoints(Vec2 *points) const {
	float radius = size.out * 0.5f;
	float halfa = radius * 0.41421356; // tg(M_PI / 8)

	switch(shape) {
		case S_CIRCLE:
		case S_CIRCLE_E:
			points[0].Set(-radius, 0);
			return 1;
		case S_SQUARE:
			points[0].Set(-radius, radius);
			points[1].Set( radius, radius);
			return 2;
		case S_SQUARE_E:
			points[0].Set(-size.out, radius);
			points[1].Set( size.out, radius);
			return 2;
		case S_OCTAGON:
			points[0].Set(-radius, halfa);
			points[1].Set(-halfa, radius);
			points[2].Set( halfa, radius);
			points[3].Set( radius, halfa);
			return 4;
		case S_OCTAGON_E:
			points[0].Set(-size.out, halfa);
			points[1].Set(-radius - halfa, radius);
			points[2].Set( radius + halfa, radius);
			points[3].Set( size.out, halfa);
			return 4;
	}
	return 0;
}
