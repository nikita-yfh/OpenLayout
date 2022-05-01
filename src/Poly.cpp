#include "Poly.h"

AABB Poly::GetAABB() const {
	Vec2 min( 1000000.0f,  1000000.0f);
	Vec2 max(-1000000.0f, -1000000.0f);

	for(int i = 0; i < count; i++) {
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

void Poly::SaveObject(File &file) const {
	file.Write<uint8_t>(Object::POLY);
	file.WriteNull(16);
	file.WriteMm<uint32_t>(width);
	file.WriteNull(1);
	file.Write<uint8_t>(layer);
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

	file.Write<uint32_t>(0);

	SavePoints(file, onlySoldermask);
}


void Poly::LoadObject(File &file) {
	file.ReadNull(16);
	width = file.ReadMm<uint32_t>();
	file.ReadNull(1);
	layer = file.Read<uint8_t>();
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

	file.Read<uint32_t>();

	LoadPoints(file, onlySoldermask);
}


