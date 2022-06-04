#include "Arc.h"
#include "GLUtils.h"
#include "Utils.h"

AABB Arc::GetAABB() const {
	Vec2 radius((diameter + width) * 0.5f, (diameter + width) * 0.5f);
	return AABB(position - radius, position + radius);
}

bool Arc::TestPoint(const Vec2 &point) const {
	if(utils::PointInCircle(point, position, (diameter + width) * 0.5f) &&
		!utils::PointInCircle(point, position, (diameter - width) * 0.5f))
		return true;
	if(utils::PointInCircle(point, position + Vec2(-beginAngle) * diameter * 0.5f, width * 0.5f))
		return true;
	if(utils::PointInCircle(point, position + Vec2(-endAngle) * diameter * 0.5f, width * 0.5f))
		return true;
	return false;
}

void Arc::SaveObject(File &file) const {
	file.Write<uint8_t>(ARC);
	position.SavePosition(file);
	file.WriteMm<float>((diameter + width) / 2.0f);
	file.WriteMm<float>((diameter - width) / 2.0f);
	file.WriteAngle(endAngle);
	file.WriteNull(1);
	file.Write<uint8_t>(layer + 1);
	file.WriteNull(5);
	file.Write<uint16_t>(componentID);
	file.WriteNull(1);
	file.WriteAngle(beginAngle);
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
	position.LoadPosition(file);
	float outRadius = file.ReadMm<float>();
	float inRadius = file.ReadMm<float>();
	diameter = outRadius + inRadius;
	width = outRadius - inRadius;
	endAngle = file.ReadAngle();
	file.ReadNull(1);
	layer = file.Read<uint8_t>() - 1;
	file.ReadNull(5);
	componentID = file.Read<uint16_t>();
	file.ReadNull(1);
	beginAngle = file.ReadAngle();
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

void Arc::Draw(float halfwidth) const {
	float radius = diameter * 0.5f;
	float end = endAngle;
	if(endAngle <= beginAngle)
		end += M_PI * 2.0f;

	glBegin(GL_TRIANGLE_STRIP);
	for(float i = beginAngle; i < end; i += (end - beginAngle) / (glutils::numSegments)) {
		Vec2 vec(-i);
		glutils::Vertex(position + vec * (radius - halfwidth));
		glutils::Vertex(position + vec * (radius + halfwidth));
	}
	glutils::Vertex(position + Vec2(-end) * (radius - halfwidth));
	glutils::Vertex(position + Vec2(-end) * (radius + halfwidth));
	glEnd();
	if(beginAngle != endAngle) {
		glutils::DrawSector(position + Vec2(-beginAngle) * radius, halfwidth, beginAngle - M_PI, beginAngle);
		glutils::DrawSector(position + Vec2(-endAngle)   * radius, halfwidth, endAngle, endAngle + M_PI);
	}
}

void Arc::DrawObject() const {
	if(!cutoff)
		Draw(width / 2.0f);
}

void Arc::DrawGroundDistance() const {
	if(cutoff)
		Draw(width / 2.0f);
	else
		Draw(width / 2.0f + groundDistance);
}

