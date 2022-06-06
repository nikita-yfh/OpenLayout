#include "Track.h"
#include "GLUtils.h"
#include "Utils.h"

AABB Track::GetAABB() const {
	Vec2 max(0.0f, 0.0f);
	Vec2 min(500.0f, 500.0f);

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

bool Track::TestPoint(const Vec2 &point) const {
	float halfWidth = width / 2.0f;
	for(int i = 0; i < count - 1; i++)
		if(utils::PointInPolySegment(point, points[i], points[i + 1], halfWidth))
			return true;
	for(int i = GetBeginStyle(); i < count - GetEndStyle(); i++)
		if(utils::PointInCircle(point, points[i], width / 2.0f))
			return true;
	return false;
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

void Track::Draw(float halfWidth) const {
	if(halfWidth == 0.0f) {
		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < count; i++)
			glutils::Vertex(points[i]);
		glEnd();
	} else {
		Vec2 normals[count - 1];
		for(int i = 0; i < count - 1; i++)
			normals[i] = (points[i + 1] - points[i]).Normal(halfWidth);

		Vec2 pathBegin1 = points[0] - normals[0];
		Vec2 pathBegin2 = points[0] + normals[0];
		Vec2 pathEnd1 = points[count - 1] - normals[count - 2];
		Vec2 pathEnd2 = points[count - 1] + normals[count - 2];

		if(GetBeginStyle()) {
			Vec2 delta = Vec2(points[1] - points[0], halfWidth);
			pathBegin1 -= delta;
			pathBegin2 -= delta;
		} else {
			glutils::DrawTriangleFan(points[0], points[0],
				points[0] + normals[0], points[0] - normals[0], true);
		}
		if(GetEndStyle()) {
			Vec2 delta = Vec2(points[count - 1] - points[count - 2], halfWidth);
			pathEnd1 -= delta;
			pathEnd2 -= delta;
		} else {
			glutils::DrawTriangleFan(points[count - 1], points[count - 1],
				points[count - 1] + normals[count - 2], points[count - 1] - normals[count - 2], false);
		}

		Vec2 intersections[count - 2];
		bool clockwise[count - 2];

		for(int i = 0; i < count - 2; i++) {
			const Vec2 &a = points[i];
			const Vec2 &b = points[i + 1];
			const Vec2 &c = points[i + 2];

			Vec2 n1 = (b - a).Normal(halfWidth);
			Vec2 n2 = (c - b).Normal(halfWidth);

			float orientation = utils::Orientation(a, b, c);
			clockwise[i] = orientation >= 0.0f;

			if(orientation > 0.0f) {
				intersections[i] = utils::Intersection(a - n1, b - n1, b - n2, c - n2);
				glutils::DrawTriangleFan(intersections[i], b, b + n2, b + n1, true);
			} else if(orientation < 0.0f) {
				intersections[i] = utils::Intersection(a + n1, b + n1, b + n2, c + n2);
				glutils::DrawTriangleFan(intersections[i], b, b - n1, b - n2, true);
			} else {
				intersections[i] = Vec2::Invalid();
				glutils::DrawTriangleFan(b, b, b - n1, b - n2, true);
			}
		}

		glBegin(GL_TRIANGLES);
		for(int i = 0; i < count - 1; i++) {
			Vec2 begin1 = pathBegin1;
			Vec2 begin2 = pathBegin2;
			Vec2 end1;
			Vec2 end2;
			
			if(i == count - 2) {
				end1 = pathEnd1;
				end2 = pathEnd2;
			} else if (intersections[i].IsValid()) {
				if(clockwise[i]) {
					end1 = pathBegin1 = intersections[i];
					end2 = points[i + 1] + normals[i];
					pathBegin2 = points[i + 1] + normals[i + 1];
				} else {
					end1 = points[i + 1] - normals[i];
					end2 = pathBegin2 = intersections[i];
					pathBegin1 = points[i + 1] - normals[i + 1];
				}
			} else {
				end1 = points[i + 1] - normals[i];
				end2 = points[i + 1] + normals[i];
				pathBegin1 = points[i + 1] - normals[i + 1];
				pathBegin2 = points[i + 1] + normals[i + 1];
			}

			glutils::Vertex(begin1);
			glutils::Vertex(begin2);
			glutils::Vertex(end1);
			glutils::Vertex(end1);
			glutils::Vertex(begin2);
			glutils::Vertex(end2);
		}
			
		glEnd();
	}
}

void Track::DrawGroundDistance() const {
	if(cutoff)
		Draw(width / 2.0f);
	else
		Draw(width / 2.0f + groundDistance);
}

void Track::DrawObject() const {
	if(!cutoff)
		Draw(width / 2.0f);
}

