#include "Board.h"
#include "GLUtils.h"
#include "Utils.h"
#include "THTPad.h"
#include "Circle.h"
#include "Track.h"

Board::Board(const char *_name, Type type, Vec2 innerSize, float border, bool originTop) : Board() {
	objects = nullptr;
	if(type != Type::Empty)
		size.Set(innerSize.x + border * 2.0f, innerSize.y + border * 2.0f);
	else
		size = innerSize;

	if(type == Type::Round) {
		Circle *circle = new Circle(LAYER_O, 0.0f, innerSize * 0.5f + Vec2(border, border), innerSize.x, 0.0f, 0.0f);
		AddObjectBegin(circle);
	} else if(type == Type::Rectangle) {
		const Vec2 points[] = {
			{border, border},
			{border + innerSize.x, border},
			{border + innerSize.x, border + innerSize.y},
			{border, border + innerSize.y},
			{border, border}
		};
		Track *frame = new Track(LAYER_O, 0.0f, points, 5);
		AddObjectBegin(frame);
	}
	if(originTop)
		origin = Vec2(border, border);
	else
		origin = Vec2(border, size.y - border);
	strcpy(name, _name);
	for(int i = 0; i < 7; i++) {
		groundPane[i] = false;
		layerVisible[i] = true;
	}
	multilayer = false;
	activeLayer = LAYER_C2;
	activeGrid = grid = 1.27;
	camera.Set(0.0f, 0.0f);
	zoom = 10;
	origin.Set(0.0f, 0.0f);
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
	origin.InvY().SaveInt(file);
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
	origin.LoadInt(file);
	origin = origin.InvY();
	multilayer = file.Read<uint8_t>();

	uint32_t objectCount = file.Read<uint32_t>();
	Object *last = nullptr;
	for(int i = 0; i < objectCount; i++)
		last = AddObjectEnd(Object::Load(file), last);
	for(Object *object = objects; object; object = object->GetNext())
		object->LoadConnections(objects, file);

	UpdateGrid(false, false);
}

bool Board::IsSelectedLayerCopper() const {
	return	activeLayer == LAYER_C1 || activeLayer == LAYER_C2 ||
			activeLayer == LAYER_I1 || activeLayer == LAYER_I2;
}

void Board::UpdateGrid(bool shift, bool ctrl) {
	if(ctrl)
		activeGrid = 0.0;
	else if(shift)
		activeGrid = grid * 0.5;
	else
		activeGrid = grid;
}

Vec2 Board::ToGrid(const Vec2 &vec) const {
	return utils::ToGrid(vec, grid, origin);
}

Vec2 Board::ToActiveGrid(const Vec2 &vec) const {
	if(activeGrid == 0.0)
		return vec;
	return utils::ToGrid(vec, activeGrid, origin);
}

void Board::SnapSelectedToGrid() {
	for(Object *object = objects; object; object = object->GetNext())
		if(object->IsSelected() && object->groups.Empty())
			object->Move(ToGrid(object->GetPosition()) - object->GetPosition());
	bool ok = true;
	for(int i = 0; ok; i++) {
		ok = false;
		Vec2 delta;
		for(Object *object = objects; object; object = object->GetNext())
			if(object->IsSelected() && !object->groups.Empty() && object->groups.Last() == i) {
				if(!ok)
					delta = ToGrid(object->GetPosition()) - object->GetPosition();
				object->Move(delta);
				ok = true;
			}
	}
}

void Board::UpdateCamera(const Vec2 &delta) {
	camera -= delta;
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

void Board::ZoomAABB(const Vec2 &screenSize, const AABB &aabb) {
	Vec2 size = aabb.Size();
	camera = aabb.lower;
	if(screenSize.x / screenSize.y > size.x / size.y) {
		zoom = screenSize.y / size.y;
		camera.x -= (screenSize.x / screenSize.y * size.y - size.x) / 2.0f;
	} else {
		zoom = screenSize.x / size.x;
		camera.y -= (screenSize.y / screenSize.x * size.x - size.y) / 2.0f;
	}
}

void Board::ZoomBoard(const Vec2 &screenSize) {
	ZoomAABB(screenSize, AABB(Vec2(0.0f, 0.0f), size));
}

void Board::ZoomObjects(const Vec2 &screenSize) {
	if(!objects)
		return;
	AABB aabb(objects->GetAABB());
	for(Object *object = objects->GetNext(); object; object = object->GetNext())
		aabb |= object->GetAABB();
	ZoomAABB(screenSize, aabb);
}

void Board::ZoomSelection(const Vec2 &screenSize) {
	Object *first = GetFirstSelected();
	if(!first)
		return;
	AABB aabb(first->GetAABB());
	for(Object *object = objects->GetNext(); object; object = object->GetNext())
		if(object->IsSelected())
			aabb |= object->GetAABB();
	ZoomAABB(screenSize, aabb);
}

Vec2 Board::ConvertToCoords(const Vec2 &vec) const {
	return (vec / zoom + camera);
}
Vec2 Board::ConvertFromCoords(const Vec2 &vec) const {
	return ((vec - camera) * zoom);
}
void Board::Draw(const Settings &settings, const Vec2 &screenSize) const {
	const ColorScheme &colors = settings.GetColorScheme();
	glMatrixMode(GL_PROJECTION);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, screenSize.x, screenSize.y);
	glLoadIdentity();
	glOrtho(0.0f, screenSize.x / zoom, screenSize.y / zoom, 0.0f, 0.0f, 1.0f);
	glutils::Translate(-camera);

	if(GetCurrentLayerGround()) {
		if(settings.darkGround)
			colors.SetGroundColor(COLOR_C1 + activeLayer);
		else
			colors.SetColor(COLOR_C1 + activeLayer);
	} else
		colors.SetColor(COLOR_BGR);

	glRectf(0.0f, 0.0f, size.x, size.y);

	glEnable(GL_SCISSOR_TEST);
	glScissor(-camera.x * zoom, screenSize.y - (size.y - camera.y) * zoom, size.x * zoom, size.y * zoom);

	if(GetCurrentLayerGround()) {
		colors.SetColor(COLOR_BGR);
		DrawGroundDistance(activeLayer);
	}

	if(settings.showGrid && activeGrid * zoom > 6.0)
		DrawGrid(settings, screenSize);

	if(settings.transparent) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}
	DrawObjects(colors, activeLayer, false);
	glDisable(GL_BLEND);

	colors.SetColor(COLOR_SELO);
	DrawSelected();

	colors.SetDrillingsColor(settings.drill);
	DrawDrillings();

	colors.SetColor(COLOR_CON);
	DrawConnections();

	glDisable(GL_SCISSOR_TEST);
}

void Board::DrawGrid(const Settings &settings, const Vec2 &screenSize) const {
	glEnable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	const ColorScheme &colors = settings.GetColorScheme();
	uint8_t subgrid = settings.GetSubGrid();
	double subgridValue = subgrid * activeGrid;
	Vec2 begin(fmod(origin.x, subgridValue) - subgridValue, fmod(origin.y, subgridValue) - subgridValue);
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
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			for(double x = begin.x; x < end.x; x += subgridValue)
				for(double y = begin.y; y < end.y; y += subgridValue)
					glutils::Vertex(Vec2(x, y));
			glEnd();
		}
	}
}
