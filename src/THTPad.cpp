#include "THTPad.h"
#include "GLUtils.h"

AABB THTPad::GetAABB() const {
	Vec2 radius(size.out * 0.5f, size.out * 0.5f);
	return AABB(position - radius, position + radius);
}

void THTPad::SaveObject(File &file) const {
	file.Write<uint8_t>(THT_PAD);
	position.Save(file);
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

	groups.Save(file);

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
		angle.Rotate(points, count);
		WriteSymmetricalArray(file, points, count, position);
	}
}

void THTPad::LoadObject(File &file) {
	position.Load(file);
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

	groups.Load(file);

	Vec2 points[8];
	uint8_t count = ReadArray(file, points, position);

	switch(shape) {
		case CIRCLE:
		case CIRCLE_E:
		case CIRCLE_E + 3:
			angle = Angle(points[1]);
			break;
		case OCTAGON:
		case OCTAGON_E:
			angle = Angle(Vec2::Mean(points[3], points[4]));
			break;
		case OCTAGON_E + 3:
			angle = Angle(Vec2::Mean(points[5], points[6]));
			break;
		case SQUARE:
		case SQUARE_E:
			angle = Angle(Vec2::Mean(points[1], points[2]));
			break;
		case SQUARE_E + 3:
			angle = Angle(Vec2::Mean(points[2], points[3]));
			break;
	}
	if(shape > SQUARE_E) {
		for(int i = 0; i < 4; i++)
			thermalStyle[i] = (thermalStyle[i] >> 2) | (thermalStyle[i] << 6);
		shape -= 3;
	}
}

void THTPad::DrawGroundDistance() const {
	glutils::Translate(position);
	glutils::DrawCircle(size.out / 2.0f + groundDistance);
}
void THTPad::DrawObject() const {
	glutils::Translate(position);
	glutils::DrawCircle(size.out / 2.0f);
}
void THTPad::DrawDrillings() const {
	glutils::Translate(position);
	glutils::DrawCircle(size.in / 2.0f);
}

