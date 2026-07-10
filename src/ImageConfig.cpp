#include "ImageConfig.h"
#include "GLUtils.h"
#include <QImage>

ImageConfig::ImageConfig(){
	enabled = false;
	*path = '\0';
	dpi = 600;
	shift.SetZero();
}

void ImageConfig::Draw() const {
	if(dirty) {
		dirty = false;
		imgW = 0;
		QImage img(QString::fromLocal8Bit(path));
		if(!img.isNull()) {
			QImage rgba = img.convertToFormat(QImage::Format_RGBA8888);
			imgW = rgba.width();
			imgH = rgba.height();
			if(!texture)
				glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0,
			             GL_RGBA, GL_UNSIGNED_BYTE, rgba.constBits());
		}
	}
	if(!imgW || !texture)
		return;
	float scale = 25.4f / (dpi ? dpi : 600);
	float w = imgW * scale, h = imgH * scale;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(shift.x,     shift.y);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(shift.x + w, shift.y);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(shift.x + w, shift.y + h);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(shift.x,     shift.y + h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void ImageConfigs::Draw() const {
	for(int i = 0; i < 2; i++)
		if(images[i].enabled && images[i].path[0])
			images[i].Draw();
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
