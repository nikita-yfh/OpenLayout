#include "Track.h"

AABB Track::GetAABB() const {
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

void Track::SaveObject(File &file) const {
	file.Write<uint8_t>(Object::TRACK);
	file.WriteNull(16);
	file.WriteMm<uint32_t>(width);
	file.WriteNull(1);
	file.Write<uint8_t>(layer + 1);
	file.Write<uint8_t>(style);
	file.WriteNull(4);
	file.Write<uint16_t>(componentID);
	file.WriteNull(11);
	file.WriteMm<uint32_t>(groundDistance);
	file.WriteNull(7);
	file.Write<uint8_t>(cutoff);
	file.WriteNull(5);
	file.Write<uint8_t>(soldermask);
	file.WriteNull(22);
	file.WriteString(marker);

	groups.Save(file);

	SavePoints(file);
}


void Track::LoadObject(File &file) {
	file.ReadNull(16);
	width = file.ReadMm<uint32_t>();
	file.ReadNull(1);
	layer = file.Read<uint8_t>() - 1;
	style = file.Read<uint8_t>();
	file.ReadNull(4);
	componentID = file.Read<uint16_t>();
	file.ReadNull(11);
	groundDistance = file.ReadMm<uint32_t>();
	file.ReadNull(7);
	cutoff = file.Read<uint8_t>();
	file.ReadNull(5);
	soldermask = file.Read<uint8_t>();
	file.ReadNull(22);
	file.ReadString(marker);

	groups.Load(file);

	LoadPoints(file);
}


