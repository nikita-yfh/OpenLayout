#include "Board.h"

void Board::Save(File &file) const{
	file.WriteString(name,30);
	file.Write(__pad0,4);
	size.Save<int>(file);
	file.Write(groundPane,7);
	file.Write<double>(grid);
	file.Write<double>(zoom);
	camera.Save<int>(file);
	file.Write<uint32_t>(activeLayer);
	file.Write(layerVisible,7);
	images.Save(file);
	file.Write(__pad1,8);
	anchor.Save<int>(file);
	file.Write<uint8_t>(isMultilayer);
}


void Board::Load(File &file){
	file.ReadString(name,30);
	file.Read(__pad0,4);
	size.Load<int>(file);
	file.Read(groundPane,7);
	file.Read<double>();
	grid = file.Read<double>();
	zoom = file.Read<double>();
	camera.Load<int>(file);
	activeLayer = file.Read<uint32_t>();
	file.Read(layerVisible,7);
	images.Load(file);
	file.Read(__pad1,8);
	anchor.Load<int>(file);
	isMultilayer = file.Read<uint8_t>();
}


