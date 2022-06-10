#include "Pad.h"
#include "GLUtils.h"

template<typename T>
static inline void Swap(T &a, T &b) {
	T c = a;
	a = b;
	b = c;
}

static bool Clockwise(float a, float b, float c) {
	if(c > a)
		return (b < a || b > c);
	else 
		return (b < a && b > c);
}

Pad::Pad(uint8_t layer, const Vec2 &_position) : Object(layer) {
	position = _position;
	thermal = false;
	memset(thermalStyle, 0x55, 4);
	thermalSize = 0.4f;
	angle = 0.0f;
}

void Pad::SaveConnections(const Object *objects, File &file) const {
	file.Write<uint32_t>(connections.Size());
	for(int i = 0; i < connections.Size(); i++) {
		uint32_t index = 0;
		for(const Object *j = objects; j; j = j->GetNext(), index++) {
			if(j == connections[i]) {
				file.Write<uint32_t>(index);
				break;
			}
		}
	}
}
void Pad::LoadConnections(Object *objects, File &file) {
	uint32_t count = file.Read<uint32_t>();
	if(count) {
		connections.Init(count);
		for(int i = 0; i < count; i++) {
			int index = file.Read<uint32_t>();
			Object *object = objects;
			while(index--)
				object = object->GetNext();
			connections[i] = (Pad*) object;
		}
	}
}

void Pad::WriteArray(File &file, const Vec2 *points, uint32_t count, const Vec2 &shift) {
	file.Write<uint32_t>(count);
	for(int i = 0; i < count; i++)
		(points[i] - shift).SavePosition(file);
}
uint32_t Pad::ReadArray(File &file, Vec2 *points, const Vec2 &shift) {
	uint32_t count = file.Read<uint32_t>();
	for(int i = 0; i < count; i++) {
		points[i].LoadPosition(file);
		points[i] -= shift;
	}
	bool wrap = false;
	if(count >= 3 && !Clockwise(points[0].Angle(), points[1].Angle(), points[2].Angle()))
		for(int i = 0; i < count / 2; i++)
			Swap(points[i], points[count - i - 1]);
	return count;
}

void Pad::WriteSymmetricalArray(File &file, const Vec2 *points, uint32_t count, const Vec2 &shift) {
	file.Write<uint32_t>(count * 2);
	for(int i = 0; i < count * 2; i++) {
		if(i < count)
			(points[i] + shift).SavePosition(file);
		else
			(shift - points[(i+count) % count]).SavePosition(file);
	}
}

void Pad::DrawConnections() const {
	for(int i = 0; i < connections.Size(); i++) {
		const Pad *pad = connections[i];
		if(pad > this) { // will draw connection once
			glutils::Vertex(position);
			glutils::Vertex(pad->position);
		}
	}
}

