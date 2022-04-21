#pragma once
#include <wx/window.h>
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
	void ShowDialog(wxWindow *parent);

	virtual void Save(File &file) const;
	virtual void Load(File &file);
private:
	ImageConfig images[2];
};
