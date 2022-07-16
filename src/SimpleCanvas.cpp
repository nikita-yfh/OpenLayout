#include "SimpleCanvas.h"
#include "GLUtils.h"

wxBEGIN_EVENT_TABLE(SimpleCanvas, wxGLCanvas)
	EVT_PAINT(SimpleCanvas::Draw)
wxEND_EVENT_TABLE()

static const int attribList[] = {
	WX_GL_RGBA,
	WX_GL_SAMPLE_BUFFERS, GL_TRUE,
	WX_GL_STENCIL_SIZE, 8, 0, 0
};

static const float zoomRatio = 1.3f;

SimpleCanvas::SimpleCanvas(wxWindow *parent, ObjectGroup *_objects, const Settings &_settings)
		: wxGLCanvas(parent, wxID_ANY, attribList), objects(_objects), settings(_settings) {}

void SimpleCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);
	const ColorScheme &colors = settings.GetColorScheme();

	colors.SetBackgroundClearColor();
	glClear(GL_COLOR_BUFFER_BIT);

	AABB aabb = objects->GetObjectsAABB();
	if(aabb.IsValid()) {
		Vec2 screenSize = GetSize();
		Vec2 size = aabb.Size();
		glViewport(5, 5, GetSize().x - 10, GetSize().y - 10);
		glLoadIdentity();
		if(screenSize.x / screenSize.y > size.x / size.y) {
			glOrtho(0.0f, screenSize.x / screenSize.y * size.y, size.y, 0.0f, 0.0f, 1.0f);
			glutils::Translate(-aabb.lower + Vec2((screenSize.x / screenSize.y * size.y - size.x) / 2.0f, 0.0f));
		} else {
			glOrtho(0.0f, size.x, screenSize.y / screenSize.x * size.y, 0.0f, 0.0f, 1.0f);
			glutils::Translate(-aabb.lower + Vec2(0.0f, (screenSize.y / screenSize.x * size.x - size.y) / 2.0f));
		}
		objects->DrawObjects(colors, ObjectGroup::LAYER_C2, true);

		colors.SetDrillingsColor(settings.drill);
		objects->DrawDrillings();
	}

	glFlush();
	SwapBuffers();
}
