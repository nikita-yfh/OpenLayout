#include "Board.h"
#include "GLUtils.h"

Board::Board(Type type, Vec2 innerSize, float border) : Board() {
	if(type != Type::Empty)
		size.Set(innerSize.x + border * 2.0f, innerSize.y + border * 2.0f);
	else
		size = innerSize;
}

Board::Board() {
	objects = nullptr;
	next = nullptr;
	for(int i = 0; i < 7; i++) {
		groundPane[i] = false;
		layerVisible[i] = true;
	}
	multilayer = false;
	activeLayer = LAYER_C2;
	activeGrid = grid = 1.27;
}

Board::~Board() {
	while(objects) {
		Object *next = objects->next;
		delete objects;
		objects = next;
	}
}

uint32_t Board::GetObjectCount() const {
	uint32_t n = 0;
	Object *tmp = objects;
	while(tmp) {
		n++;
		tmp = tmp->GetNext();
	}
	return n;
}

void Board::Save(File &file) const {
	file.WriteString(name, 30);
	file.WriteNull(4);
	size.SaveInt(file);
	file.Write(groundPane, 7);
	file.Write<double>(grid * 10000.0f);
	file.Write<double>(zoom / 10000.0f);
	file.Write<int32_t>(camera.x * zoom);
	file.Write<int32_t>(camera.y * zoom);
	file.Write<uint32_t>(activeLayer + 1);
	file.Write(layerVisible, 7);
	images.Save(file);
	file.WriteNull(8);
	anchor.InvY().SaveInt(file);
	file.Write<uint8_t>(multilayer);

	file.Write<uint32_t>(GetObjectCount());
	for(Object *object = objects; object; object = object->GetNext())
		object->Save(file);
	for(Object *object = objects; object; object = object->GetNext())
		object->SaveConnections(objects, file);
}


void Board::Load(File &file) {
	file.ReadString(name, 30);
	file.ReadNull(4);
	size.LoadInt(file);
	file.Read(groundPane, 7);
	grid = file.Read<double>() / 10000.0f;
	zoom = file.Read<double>() * 10000.0f;
	camera.x = file.Read<int32_t>() / zoom;
	camera.y = file.Read<int32_t>() / zoom;
	activeLayer = file.Read<uint32_t>() - 1;
	file.Read(layerVisible, 7);
	images.Load(file);
	file.ReadNull(8);
	anchor.LoadInt(file);
	anchor = anchor.InvY();
	multilayer = file.Read<uint8_t>();

	uint32_t objectCount = file.Read<uint32_t>();
	for(int i = 0; i < objectCount; i++)
		AddObject(Object::Load(file));
	for(Object *object = objects; object; object = object->GetNext())
		object->LoadConnections(objects, file);

	UpdateGrid(false, false);
}

void Board::AddObject(Object *object) {
	object->next = objects;
	object->prev = nullptr;
	if(objects)
		objects->prev = object;
	objects = object;
}

Object *Board::GetFirstSelected() {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return object;
	return nullptr;
}

void Board::UpdateGrid(bool shift, bool ctrl) {
	if(ctrl)
		activeGrid = 0.0;
	else if(shift)
		activeGrid = grid * 0.5;
	else
		activeGrid = grid;
}

void Board::UpdateCamera(const Vec2 &delta) {
	camera += delta;
}

void Board::Zoom(float ratio, const Vec2 &mouse) {
	// pos = mouse / zoom + camera
	// pos1 = pos2
	// mouse / zoom1 + camera1 = mouse / zoom2 + camera2
	// camera2 = camera1 + mouse / zoom1 - mouse / zoom2
	// camera2 = camera1 + mouse * (1 / zoom1 - 1 / zoom2)
	// camera2 = camera1 + mouse * (zoom2 - zoom1) / (zoom1 * zoom2)
	// camera2 = camera1 + mouse * zoom1 * (ratio - 1) / (zoom1 * zoom1 * ratio)
	// camera2 = camera1 + mouse * (ratio - 1) / (zoom1 * ratio)
	camera += mouse * (ratio - 1.0f) / (zoom * ratio);
	zoom *= ratio;
}

Vec2 Board::ConvertToCoords(const Vec2 &vec) const {
	return (vec / zoom + camera);
}
Vec2 Board::ConvertFromCoords(const Vec2 &vec) const {
	return ((vec - camera) * zoom);
}

void Board::Draw(const Settings &settings, const Vec2 &screenSize) const {
	const ColorScheme &colors = settings.GetColorScheme();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, screenSize.x, screenSize.y);
	glLoadIdentity();
	glOrtho(0.0f, screenSize.x / zoom, screenSize.y / zoom, 0.0f, 0.0f, 1.0f);
	glutils::Translate(-camera);

	if(GetCurrentLayerGround())
		colors.SetGroundColor(COLOR_C1 + activeLayer);
	else
		colors.SetColor(COLOR_BGR);

	glRectf(0.0f, 0.0f, size.x, size.y);

	glEnable(GL_SCISSOR_TEST);
	glScissor(-camera.x * zoom, screenSize.y - (size.y - camera.y) * zoom, size.x * zoom, size.y * zoom);

	if(GetCurrentLayerGround()) {
		colors.SetColor(COLOR_BGR);
		for(Object *object = objects; object; object = object->next) {
			glPushMatrix();
			object->DrawGroundDistance();
			glPopMatrix();
		}
	}

	if(settings.showGrid && activeGrid * zoom > 6.0)
		DrawGrid(settings, screenSize);

	for(Object *object = objects; object; object = object->next) {
		colors.SetColor(COLOR_C1 + object->layer);
		glPushMatrix();
		object->DrawObject();
		glPopMatrix();
	}

	if(settings.drill == DRILL_BGR)
		colors.SetColor(COLOR_BGR);
	else if(settings.drill == DRILL_BLACK)
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for(Object *object = objects; object; object = object->next) {
		glPushMatrix();
		object->DrawDrillings();
		glPopMatrix();
	}
	glDisable(GL_SCISSOR_TEST);
}

void Board::DrawGrid(const Settings &settings, const Vec2 &screenSize) const {
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	const ColorScheme &colors = settings.GetColorScheme();
	uint8_t subgrid = settings.GetSubGrid();
	double subgridValue = subgrid * activeGrid;
	Vec2 begin(fmod(anchor.x, subgridValue) - subgridValue, fmod(anchor.y, subgridValue) - subgridValue);
	Vec2 end(size.x, size.y);
	if(settings.gridStyle == GRID_LINES) {
		colors.SetColor(COLOR_LINES);
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		for(double x = begin.x; x < end.x; x += activeGrid) {
			glutils::Vertex(Vec2(x, begin.y));
			glutils::Vertex(Vec2(x, end.y));
		}
		for(double y = begin.y; y < end.y; y += activeGrid) {
			glutils::Vertex(Vec2(begin.x, y));
			glutils::Vertex(Vec2(end.x, y));
		}
		glEnd();
		if(subgrid != 1) {
			glLineWidth(2.0f);
			glBegin(GL_LINES);
			for(double x = begin.x; x < end.x; x += subgridValue) {
				glutils::Vertex(Vec2(x, begin.y));
				glutils::Vertex(Vec2(x, end.y));
			}
			for(double y = begin.y; y < end.y; y += subgridValue) {
				glutils::Vertex(Vec2(begin.x, y));
				glutils::Vertex(Vec2(end.x, y));
			}
			glEnd();
		}
	} else {
		colors.SetColor(COLOR_DOTS);
		glPointSize(1.0f);
		glBegin(GL_POINTS);
		for(double x = begin.x; x < end.x; x += activeGrid)
			for(double y = begin.y; y < end.y; y += activeGrid)
				glutils::Vertex(Vec2(x, y));
		glEnd();
		if(subgrid != 1) {
			glPointSize(2.0f);
			glBegin(GL_POINTS);
			for(double x = begin.x; x < end.x; x += subgridValue)
				for(double y = begin.y; y < end.y; y += subgridValue)
					glutils::Vertex(Vec2(x, y));
			glEnd();
		}
	}
}
