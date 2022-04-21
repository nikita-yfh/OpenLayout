#include "THTPad.h"

void THTPad::SaveObject(File &file) const {
	file.Write<uint8_t>(THT_PAD);
	position.Save<float>(file);
	file.WriteMm(outDiameter / 2.0f);
	file.WriteMm(inDiameter / 2.0f);
	file.WriteNull(5);
	file.Write<uint8_t>(layer);
	file.Write<uint8_t>(shape);
	file.WriteNull(4);
	file.Write<uint16_t>(componentID);
	file.WriteNull(1);
	file.Write(thermalStyle, 4);
	file.WriteNull(6);
	file.WriteMm<uint32_t>(groundDistance);
	file.WriteNull(5);
	file.Write<uint8_t>(thermal);
	file.WriteNull(2);
	file.WriteMm<uint32_t>(thermalSize);
	file.Write<uint8_t>(through);
	file.Write<uint8_t>(soldermask);
	file.WriteNull(22);
	file.WriteString(marker);
}
void THTPad::LoadObject(File &file){
	position.Load<float>(file);
	inDiameter = file.ReadMm<float>() * 2.0f;
	outDiameter = file.ReadMm<float>() * 2.0f;
	file.ReadNull(5);
	layer = file.Read<uint8_t>();
	shape = file.Read<uint8_t>();
	file.ReadNull(4);
	componentID = file.Read<uint16_t>();
	file.ReadNull(1);
	file.Read(thermalStyle, 4);
	file.ReadNull(6);
	groundDistance = file.ReadMm<uint32_t>();
	file.ReadNull(5);
	thermal = file.Read<uint8_t>();
	file.ReadNull(2);
	thermalSize = file.ReadMm<uint32_t>();
	through = file.Read<uint8_t>();
	soldermask = file.Read<uint8_t>();
	file.ReadNull(22);
	file.ReadString(marker);
}


