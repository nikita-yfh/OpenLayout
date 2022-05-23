#include "SMDPad.h"
#include "GLUtils.h"

template<typename T>
static inline T min(T a, T b) {
	if(a > b)
		return b;
	return a;
}

AABB SMDPad::GetAABB() const {
	Vec2 halfSize(size * 0.5f);
	return AABB(position - halfSize, position + halfSize);
}

void SMDPad::SaveObject(File &file) const {
	file.Write<uint8_t>(SMD_PAD);
	position.SavePosition(file);
	size.Save(file);
	file.WriteNull(5);
	file.Write<uint8_t>(layer + 1);
	file.WriteNull(5);
	file.Write<uint16_t>(componentID);
	file.WriteNull(1);
	file.Write(thermalStyle, 4);
	file.WriteNull(6);
	file.WriteMm<uint32_t>(groundDistance);
	file.WriteNull(5);
	file.Write<uint8_t>(thermal);
	file.WriteNull(2);
	file.Write<uint32_t>(thermalSize / min(size.x, size.y) * 300.0f);
	file.WriteNull(1);
	file.Write<uint8_t>(soldermask);
	file.WriteNull(3);
	file.Write<uint32_t>(123456789); //WTF?
	file.WriteNull(15);
	file.WriteString(marker);

	groups.Save(file);

	Vec2 points[2] = {
		Vec2(-size.x, size.y).Rotate(angle) * 0.5f,
		Vec2( size.x, size.y).Rotate(angle) * 0.5f
	};

	WriteSymmetricalArray(file, points, 2, position);
}

void SMDPad::LoadObject(File &file) {
	file.ReadNull(8);
	size.Load(file);
	file.ReadNull(5);
	layer = file.Read<uint8_t>() - 1;
	file.ReadNull(5);
	componentID = file.Read<uint16_t>();
	file.ReadNull(1);
	file.Read(thermalStyle, 4);
	file.ReadNull(6);
	groundDistance = file.ReadMm<uint32_t>();
	file.ReadNull(5);
	thermal = file.Read<uint8_t>();
	file.ReadNull(2);
	thermalSize = file.Read<uint32_t>() / 300.0f * min(size.x, size.y);
	file.ReadNull(1);
	soldermask = file.Read<uint8_t>();
	file.ReadNull(22);
	file.ReadString(marker);

	groups.Load(file);

	Vec2 points[4];
	ReadArray(file, points, position);

	Vec2 dx = points[1] - points[0];
	angle = dx.Angle();
	position = Vec2::Mean(points[0], points[2]);
}

void SMDPad::DrawGroundDistance() const {
	glPushMatrix();
	glutils::Translate(position);
	glutils::Rotate(angle);
	glutils::DrawRectangle(size * 0.5f + Vec2(groundDistance, groundDistance));
	glPopMatrix();
}

void SMDPad::DrawObject() const {
	glPushMatrix();
	glutils::Translate(position);
	glutils::Rotate(angle);
	glutils::DrawRectangle(size * 0.5f);
	glPopMatrix();
}

