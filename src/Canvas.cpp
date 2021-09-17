#include "Canvas.h"
#include "OpenLayoutApp.h"
#include <wx/sizer.h>
const int attribList[] = {WX_GL_RGBA,
						  //WX_GL_DOUBLEBUFFER,
						  WX_GL_SAMPLE_BUFFERS, GL_FALSE,
						  WX_GL_DEPTH_SIZE, 24, 0, 0
						 };
Canvas::Canvas(wxWindow *parent)
	:wxGLCanvas(parent,wxID_ANY,attribList) {
	SetFocus();
	Bind(wxEVT_MOUSEWHEEL,&Canvas::OnMouseWheel,this);
	Bind(wxEVT_MIDDLE_DOWN,&Canvas::OnMiddleDown,this);
	Bind(wxEVT_LEFT_DOWN,&Canvas::OnLeftDown,this);
	Bind(wxEVT_MOTION,&Canvas::OnMouseMotion,this);
	Bind(wxEVT_LEFT_UP,&Canvas::OnLeftUp,this);
	Bind(wxEVT_KEY_DOWN,&Canvas::OnKey,this);
	Bind(wxEVT_KEY_UP,&Canvas::OnKey,this);
	Bind(wxEVT_KILL_FOCUS,&Canvas::OnKillFocus,this);

	dragscroll_timer.Bind(wxEVT_TIMER, &Canvas::OnDragScroll, this);
	dragscroll_timer.Stop();
	tip=new TextTooltip(this);
	measure=new MeasureTooltip(this);
}
void Canvas::OnKillFocus(wxFocusEvent&e) {
	measure->Hide();
	tip->Hide();
	e.Skip();
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
	SetFocus();
	e.Skip();
}
void Canvas::OnLeftDown(wxMouseEvent&e) {

	Vec2 mouse=GetMousePos(e);
	Vec2 boardpos=GetPos(mouse);

	if(APP.selected_tool==TOOL_EDIT) {
		Object *firstsel=nullptr;

		for(Object &o : BOARD.objects)
			if(o.point_in(boardpos))
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
			sel_rect.SetP1(boardpos);
			sel_rect.SetP2(boardpos);
			selection=SEL_RECT;
		}
	} else if(APP.selected_tool==TOOL_MEASURE) {
		sel_rect.SetP1(BOARD.to_grid(boardpos,shift,ctrl));
		sel_rect.SetP2(BOARD.to_grid(boardpos,shift,ctrl));
		selection=SEL_MEASURE;
		measure->Show(BOARD.readable_coord(sel_rect.GetP2()));
	}
	dragscroll_timer.Start(5);
	tip->Hide();
	clickmousepos=mousepos=mouse;
	e.Skip();
}
void Canvas::OnLeftUp(wxMouseEvent&e) {

	if(APP.selected_tool==TOOL_EDIT) {
		if(selection==SEL_RECT)
			BOARD.select(sel_rect);
		Refresh();
	} else if(APP.selected_tool==TOOL_MEASURE) {
		Refresh();
		measure->Show(BOARD.readable_coord(sel_rect.GetP2()));
	}
	selection=SEL_NONE;
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
		} else if(selection==SEL_RECT)
			sel_rect.SetP2(GetPos(mousepos));
		else
			sel_rect.SetP2(BOARD.to_grid(GetPos(mousepos),shift,ctrl));
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
	Vec2 mouse=GetMousePos(e);
	Vec2 boardpos=GetPos(mouse);

	if(APP.selected_tool==TOOL_EDIT) {
		if(e.LeftIsDown()) {
			if(selection==SEL_RECT) {
				sel_rect.SetP2(boardpos);
			} else if(selection==SEL_OBJECT)
				MoveObjects(mouse);
			Refresh();
		}

		bool ok=false;

		if(selection==SEL_NONE && !BOARD.is_selected())
			for(Object &o : BOARD.objects)
				if(o.point_in(boardpos) && !o.marker.empty()) {
					tip->Show(o.marker);
					ok=true;
					break;
				}
		if(!ok)tip->Hide();
		if(selection==SEL_OBJECT)
			measure->Show(BOARD.readable_coord(boardpos));
		else measure->Hide();
	} else if(APP.selected_tool==TOOL_MEASURE) {
		sel_rect.SetP2(BOARD.to_grid(boardpos,shift,ctrl));
		if(selection==SEL_MEASURE)
			measure->Show(BOARD.readable_coord(sel_rect.GetP2()),BOARD.readable_coord(sel_rect.GetP1()),true);
		else {
			measure->Show(BOARD.readable_coord(sel_rect.GetP2()));
			Refresh();
		}
	}
	if(e.LeftIsDown())
		mousepos=mouse;
	else if(e.MiddleIsDown()) {
		BOARD.camera=clickboardpos+(clickmousepos-mouse);
		StabilizeCamera();
		Refresh();
	}
	APP.mouse_board_pos=BOARD.readable_coord(boardpos);
	e.Skip();
}
void Canvas::OnKey(wxKeyEvent &e) {
	ctrl=e.ControlDown();
	shift=e.ShiftDown();
	if(selection==SEL_OBJECT)
		Refresh();
	if(!shift && !ctrl){
		const char keycode=e.GetKeyCode();
		if(keycode>='1' && keycode<='9'){
			BOARD.active_grid_val=SETTINGS.grids_bind[keycode-'1'];
			Refresh();
		}else{
			for(int q=0;q<TOOL_COUNT;q++){
				const char &key=SETTINGS.tool_keys[q];
				if(key==keycode){
					APP.selected_tool=q;
					Refresh();
					break;
				}
			}
		}

	}
	e.Skip();
}
Vec2 Canvas::GetPos(Vec2i mouse) {
	return ((mouse+Vec2(BOARD.camera.x,BOARD.camera.y))/BOARD.zoom).SwapY();
}
Vec2i Canvas::GetMousePos(wxMouseEvent&e) {
	long mx,my;
	e.GetPosition(&mx,&my);
	return Vec2i(mx,my);
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
void Canvas::UpdateSettings() {
	measure->UpdateSettings();
}
