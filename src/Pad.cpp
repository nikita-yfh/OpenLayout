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

Connections::Connections() {
	connections = nullptr;
	count = 0;
}
Connections::~Connections() {
	if(connections)
		free(connections);
	count = 0;
}
void Connections::Add(Pad *object) {
	if(Find(object))
		return;
	connections = (Pad**) realloc(connections, (count + 1) * sizeof(Pad*));
	connections[count++] = object;
}
void Connections::Remove(const Pad *object) {
	Pad **i = Find(object);
	if(i) {
		count--;
		*i = connections[count]; // Move last item to deleted
		connections = (Pad**) realloc(connections, count * sizeof(Pad*));
	}
}
Pad **Connections::Find(const Pad *object) {
	for(Pad **i = connections; *i; i++)
		if(*i == object)
			return i;
	return nullptr;
}
bool Connections::Has(const Pad *object) const {
	for(Pad **i = connections; *i; i++)
		if(*i == object)
			return true;
	return false;
}
void Connections::Save(const Object *objects, File &file) const {
	file.Write<uint32_t>(count);
	if(count) {
		for(Pad **i = connections; *i; i++) {
			uint32_t index;
			for(const Object *j = objects; j; j = j->GetNext(), index++) {
				if(j == *i) {
					file.Write<uint32_t>(index);
					break;
				}
			}
		}
	}
}
void Connections::Load(Object *objects, File &file) {
	this->~Connections();
	count = file.Read<uint32_t>();
	if(count) {
		connections = (Pad**) malloc(count * sizeof(Pad*));
		for(int i = 0; i < count; i++) {
			int index = file.Read<uint32_t>();
			Object *object = objects;
			while(index--)
				object = object->GetNext();
			connections[i] = (Pad*) object;
		}
	}
}

void Pad::SaveConnections(const Object *objects, File &file) const {
	connections.Save(objects, file);
}
void Pad::LoadConnections(Object *objects, File &file) {
	connections.Load(objects, file);
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
	for(int i = 0; i < connections.Count(); i++) {
		const Pad *pad = connections[i];
		if(pad > this) { // will draw connection once
			glutils::Vertex(position);
			glutils::Vertex(pad->position);
		}
	}
}

