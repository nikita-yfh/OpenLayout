#include "SMDPad.h"

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
	position.Save<float>(file);
	size.Save<float>(file);
	file.WriteNull(5);
	file.Write<uint8_t>(layer);
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

	file.Write<uint32_t>(0);

	Vec2 points[2] = {
		(Vec2(-size.x, size.y) * 0.5f).Rotate(angle),
		(Vec2( size.x, size.y) * 0.5f).Rotate(angle)
	};

	WriteSymmetricalArray(file, points, 2, position);
}

void SMDPad::LoadObject(File &file) {
	position.Load<float>(file);
	size.Load<float>(file);
	file.ReadNull(5);
	layer = file.Read<uint8_t>();
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

	file.Read<uint32_t>();

	Vec2 points[4];
	ReadArray(file, points, position);

	Vec2 dx = points[1] - points[0];
	Vec2 dy = points[1] - points[2];

	angle = dx.Angle();
	size.Set(dx.Length(), dy.Length());
}
