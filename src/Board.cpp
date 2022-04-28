#include "Board.h"

Board::Board(Type type, Vec2 innerSize, float border) {
	if(type != Type::Empty)
		size.Set(innerSize.x + border * 2.0f, innerSize.y + border * 2.0f);
	else
		size = innerSize;

	objects = nullptr;
	next = nullptr;
}

Board::Board() {
	objects = nullptr;
	next = nullptr;
}

Board::~Board() {
	while(objects) {
		Object *next = objects->next;
		delete objects;
		objects = next;
	}
}

uint32_t Board::GetObjectCount() const {
	uint32_t n = 0;
	Object *tmp = objects;
	while(tmp) {
		n++;
		tmp = tmp->GetNext();
	}
	return n;
}

void Board::Save(File &file) const {
	file.WriteString(name, 30);
	file.WriteNull(4);
	size.Save<int>(file);
	file.Write(groundPane, 7);
	file.Write<double>(grid);
	file.Write<double>(zoom);
	camera.Save<int>(file);
	file.Write<uint32_t>(activeLayer);
	file.Write(layerVisible, 7);
	images.Save(file);
	file.WriteNull(8);
	anchor.Save<int>(file);
	file.Write<uint8_t>(isMultilayer);

	file.Write<uint32_t>(GetObjectCount());
	for(Object *object = objects; object; object = object->GetNext())
		object->Save(file);
	for(Object *object = objects; object; object = object->GetNext())
		object->SaveConnections(objects, file);
}


void Board::Load(File &file) {
	file.ReadString(name, 30);
	file.ReadNull(4);
	size.Load<int>(file);
	file.Read(groundPane, 7);
	grid = file.Read<double>();
	zoom = file.Read<double>();
	camera.Load<int>(file);
	activeLayer = file.Read<uint32_t>();
	file.Read(layerVisible, 7);
	images.Load(file);
	file.ReadNull(8);
	anchor.Load<int>(file);
	isMultilayer = file.Read<uint8_t>();

	uint32_t objectCount = file.Read<uint32_t>();
	for(int i = 0; i < objectCount; i++)
		AddObject(Object::Load(file));
	for(Object *object = objects; object; object = object->GetNext())
		object->SaveConnections(objects, file);
}

void Board::AddObject(Object *object) {
	object->next = objects;
	object->prev = nullptr;
	if(objects)
		objects->prev = object;
	objects = object;
}

