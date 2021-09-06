#include "Canvas.h"
#include "OpenLayoutApp.h"
const int attribList[] = {WX_GL_RGBA,
						  WX_GL_DOUBLEBUFFER,
						  WX_GL_SAMPLE_BUFFERS, GL_TRUE,
						  WX_GL_DEPTH_SIZE, 24, 0, 0
						 };
Canvas::Canvas(wxWindow *parent)
	:wxGLCanvas(parent,(wxGLCanvas*)NULL,wxID_ANY, wxDefaultPosition, wxDefaultSize, 0,L"GLCanvas",attribList) {
	Bind(wxEVT_MOUSEWHEEL,&Canvas::OnMouseWheel,this);
	Bind(wxEVT_MIDDLE_DOWN,&Canvas::OnMiddleDown,this);
	Bind(wxEVT_LEFT_DOWN,&Canvas::OnLeftDown,this);
	Bind(wxEVT_MOTION,&Canvas::OnMouseMotion,this);
	Bind(wxEVT_LEFT_UP,&Canvas::OnLeftUp,this);
	Bind(wxEVT_KEY_DOWN,&Canvas::OnKey,this);
	Bind(wxEVT_KEY_UP,&Canvas::OnKey,this);

	dragscroll_timer.Bind(wxEVT_TIMER, &Canvas::OnDragScroll, this);
	dragscroll_timer.Stop();
}
void Canvas::OnMouseWheel(wxMouseEvent&e) { //zooming canvas
	Vec2 mouse=GetMousePos(e);


	float zoom_old=BOARD.zoom;
	if(e.GetWheelRotation()>0)
		BOARD.zoom*=zoom_ratio;
	else
		BOARD.zoom/=zoom_ratio;
	if(BOARD.zoom<0.0001)
		BOARD.zoom=0.0001;

	BOARD.camera=((mouse+BOARD.camera)/zoom_old*BOARD.zoom)-mouse;
	StabilizeCamera();

	if(BOARD.get_selected_count())
		clickboardpos=BOARD.first_selected().get_position();
	clickmousepos=mousepos=mouse;

	Refresh();
	e.Skip();
}
void Canvas::OnMiddleDown(wxMouseEvent&e) {
	clickboardpos=BOARD.camera;
	clickmousepos=GetMousePos(e);
	e.Skip();
}
void Canvas::OnLeftDown(wxMouseEvent&e) {

	Vec2 mouse=GetMousePos(e);
	Vec2 board_pos=GetPos(mouse);
	Object *firstsel=nullptr;

	for(Object &o : BOARD.objects)
		if(o.point_in(board_pos))
			firstsel=&o;
	if((!firstsel || !firstsel->selected) && !shift)
		for(Object &o : BOARD.objects)
			o.selected=false;
	if(firstsel) {
		BOARD.select(*firstsel);
		clickboardpos=BOARD.first_selected().get_position();
		selection=SEL_OBJECT;
	}
	if(selection!=SEL_OBJECT) {
		sel_rect.SetP1(board_pos);
		sel_rect.SetP2(board_pos);
		selection=SEL_RECT;
	}
	clickmousepos=mousepos=mouse;
	dragscroll_timer.Start(5);
	Refresh();
	e.Skip();
}
void Canvas::OnLeftUp(wxMouseEvent&e) {


	if(selection==SEL_RECT)
		BOARD.select(sel_rect);

	selection=SEL_NONE;
	Refresh();
	dragscroll_timer.Stop();
	e.Skip();
}
void Canvas::OnDragScroll(wxTimerEvent&e) {
	if(selection!=SEL_NONE) {

		Vec2i wsize;
		GetSize(&wsize.width,&wsize.height);
		Vec2 camera_old=BOARD.camera;
		if(mousepos.x<0)BOARD.camera.x-=10;
		if(mousepos.y<0)BOARD.camera.y-=10;
		if(mousepos.x>wsize.x)BOARD.camera.x+=10;
		if(mousepos.y>wsize.y)BOARD.camera.y+=10;
		if(selection==SEL_OBJECT) {
			clickmousepos+=(camera_old-BOARD.camera);
			MoveObjects(mousepos);
		}
		sel_rect.SetP2(GetPos(mousepos));
		Refresh();
	}
	e.Skip();
}
void Canvas::MoveObjects(Vec2 mouse) {

	Vec2 delta=GetPos(mouse)-GetPos(clickmousepos)-BOARD.first_selected().get_position()+clickboardpos;
	for(Object &o : BOARD.objects)
		if(o.selected)
			o.move(BOARD.to_grid(delta,shift,ctrl));
}
void Canvas::OnMouseMotion(wxMouseEvent&e) {

	bool refresh=true;
	Vec2 mouse=GetMousePos(e);
	if(e.MiddleIsDown()) {
		BOARD.camera=clickboardpos+(clickmousepos-mouse);
		StabilizeCamera();
	} else if(e.LeftIsDown()) {
		mousepos=mouse;
		if(selection==SEL_RECT) {
			sel_rect.SetP2(GetPos(mouse));
			///////////
			/*for(Object &o : BOARD.objects)
				o.selected=false;
			if(selection==SEL_RECT)
				BOARD.select(sel_rect);*/
			//////////
		} else if(selection==SEL_OBJECT)
			MoveObjects(mouse);
	} else refresh=false;
	if(refresh)Refresh();
	e.Skip();
}
void Canvas::OnKey(wxKeyEvent &e) {
	ctrl=e.ControlDown();
	shift=e.ShiftDown();
	if(selection==SEL_OBJECT)
		Refresh();
	e.Skip();
}
Vec2 Canvas::GetPos(Vec2 mouse) {


	return ((mouse+Vec2(BOARD.camera.x,BOARD.camera.y))/BOARD.zoom).SwapY();
}
Vec2 Canvas::GetMousePos(wxMouseEvent&e) {
	long mx,my;
	e.GetPosition(&mx,&my);
	return Vec2(mx,my);
}
void Canvas::StabilizeCamera() {

	Vec2i wsize;
	GetSize(&wsize.width,&wsize.height);
	if(BOARD.camera.x<-wsize.width+100)
		BOARD.camera.x=-wsize.width+100;
	if(BOARD.camera.y<-wsize.height+100)
		BOARD.camera.y=-wsize.height+100;
	if(BOARD.camera.x>BOARD.zoom*BOARD.size.width-100)
		BOARD.camera.x=BOARD.zoom*BOARD.size.width-100;
	if(BOARD.camera.y>BOARD.zoom*BOARD.size.height-100)
		BOARD.camera.y=BOARD.zoom*BOARD.size.height-100;

}
