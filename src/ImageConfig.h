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
};

class ImageConfigs {
public:
	//void ShowDialog(wxWindow *parent, const ColorScheme &colors);

	virtual void Save(File &file) const;
	virtual void Load(File &file);
private:
	ImageConfig images[2];
};
