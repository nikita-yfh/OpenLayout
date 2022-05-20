#include "Pad.h"

template<typename T>
static inline void Swap(T &a, T &b) {
	T c = a;
	a = b;
	b = c;
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
void Connections::Add(Object *object) {
	if(Find(object))
		return;
	connections = (Object**)realloc(connections, (count + 1) * sizeof(Object*));
	connections[count++] = object;
}
void Connections::Remove(const Object *object) {
	Object **i = Find(object);
	if(i) {
		count--;
		*i = connections[count]; // Move last item to deleted
		connections = (Object**)realloc(connections, count * sizeof(Object*));
	}
}
Object **Connections::Find(const Object *object) {
	for(Object **i = connections; *i; i++)
		if(*i == object)
			return i;
	return nullptr;
}
bool Connections::Has(const Object *object) const {
	for(Object **i = connections; *i; i++)
		if(*i == object)
			return true;
	return false;
}
void Connections::Save(const Object *objects, File &file) const {
	file.Write<uint32_t>(count);
	if(count) {
		for(Object **i = connections; *i; i++) {
			uint32_t index;
			for(const Object *j = objects; j; j = j->GetNext(), index++)
				if(j == *i) {
					file.Write<uint32_t>(index);
					break;
				}
		}
	}
}
void Connections::Load(Object *objects, File &file) {
	this->~Connections();
	count = file.Read<uint32_t>();
	if(count) {
		connections = (Object**)malloc(count * sizeof(Object*));
		for(int i = 0; i < count; i++) {
			int index = file.Read<uint32_t>();
			connections[i] = objects;
			while(index) {
				connections[i] = connections[i]->GetNext();
				index--;
			}
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
		(points[i] - shift).Save(file);
}
uint32_t Pad::ReadArray(File &file, Vec2 *points, const Vec2 &shift) {
	uint32_t count = file.Read<uint32_t>();
	for(int i = 0; i < count; i++) {
		points[i].Load(file);
		points[i] -= shift;
	}
	bool wrap = false;
	if(count >= 3) {
		if(!Angle::Clockwise(points[0], points[1], points[2]))
			for(int i = 0; i < count / 2; i++)
				Swap(points[i], points[count - i - 1]);
	}
	return count;
}

void Pad::WriteSymmetricalArray(File &file, const Vec2 *points, uint32_t count, const Vec2 &shift) {
	file.Write<uint32_t>(count * 2);
	for(int i = 0; i < count * 2; i++) {
		if(i < count)
			(points[i] + shift).Save(file);
		else
			(shift - points[(i+count) % count]).Save(file);
	}
}
