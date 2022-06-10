#include "THTPad.h"
#include "GLUtils.h"
#include "Utils.h"

THTPad::THTPad(uint8_t layer, const Vec2 &position, const PadSize &_size) : Pad(layer, position) {
	size = _size;
	shape = 0;
	through = false;
}

AABB THTPad::GetAABB() const {
	Vec2 maxRadius(size.out * 2.5f, size.out * 2.5f);
	return AABB(position - maxRadius, position + maxRadius);
}

bool THTPad::TestPoint(const Vec2 &point) const {
	float radius = size.out * 0.5f;
	Vec2 _point = (point - position).Rotate(-angle);
	switch(shape) {
	case CIRCLE:
		return utils::PointInCircle(point, position, radius);
	case CIRCLE_E:
		if(utils::PointInCircle(_point, Vec2(-radius, 0.0f), radius))
			return true;
		if(utils::PointInCircle(_point, Vec2( radius, 0.0f), radius))
			return true;
	case SQUARE:
		return abs(_point.x) < radius && abs(_point.y) < radius;
	case SQUARE_E:
		return abs(_point.x) < size.out && abs(_point.y) < radius;
	case OCTAGON:
		if(abs(_point.x) > radius || abs(_point.y) > radius)
			return false;
		return abs(_point.x) + abs(_point.y) < radius * sqrtf(2.0f);
	case OCTAGON_E:
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
	file.Write<uint8_t>(shape);
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
	int count = 0;

	float radius = size.out * 0.5f;
	float halfa = radius * 0.41421356; // tg(M_PI / 8)

	switch(shape) {
		case CIRCLE:
		case CIRCLE_E:
			points[0].Set(-radius, 0);
			count = 1;
			break;
		case SQUARE:
			points[0].Set(-radius, radius);
			points[1].Set( radius, radius);
			count = 2;
			break;
		case SQUARE_E:
			points[0].Set(-size.out, radius);
			points[1].Set( size.out, radius);
			count = 2;
			break;
		case OCTAGON:
			points[0].Set(-radius, halfa);
			points[1].Set(-halfa, radius);
			points[2].Set( halfa, radius);
			points[3].Set( radius, halfa);
			count = 4;
			break;
		case OCTAGON_E:
			points[0].Set(-size.out, halfa);
			points[1].Set(-radius - halfa, radius);
			points[2].Set( radius + halfa, radius);
			points[3].Set( size.out, halfa);
			count = 4;
			break;
	}

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
	shape = file.Read<uint8_t>();
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
		case CIRCLE:
		case CIRCLE_E:
		case CIRCLE_E + 3:
			angle = points[1].Angle();
			break;
		case OCTAGON:
		case OCTAGON_E:
			angle = Vec2::Mean(points[3], points[4]).Angle();
			break;
		case OCTAGON_E + 3:
			angle = Vec2::Mean(points[5], points[6]).Angle();
			break;
		case SQUARE:
		case SQUARE_E:
			angle = Vec2::Mean(points[1], points[2]).Angle();
			break;
		case SQUARE_E + 3:
			angle = Vec2::Mean(points[2], points[3]).Angle();
			break;
	}
	if(shape > SQUARE_E) {
		for(int i = 0; i < 4; i++)
			thermalStyle[i] = (thermalStyle[i] >> 2) | (thermalStyle[i] << 6);
		shape -= 3;
	}
}

void THTPad::Draw(float halfSize, float distance) const {
	switch(shape) {
	case CIRCLE:
		glutils::DrawCircle(position, halfSize + distance);
		break;
	case SQUARE:
		glPushMatrix();
		glutils::Translate(position);
		glutils::Rotate(angle);
		glutils::DrawRectangle(Vec2(halfSize + distance, halfSize + distance));
		glPopMatrix();
		break;
	case SQUARE_E:
		glPushMatrix();
		glutils::Translate(position);
		glutils::Rotate(angle);
		glutils::DrawRectangle(Vec2(halfSize * 2.0f + distance, halfSize + distance));
		glPopMatrix();
		break;
	case CIRCLE_E:
		glBegin(GL_TRIANGLE_FAN);
		glutils::Vertex(position);
		for(float i = angle + M_PI / 2.0f; i < angle + M_PI * 3.0f / 2.0f; i += glutils::stepAngle)
			glutils::Vertex(Vec2(i) * (halfSize + distance) + position - Vec2(angle) * halfSize);
		for(float i = angle + M_PI * 3.0f / 2.0f; i < angle + M_PI * 5.0f / 2.0f; i += glutils::stepAngle)
			glutils::Vertex(Vec2(i) * (halfSize + distance) + position + Vec2(angle) * halfSize);
		glutils::Vertex(Vec2(angle + M_PI / 2.0f) * (halfSize + distance) + position - Vec2(angle) * halfSize);
		glEnd();
		break;
	case OCTAGON:
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
	case OCTAGON_E:
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

