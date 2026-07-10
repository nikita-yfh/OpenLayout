#pragma once
#include "ColorScheme.h"
#include "File.h"
#include "Vec2.h"

struct ImageConfig{
	ImageConfig();

	uint8_t enabled;
	char path[200];
	uint32_t dpi;
	Vec2 shift;

	void Draw() const;          // draw the underlay (lazily uploads a texture)
	void Reload() { dirty = true; }
private:
	mutable unsigned int texture = 0;
	mutable bool dirty = true;
	mutable int imgW = 0, imgH = 0;
};

class ImageConfigs {
public:
	//void ShowDialog(wxWindow *parent, const ColorScheme &colors);

	virtual void Save(File &file) const;
	virtual void Load(File &file);

	void Draw() const;
	ImageConfig &Get(uint8_t i) { return images[i]; }
private:
	ImageConfig images[2];
};
