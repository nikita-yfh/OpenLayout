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

void Board::Save(File &file) const{
	file.WriteString(name,30);
	file.WriteNull(4);
	size.Save<int>(file);
	file.Write(groundPane,7);
	file.Write<double>(grid);
	file.Write<double>(zoom);
	camera.Save<int>(file);
	file.Write<uint32_t>(activeLayer);
	file.Write(layerVisible,7);
	images.Save(file);
	file.WriteNull(4);
	anchor.Save<int>(file);
	file.Write<uint8_t>(isMultilayer);
}


void Board::Load(File &file){
	file.ReadString(name,30);
	file.ReadNull(4);
	size.Load<int>(file);
	file.Read(groundPane,7);
	file.Read<double>();
	grid = file.Read<double>();
	zoom = file.Read<double>();
	camera.Load<int>(file);
	activeLayer = file.Read<uint32_t>();
	file.Read(layerVisible,7);
	images.Load(file);
	file.ReadNull(8);
	anchor.Load<int>(file);
	isMultilayer = file.Read<uint8_t>();
}


