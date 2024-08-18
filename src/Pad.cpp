#include "Pad.h"
#include "GLUtils.h"
#include "Utils.h"

Pad::~Pad() {
	for(int i = 0; i < connections.Size(); i++) {
		Pad *connected = connections[i];
		connected->connections.Remove(this);
	}
}

Vec2 Pad::GetPosition() const {
	return position;
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
	if(count >= 3 && !utils::Clockwise(points[0].Angle(), points[1].Angle(), points[2].Angle()))
		for(int i = 0; i < count / 2; i++)
			utils::Swap(points[i], points[count - i - 1]);
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
		if(pad > this && !(IsSelected() && pad->IsSelected())) { // will draw connection once
			glutils::Vertex(position);
			glutils::Vertex(pad->position);
		}
	}
}

void Pad::DrawConnectionsSelected() const {
    if(!IsSelected())
        return;
	for(int i = 0; i < connections.Size(); i++) {
		const Pad *pad = connections[i];
		if(pad > this && pad->IsSelected()) { // will draw connection once
			glutils::Vertex(position);
			glutils::Vertex(pad->position);
		}
	}
}

void Pad::UpdateConnections(Object *objects) {
	for(int i = 0; i < connections.Size(); i++) {
		const Object *newConnection = objects;
		const Object *oldConnection = connections[i];
		while(oldConnection->GetPrev()) {
			oldConnection = oldConnection->GetPrev();
			newConnection = newConnection->GetNext();
		}
		connections[i] = (Pad*) newConnection;
	}
}

Object *Pad::TestConnections(const Vec2 &mousePos, float radius) const {
    for(int i = 0; i < connections.Size(); i++)
        if(Vec2::DistanceSegmentToPoint(position, connections[i]->position, mousePos) < radius)
            return connections[i];
    return nullptr;
}

void Pad::RemoveConnections(Object *object) {
    if(connections.Remove(static_cast<Pad*>(object)))
        object->RemoveConnections(this);
}

void Pad::AddConnection(Pad *object) {
    if(!connections.Remove(object))
        connections.Add(object);
}

void Pad::Rotate(const Vec2 &center, float delta) {
	position = position.Rotate(delta, center);
	angle += delta;
}

void Pad::MirrorHorizontal(float x) {
	position.MirrorHorizontal(x);
	utils::MirrorAngle(angle, M_PI / 2.0f);
}

void Pad::MirrorVertical(float y) {
	position.MirrorVertical(y);
	utils::MirrorAngle(angle, M_PI);
}

void Pad::Move(const Vec2 &d) {
	position += d;
}
