#include "THTPad.h"

AABB THTPad::GetAABB() const {
	Vec2 radius(outDiameter * 0.5f, outDiameter * 0.5f);
	return AABB(position - radius, position + radius);
}

void THTPad::SaveObject(File &file) const {
	file.Write<uint8_t>(THT_PAD);
	position.Save<float>(file);
	file.WriteMm<float>(outDiameter / 2.0f);
	file.WriteMm<float>(inDiameter / 2.0f);
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
	file.Write<uint32_t>(thermalSize / outDiameter * 300.0f);
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

	float radius = outDiameter * 0.5f;
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
			points[0].Set(-outDiameter, radius);
			points[1].Set( outDiameter, radius);
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
			points[0].Set(-outDiameter, halfa);
			points[1].Set(-radius - halfa, radius);
			points[2].Set( radius + halfa, radius);
			points[3].Set( outDiameter, halfa);
			count = 4;
			break;
	}

	if(count) {
		Vec2::Rotate(points, count, angle);
		WriteSymmetricalArray(file, points, count, position);
	}
}

void THTPad::LoadObject(File &file) {
	position.Load<float>(file);
	outDiameter = file.ReadMm<float>() * 2.0f;
	inDiameter = file.ReadMm<float>() * 2.0f;
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
	thermalSize = file.Read<uint32_t>() / 300.0f * outDiameter;
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
