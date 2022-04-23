#include "MacroPreview.h"

static const int attribList[] = {
	WX_GL_RGBA,
	WX_GL_SAMPLE_BUFFERS, GL_FALSE,
	WX_GL_DEPTH_SIZE, 24, 0, 0
};

MacroPreview::MacroPreview(wxWindow *parent)
	: wxGLCanvas(parent, wxID_ANY, attribList) {}


