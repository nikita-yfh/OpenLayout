#include "ObjectGroup.h"
#include "GLUtils.h"
#include "THTPad.h"
#include "Track.h"
#include "Poly.h"
#include "Circle.h"
#include "Utils.h"
#include <vector>
#include <cfloat>
#include <algorithm>
#include <map>

ObjectGroup::ObjectGroup(const ObjectGroup &other) {
	Object *last = nullptr;
	for(const Object *object = other.objects; object; object = object->next)
		last = AddObjectEnd(object->Clone(), last);
	for(Object *object = objects; object; object = object->next)
		object->UpdateConnections(objects);
}

ObjectGroup::ObjectGroup() {
	objects = nullptr;
}

ObjectGroup *ObjectGroup::CopySelected() const {
	ObjectGroup *group = new ObjectGroup();
	Object *last = nullptr;
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			last = group->AddObjectEnd(object->Clone(), last);
	if(group->IsEmpty()) {
		delete group;
		return nullptr;
	}
	Vec2 center = group->GetObjectsAABB().GetCenter();
	for(Object *object = group->objects; object; object = object->next)
		object->Move(Vec2(-center.x, -center.y));
	return group;
}

ObjectGroup::~ObjectGroup() {
	while(objects) {
		Object *next = objects->next;
		delete objects;
		objects = next;
	}
}

Object *ObjectGroup::GetLast() {
	if(IsEmpty())
		return nullptr;
	Object *object = objects;
	while(object->next) object = object->next;
	return object;
}

uint32_t ObjectGroup::GetObjectCount() const {
	uint32_t n = 0;
	Object *tmp = objects;
	while(tmp) {
		n++;
		tmp = tmp->GetNext();
	}
	return n;
}

void ObjectGroup::AddObjectEnd(Object *object) {
	object->next = nullptr;
	Object *last = GetLast();
	if(last)
		last->next = object;
	else
		objects = object;
	object->prev = last;
}

Object *ObjectGroup::AddObjectEnd(Object *object, Object *last) {
	if(last == nullptr)
		objects = object;
	else
		last->next = object;
	object->prev = last;
	object->next = nullptr;
	return object;
}

void ObjectGroup::AddObjectBegin(Object *object) {
	object->next = objects;
	object->prev = nullptr;
	if(objects)
		objects->prev = object;
	objects = object;
}
void ObjectGroup::RemoveObject(Object *object) {
	if(object->next)
		object->next->prev = object->prev;
	if(object->prev)
		object->prev->next = object->next;
	else
		objects = object->next;
	delete object;
}

void ObjectGroup::PlaceObject(Object *object) {
	object->SetPlaced();
	AddObjectBegin(object);
}

void ObjectGroup::PlaceGroup(const ObjectGroup &group, const Vec2 &position) {
	uint32_t freeGroup = GetFreeGroup();
	Object *last = GetLast();
	Object *beginGroup = nullptr;
	for(const Object *object = group.objects; object; object = object->next) {
		last = AddObjectEnd(object->Clone(), last);
		if(!beginGroup)
			beginGroup = last;
	}
	for(Object *object = beginGroup; object; object = object->next) {
		object->UpdateConnections(beginGroup);
		for(int i = 0; i < object->groups.Size(); i++)
			object->groups[i] += freeGroup;
		object->groups.Add(freeGroup);
		object->SetPlaced();
		object->Move(position);
	}
}

Object *ObjectGroup::GetFirstPlaced() {
	for(Object *object = objects; object; object = object->next)
		if(object->IsPlaced())
			return object;
	return nullptr;
}

Object *ObjectGroup::GetFirstSelected() {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return object;
	return nullptr;
}

const Object *ObjectGroup::GetFirstSelected() const {
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return object;
	return nullptr;
}

bool ObjectGroup::IsSelected() const {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			return true;
	return false; }

bool ObjectGroup::IsSelectedTwo() const {
	bool first = false;
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected()) {
			if(first)
				return true;
			first = true;
		}
	return false;
}

void ObjectGroup::InvertSelectionGroup(Object *o1) {
	bool selected = !o1->selected;
	o1->selected = selected;
	for(Object *o2 = objects; o2; o2 = o2->next)
		for(int i1 = 0; i1 < o1->groups.Size(); i1++)
			for(int i2 = 0; i2 < o2->groups.Size(); i2++)
				if(o1->groups[i1] == o2->groups[i2])
					o2->selected = selected;
}

void ObjectGroup::SelectAll() {
	for(Object *object = objects; object; object = object->next)
		if(!object->IsPlaced())
			object->Select();
}

void ObjectGroup::UnselectAll() {
	for(Object *object = objects; object; object = object->next)
		object->Unselect();
}
void ObjectGroup::SelectInRect(const AABB &rect) {
	for(Object *object = objects; object; object = object->next)
		if(!object->IsPlaced() && rect.Contains(object->GetAABB()))
			object->Select();
}

void ObjectGroup::DeleteSelected() {
	for(Object *object = objects; object;) {
		if(object->IsSelected()) {
			if(object->next)
				object->next->prev = object->prev;
			if(object->prev)
				object->prev->next = object->next;
			else
				objects = object->next;
			Object *temp = object;
			object = object->next;
			delete temp;
		} else
			object = object->next;
	}
}

void ObjectGroup::DeleteOutside(const AABB &keepConst) {
	AABB keep = keepConst;
	for(Object *object = objects; object;) {
		if(!keep.TestOverlap(object->GetAABB())) {
			if(object->next)
				object->next->prev = object->prev;
			if(object->prev)
				object->prev->next = object->next;
			else
				objects = object->next;
			Object *temp = object;
			object = object->next;
			delete temp;
		} else
			object = object->next;
	}
}

void ObjectGroup::RemoveAllConnections() {
	for(Object *object = objects; object; object = object->next)
		object->ClearConnections();
}

void ObjectGroup::SetSelectedToLayer(uint8_t layer) {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->layer = layer;
}

void ObjectGroup::ResetSoldermask() {
	for(Object *object = objects; object; object = object->next)
		object->soldermask = false;
}

void ObjectGroup::ToggleSoldermask(Object *object) {
	object->soldermask = !object->soldermask;
}

// --- Continuity test (approximate copper connectivity) ----------------------
//
// Two copper objects connect if they share a copper layer and their geometry
// touches. Metallised through-pads bridge all copper layers. The touch test is
// a heuristic: minimum distance between the objects' outlines (polyline for
// tracks/zones, disc for pads/circles) compared against their half-widths.

static bool layerIsCopper(uint8_t l) {
	return l == ObjectGroup::LAYER_C1 || l == ObjectGroup::LAYER_C2 ||
	       l == ObjectGroup::LAYER_I1 || l == ObjectGroup::LAYER_I2;
}

static uint8_t copperMask(const Object *o) {
	if(o->GetType() == Object::THT_PAD && ((const THTPad*) o)->HasMetallization())
		return (1 << ObjectGroup::LAYER_C1) | (1 << ObjectGroup::LAYER_C2) |
		       (1 << ObjectGroup::LAYER_I1) | (1 << ObjectGroup::LAYER_I2);
	uint8_t l = o->GetLayer();
	return layerIsCopper(l) ? (1 << l) : 0;
}

static void shapeOf(const Object *o, std::vector<Vec2> &pts, float &halfWidth) {
	pts.clear();
	switch(o->GetType()) {
	case Object::TRACK:
	case Object::POLY: {
		const PolygonBase *p = (const PolygonBase*) o;
		for(uint32_t i = 0; i < p->points.Size(); i++)
			pts.push_back(p->points[i]);
		if(o->GetType() == Object::POLY && pts.size() > 1)
			pts.push_back(pts[0]);          // close the zone outline
		halfWidth = ((const LineObject*) o)->GetWidth() / 2.0f;
		if(pts.empty())
			pts.push_back(o->GetPosition());
		break;
	}
	case Object::CIRCLE:
		pts.push_back(o->GetPosition());
		halfWidth = ((const Circle*) o)->GetDiameter() / 2.0f;
		break;
	default: {                       // pads
		AABB box = o->GetAABB();
		Vec2 s = box.Size();
		pts.push_back(o->GetPosition());
		halfWidth = 0.5f * std::min(s.x, s.y);
		break;
	}
	}
}

static float pointSeg(const Vec2 &p, const Vec2 &a, const Vec2 &b) {
	Vec2 ab = b - a;
	float len2 = ab.LengthSq();
	if(len2 < 1e-9f)
		return (p - a).Length();
	float t = Vec2::Dot(p - a, ab) / len2;
	if(t < 0.0f) t = 0.0f;
	else if(t > 1.0f) t = 1.0f;
	return (p - (a + ab * t)).Length();
}

static bool objectsConnect(const Object *a, const Object *b) {
	uint8_t ma = copperMask(a), mb = copperMask(b);
	if(!ma || !mb || !(ma & mb))
		return false;

	std::vector<Vec2> pa, pb;
	float ha, hb;
	shapeOf(a, pa, ha);
	shapeOf(b, pb, hb);
	float reach = ha + hb + 0.05f;

	AABB ba = a->GetAABB();
	if(!ba.Expand(reach).TestOverlap(b->GetAABB()))
		return false;

	float best = FLT_MAX;
	for(size_t i = 0; i < pa.size(); i++) {
		if(pb.size() == 1)
			best = std::min(best, (pa[i] - pb[0]).Length());
		else for(size_t j = 0; j + 1 < pb.size(); j++)
			best = std::min(best, pointSeg(pa[i], pb[j], pb[j + 1]));
	}
	for(size_t i = 0; i < pb.size(); i++) {
		if(pa.size() == 1)
			best = std::min(best, (pb[i] - pa[0]).Length());
		else for(size_t j = 0; j + 1 < pa.size(); j++)
			best = std::min(best, pointSeg(pb[i], pa[j], pa[j + 1]));
	}
	if(best <= reach)
		return true;

	if(pa.size() > 1 && pb.size() > 1)             // crossing tracks
		for(size_t i = 0; i + 1 < pa.size(); i++)
			for(size_t j = 0; j + 1 < pb.size(); j++)
				if(utils::IntersectTwoLines(pa[i], pa[i + 1], pb[j], pb[j + 1]))
					return true;
	return false;
}

// Edge-to-edge gap between two copper outlines (negative if they overlap).
static float copperEdgeGap(const Object *a, const Object *b) {
	std::vector<Vec2> pa, pb;
	float ha, hb;
	shapeOf(a, pa, ha);
	shapeOf(b, pb, hb);
	float best = FLT_MAX;
	for(size_t i = 0; i < pa.size(); i++) {
		if(pb.size() == 1)
			best = std::min(best, (pa[i] - pb[0]).Length());
		else for(size_t j = 0; j + 1 < pb.size(); j++)
			best = std::min(best, pointSeg(pa[i], pb[j], pb[j + 1]));
	}
	for(size_t i = 0; i < pb.size(); i++) {
		if(pa.size() == 1)
			best = std::min(best, (pb[i] - pa[0]).Length());
		else for(size_t j = 0; j + 1 < pa.size(); j++)
			best = std::min(best, pointSeg(pb[i], pa[j], pa[j + 1]));
	}
	if(pa.size() > 1 && pb.size() > 1)
		for(size_t i = 0; i + 1 < pa.size(); i++)
			for(size_t j = 0; j + 1 < pb.size(); j++)
				if(utils::IntersectTwoLines(pa[i], pa[i + 1], pb[j], pb[j + 1]))
					best = 0.0f;
	return best - ha - hb;
}

std::vector<std::pair<Vec2, std::string>> ObjectGroup::CheckDRC(float clearance) const {
	std::vector<std::pair<Vec2, std::string>> issues;

	// Collect copper objects and flood them into nets by physical copper touch.
	std::vector<Object*> cop;
	for(Object *o = objects; o; o = o->next)
		if(copperMask(o))
			cop.push_back(o);
	std::map<Object*, int> net;
	int nid = 0;
	for(Object *s : cop) {
		if(net.count(s))
			continue;
		std::vector<Object*> st = {s};
		net[s] = nid;
		while(!st.empty()) {
			Object *x = st.back();
			st.pop_back();
			for(Object *y : cop)
				if(!net.count(y) && objectsConnect(x, y)) {
					net[y] = nid;
					st.push_back(y);
				}
		}
		nid++;
	}

	// Clearance: copper of different nets closer than the minimum gap.
	for(size_t i = 0; i < cop.size(); i++)
		for(size_t j = i + 1; j < cop.size(); j++) {
			Object *a = cop[i], *b = cop[j];
			if(!(copperMask(a) & copperMask(b)) || net[a] == net[b])
				continue;
			AABB box = a->GetAABB();
			if(!box.Expand(clearance).TestOverlap(b->GetAABB()))
				continue;
			float gap = copperEdgeGap(a, b);
			if(gap < clearance - 1e-3f)
				issues.push_back({(a->GetPosition() + b->GetPosition()) * 0.5f,
					gap < 0.0f ? "Copper overlap (short) between nets"
					           : "Clearance below minimum between nets"});
		}

	// Unrouted: a rubber-band whose pads are not joined by copper.
	for(Object *o : cop)
		if(o->IsPad()) {
			Pad *p = (Pad*) o;
			for(uint32_t k = 0; k < p->ConnectionCount(); k++) {
				Pad *q = p->GetConnection(k);
				if(p < q && net.count(p) && net.count(q) && net[p] != net[q])
					issues.push_back({(p->GetPosition() + q->GetPosition()) * 0.5f,
						"Unrouted connection"});
			}
		}

	return issues;
}

// Edge distance from a point to an object's copper outline (negative if inside).
static float distToOutlineEdge(const Vec2 &p, const Object *o) {
	std::vector<Vec2> pts;
	float hw;
	shapeOf(o, pts, hw);
	float best = FLT_MAX;
	if(pts.size() == 1)
		best = (p - pts[0]).Length();
	else
		for(size_t j = 0; j + 1 < pts.size(); j++)
			best = std::min(best, pointSeg(p, pts[j], pts[j + 1]));
	return best - hw;
}

void ObjectGroup::FillZone(Object *zone, float clearance, float lineWidth) {
	if(!zone || zone->GetType() != Object::POLY)
		return;
	PolygonBase *pb = (PolygonBase*) zone;
	int n = pb->points.Size();
	if(n < 3)
		return;
	uint8_t layer = zone->GetLayer();
	if(!layerIsCopper(layer))
		return;

	std::vector<Vec2> poly;
	for(int i = 0; i < n; i++)
		poly.push_back(pb->points[i]);

	// Flood copper into nets, then pick the net dominant inside the zone.
	std::vector<Object*> cop;
	for(Object *o = objects; o; o = o->next)
		if(copperMask(o))
			cop.push_back(o);
	std::map<Object*, int> net;
	int nid = 0;
	for(Object *s : cop) {
		if(net.count(s)) continue;
		std::vector<Object*> st = {s};
		net[s] = nid;
		while(!st.empty()) {
			Object *x = st.back(); st.pop_back();
			for(Object *y : cop)
				if(!net.count(y) && objectsConnect(x, y)) { net[y] = nid; st.push_back(y); }
		}
		nid++;
	}
	std::map<int, int> inside;
	for(Object *o : cop)
		if(o != zone && utils::PointInConcavePolygon(o->GetPosition(), n, poly.data()))
			inside[net[o]]++;
	int zoneNet = -1, bestCount = 0;
	for(auto &kv : inside)
		if(kv.second > bestCount) { bestCount = kv.second; zoneNet = kv.first; }

	float keep = clearance + lineWidth * 0.5f;
	uint32_t grp = GetFreeGroup();
	float s = lineWidth;
	AABB box = zone->GetAABB();

	auto blocked = [&](const Vec2 &p) -> bool {
		for(Object *o : cop) {
			if(o == zone || net[o] == zoneNet)
				continue;
			// Through-pads pierce every layer, so the pour must clear their holes
			// regardless of layer; other copper only matters on the pour layer.
			bool through = (o->GetType() == Object::THT_PAD);
			if(!through && !(copperMask(o) & (1 << layer)))
				continue;
			if(!o->GetAABB().Expand(keep).TestPoint(p))
				continue;
			if(distToOutlineEdge(p, o) < keep)
				return true;
		}
		return false;
	};
	auto emitRun = [&](const Vec2 &a, const Vec2 &b) {
		Vec2 pts[2] = {a, b};
		Track *t = new Track(layer, clearance, lineWidth, pts, 2);
		t->groups.Add(grp);          // grouped so the pour selects/deletes as one
		AddObjectEnd(t);             // permanent (not SetPlaced -> not a temporary)
	};

	for(float y = box.lower.y; y <= box.upper.y; y += s) {
		bool run = false;
		Vec2 a;
		float prevx = box.lower.x;
		for(float x = box.lower.x; x <= box.upper.x; x += s) {
			Vec2 p(x, y);
			bool ok = utils::PointInConcavePolygon(p, n, poly.data()) && !blocked(p);
			if(ok && !run) { run = true; a = p; }
			else if(!ok && run) { run = false; emitRun(a, Vec2(prevx, y)); }
			prevx = x;
		}
		if(run)
			emitRun(a, Vec2(prevx, y));
	}

	RemoveObject(zone);   // the cleared fill replaces the naive solid polygon
}

void ObjectGroup::SelectConnected(Object *start) {
	UnselectAll();
	if(!start || !copperMask(start))
		return;
	std::vector<Object*> stack;
	start->Select();
	stack.push_back(start);
	while(!stack.empty()) {
		Object *a = stack.back();
		stack.pop_back();
		for(Object *b = objects; b; b = b->next)
			if(!b->IsSelected() && objectsConnect(a, b)) {
				b->Select();
				stack.push_back(b);
			}
	}
}

void ObjectGroup::DrawSoldermaskMarked() const {
	// Translucent green coating over the objects flagged for the solder mask.
	glColor4ub(0, 200, 90, 130);
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->soldermask)
			object->DrawObject();
}

void ObjectGroup::SetSelectedWidth(float width) {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->SetWidth(width);
}

void ObjectGroup::ArraySelected(int cols, int rows, const Vec2 &spacing) {
	if(!IsSelected())
		return;
	std::vector<Object*> sel;
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			sel.push_back(object);
	Object *last = GetLast();
	for(int r = 0; r < rows; r++)
		for(int c = 0; c < cols; c++) {
			if(r == 0 && c == 0)
				continue;
			Vec2 d(c * spacing.x, r * spacing.y);
			for(Object *o : sel) {
				Object *clone = o->Clone();
				clone->Move(d);
				last = AddObjectEnd(clone, last);
			}
		}
}

void ObjectGroup::ChangeSide(float mirrorX) {
	// Mirror every object horizontally and swap the top/bottom layer pairs.
	static const uint8_t swap[7] = {LAYER_C2, LAYER_S2, LAYER_C1, LAYER_S1,
	                                LAYER_I2, LAYER_I1, LAYER_O};
	for(Object *object = objects; object; object = object->next) {
		object->MirrorHorizontal(mirrorX);
		if(object->layer < 7)
			object->layer = swap[object->layer];
	}
}

void ObjectGroup::CancelPlacing() {
	for(Object *object = objects; object;) {
		if(object->IsPlaced()) {
			if(object->next)
				object->next->prev = object->prev;
			if(object->prev)
				object->prev->next = object->next;
			else
				objects = object->next;
			Object *temp = object;
			object = object->next;
			delete temp;
		} else
			object = object->next;
	}
}

bool ObjectGroup::CanGroup() const {
	const Object *first = GetFirstSelected();
	if(!first)
		return false;
	for(const Object *object = first->next; object; object = object->next)
		if(object->IsSelected() && (first->groups.Empty() || object->groups.Empty() || object->groups.Last() != first->groups.Last()))
			return true;
	return false;
}
bool ObjectGroup::CanUngroup() const {
	uint32_t max = GetMaxSelectedGroup();
	bool groups[max + 1] = {false};
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			for(int i = 0; i < object->groups.Size(); i++) {
				if(groups[object->groups[i]])
					return true;
				else
					groups[object->groups[i]] = true;
			}
	return false;
}

void ObjectGroup::GroupSelected() {
	uint32_t group = GetFreeGroup();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->groups.Add(group);
}

void ObjectGroup::UngroupSelected() {
	if(!IsSelected())
		return;
	uint32_t max = GetMaxSelectedGroup();
	uint32_t groupCount[max + 1] = {0};
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			for(int i = 0; i < object->groups.Size(); i++)
				groupCount[object->groups[i]]++;
	uint32_t group;
	uint32_t maxCount = 0;
	for(int i = 0; i <= max; i++) {
		if(maxCount < groupCount[i]) {
			maxCount = groupCount[i];
			group = i;
		}
	}
	
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->groups.Remove(group);

}

uint32_t ObjectGroup::GetFreeGroup() const {
	uint32_t max = 0;
	for(const Object *object = objects; object; object = object->next)
		for(int i = 0; i < object->groups.Size(); i++)
			if(max < object->groups[i])
				max = object->groups[i];
	return max + 1;
}

uint32_t ObjectGroup::GetMaxSelectedGroup() const {
	uint32_t max = 0;
	for(const Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			for(int i = 0; i < object->groups.Size(); i++)
				if(max < object->groups[i])
					max = object->groups[i];
	return max;
}

void ObjectGroup::MovePlaced(const Vec2 &d) {
	for(Object *object = objects; object; object = object->next)
		if(object->IsPlaced())
			object->Move(d);
}

void ObjectGroup::MoveSelected(const Vec2 &d) {
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->Move(d);
}

void ObjectGroup::RotateSelected(float angle) {
	if(!IsSelected())
		return;
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->Rotate(center, angle);
}

void ObjectGroup::MirrorSelectedHorizontal() {
	if(!IsSelected())
		return;
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->MirrorHorizontal(center.x);
}

void ObjectGroup::MirrorSelectedVertical() {
	if(!IsSelected())
		return;
	Vec2 center = GetSelectedCenter();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected())
			object->MirrorVertical(center.y);
}

void ObjectGroup::AlignSelected(Vec2(*callback)(const AABB&, const AABB&)) {
	if(!IsSelected())
		return;
	uint32_t max = GetMaxSelectedGroup();
	AABB aabb[max + 1];
	for(int i = 0; i <= max; i++)
		aabb[i] = AABB::Invalid();
	AABB all = GetSelectedAABB();
	for(Object *object = objects; object; object = object->next)
		if(object->IsSelected() && !object->groups.Empty())
			aabb[object->groups.Last()] |= object->GetAABB();
	for(Object *object = objects; object; object = object->GetNext())
		if(object->IsSelected()) {
			if(object->groups.Empty())
				object->Move(callback(all, object->GetAABB()));
			else
				object->Move(callback(all, aabb[object->groups.Last()]));
		}
}

AABB ObjectGroup::GetObjectsAABB() const {
	if(!objects)
		return AABB::Invalid();
	AABB aabb = objects->GetAABB();
	for(const Object *object = objects->next; object; object = object->next)
		aabb |= object->GetAABB();
	return aabb;
}

AABB ObjectGroup::GetSelectedAABB() const {
	const Object *first = GetFirstSelected();
	AABB aabb = first->GetAABB();
	for(const Object *object = first->next; object; object = object->next)
		if(object->IsSelected())
			aabb |= object->GetAABB();
	return aabb;
}

Vec2 ObjectGroup::GetSelectedCenter() const {
	return GetSelectedAABB().GetCenter();
}

void ObjectGroup::DrawObjects(const ColorScheme &colors, uint8_t activeLayer, bool selected, const bool *layerVisible) const {
	const uint8_t layers[7][7] = {
		{LAYER_I1, LAYER_I2, LAYER_C2, LAYER_C1, LAYER_S2, LAYER_S1, LAYER_O},
		{LAYER_C1, LAYER_C2, LAYER_I2, LAYER_I1, LAYER_S1, LAYER_S2, LAYER_O},
		{LAYER_C1, LAYER_C2, LAYER_I1, LAYER_I2, LAYER_S1, LAYER_S2, LAYER_O},
		{LAYER_I1, LAYER_I2, LAYER_C1, LAYER_C2, LAYER_S1, LAYER_S2, LAYER_O}
	};
	for(int i = 0; i < 7; i++) {
		uint8_t layer = layers[3][i];
		if(activeLayer == LAYER_C1 || activeLayer == LAYER_S1)
			layer = layers[0][i];
		else if(activeLayer == LAYER_I1)
			layer = layers[1][i];
		else if(activeLayer == LAYER_I2)
			layer = layers[2][i];
		colors.SetColor(COLOR_C1 + layer);
		for(const Object *object = objects; object; object = object->GetNext())
			if(object->GetLayer() == layer && (!object->IsSelected() || selected) && (!layerVisible || layerVisible[object->GetLayer()]) &&
					!(object->GetType() == Object::THT_PAD && ((THTPad*) object)->HasMetallization()))
				object->DrawObject();
	}
	colors.SetColor(COLOR_VIA);
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->GetType() == Object::THT_PAD && !object->IsSelected() && ((THTPad*) object)->HasMetallization())
			object->DrawObject();
}

void ObjectGroup::DrawObjectsPhoto(uint8_t activeLayer, const bool *layerVisible) const {
	// Same back-to-front layer ordering as DrawObjects, but with opaque
	// photo-realistic colours: copper as gold, silkscreen white, outline dark.
	const uint8_t layers[4][7] = {
		{LAYER_I1, LAYER_I2, LAYER_C2, LAYER_C1, LAYER_S2, LAYER_S1, LAYER_O},
		{LAYER_C1, LAYER_C2, LAYER_I2, LAYER_I1, LAYER_S1, LAYER_S2, LAYER_O},
		{LAYER_C1, LAYER_C2, LAYER_I1, LAYER_I2, LAYER_S1, LAYER_S2, LAYER_O},
		{LAYER_I1, LAYER_I2, LAYER_C1, LAYER_C2, LAYER_S1, LAYER_S2, LAYER_O}
	};
	int row = 3;
	if(activeLayer == LAYER_C1 || activeLayer == LAYER_S1) row = 0;
	else if(activeLayer == LAYER_I1) row = 1;
	else if(activeLayer == LAYER_I2) row = 2;

	for(int i = 0; i < 7; i++) {
		uint8_t layer = layers[row][i];
		if(layer == LAYER_S1 || layer == LAYER_S2)
			glColor3ub(235, 235, 235);          // silkscreen
		else if(layer == LAYER_O)
			glColor3ub(20, 20, 20);             // outline
		else
			glColor3ub(212, 170, 75);           // copper
		for(const Object *object = objects; object; object = object->GetNext())
			if(object->GetLayer() == layer && (!layerVisible || layerVisible[object->GetLayer()]) &&
					!(object->GetType() == Object::THT_PAD && ((THTPad*) object)->HasMetallization()))
				object->DrawObject();
	}
	glColor3ub(212, 170, 75);
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->GetType() == Object::THT_PAD && ((THTPad*) object)->HasMetallization())
			object->DrawObject();
}

void ObjectGroup::DrawGroundDistance(uint8_t activeLayer) const {
	for(const Object *object = objects; object; object = object->GetNext())
		if(object->GetLayer() == activeLayer)
			object->DrawGroundDistance();
}

void ObjectGroup::DrawDrillings(const bool *layerVisible) const {
	for(const Object *object = objects; object; object = object->GetNext())
		if(!layerVisible || layerVisible[object->GetLayer()])
			object->DrawDrillings();
}


