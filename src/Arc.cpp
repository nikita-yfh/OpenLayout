#include "Arc.h"

AABB Arc::GetAABB() const {
	Vec2 radius((diameter + width) * 0.5f, (diameter + width) * 0.5f);
	return AABB(position - radius, position + radius);
}

void Arc::SaveObject(File &file) const {
	file.Write<uint8_t>(CIRCLE);
	position.Save<float>(file);
	file.WriteMm<float>((diameter + width) / 2.0f);
	file.WriteMm<float>((diameter - width) / 2.0f);
	file.WriteAngle<uint32_t>(endAngle);
	file.WriteNull(1);
	file.Write<uint8_t>(layer + 1);
	file.WriteNull(5);
	file.Write<uint16_t>(componentID);
	file.WriteNull(1);
	file.WriteAngle<uint32_t>(beginAngle);
	file.WriteNull(5);
	file.Write<uint8_t>(fill);
	file.WriteMm<uint32_t>(groundDistance);
	file.WriteNull(7);
	file.Write<uint8_t>(cutoff);
	file.WriteNull(5);
	file.Write<uint8_t>(soldermask);
	file.WriteNull(22);
	file.WriteString(marker);

	groups.Save(file);
}

void Arc::LoadObject(File &file) {
	position.Load<float>(file);
	float outRadius = file.ReadMm<float>();
	float inRadius = file.ReadMm<float>();
	diameter = outRadius + inRadius;
	width = outRadius - inRadius;
	endAngle = file.ReadAngle<uint32_t>();
	file.ReadNull(1);
	layer = file.Read<uint8_t>() - 1;
	file.ReadNull(5);
	componentID = file.Read<uint16_t>();
	file.ReadNull(1);
	beginAngle = file.ReadAngle<uint32_t>();
	file.ReadNull(5);
	fill = file.Read<uint8_t>();
	groundDistance = file.ReadMm<uint32_t>();
	file.ReadNull(7);
	cutoff = file.Read<uint8_t>();
	file.ReadNull(5);
	soldermask = file.Read<uint8_t>();
	file.ReadNull(22);
	file.ReadString(marker);

	groups.Load(file);
}
