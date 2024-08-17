#include "ImageConfig.h"

ImageConfig::ImageConfig(){
	enabled = false;
	*path = '\0';
	dpi = 600;
	shift.SetZero();
}
void ImageConfigs::Save(File &file) const{
	file.Write<uint8_t>(images[0].enabled);
	file.Write<uint8_t>(images[1].enabled);
	file.WriteString(images[0].path, 200);
	file.WriteString(images[1].path, 200);
	file.Write<uint32_t>(images[0].dpi);
	file.Write<uint32_t>(images[1].dpi);
	images[0].shift.SaveInt(file);
	images[1].shift.SaveInt(file);
}
void ImageConfigs::Load(File &file){
	images[0].enabled = file.Read<uint8_t>();
	images[1].enabled = file.Read<uint8_t>();
	file.ReadString(images[0].path, 200);
	file.ReadString(images[1].path, 200);
	images[0].dpi = file.Read<uint32_t>();
	images[1].dpi = file.Read<uint32_t>();
	images[0].shift.LoadInt(file);
	images[1].shift.LoadInt(file);
}
