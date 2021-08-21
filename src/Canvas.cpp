#include "Canvas.h"
Canvas::Canvas(wxWindow *parent)
		:wxGLCanvas(parent, wxID_ANY, nullptr,  wxDefaultPosition, wxDefaultSize, 0){
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
void Canvas::OnMouseWheel(wxMouseEvent&e){ //zooming canvas
	Vec2 mouse=GetMousePos(e);
    Board &board=file.GetSelectedBoard();

    float zoom_old=board.zoom;
	if(e.GetWheelRotation()>0)
		board.zoom*=zoom_ratio;
	else
		board.zoom/=zoom_ratio;
	if(board.zoom<0.0001)
		board.zoom=0.0001;

	board.camera=((mouse+board.camera)/zoom_old*board.zoom)-mouse;
	StabilizeCamera();
	Refresh();
	e.Skip();
}
void Canvas::OnMiddleDown(wxMouseEvent&e){
	clickboardpos=file.GetSelectedBoard().camera;
	clickmousepos=GetMousePos(e);
	e.Skip();
}
void Canvas::OnLeftDown(wxMouseEvent&e){
    Board &board=file.GetSelectedBoard();
	Vec2 mouse=GetMousePos(e);
	Vec2 board_pos=GetPos(mouse);
	Object *firstsel=nullptr;

	for(Object &o : board.objects)
		if(o.point_in(board_pos))
			firstsel=&o;
	if((!firstsel || !firstsel->selected) && !shift)
		for(Object &o : board.objects)
			o.selected=false;

	if(firstsel){
		clickboardpos=firstsel->get_position();
		board.select(*firstsel);
		selection=SEL_OBJECT;
	}
	if(selection!=SEL_OBJECT){
		sel_rect.SetP1(board_pos);
		sel_rect.SetP2(board_pos);
		selection=SEL_RECT;
	}
	clickmousepos=mouse;
	dragscroll_timer.Start(5);
	Refresh();
	e.Skip();
}
void Canvas::OnLeftUp(wxMouseEvent&e){
    Board &board=file.GetSelectedBoard();

	if(selection==SEL_RECT)
		board.select(sel_rect);

	selection=SEL_NONE;
	Refresh();
	dragscroll_timer.Stop();
	e.Skip();
}
void Canvas::OnDragScroll(wxTimerEvent&e){
	if(selection!=SEL_NONE){
		Board &board=file.GetSelectedBoard();
		Vec2i wsize;
		GetSize(&wsize.width,&wsize.height);
		if(clickmousepos.x<0)board.camera.x-=10;
		if(clickmousepos.y<0)board.camera.y-=10;
		if(clickmousepos.x>wsize.x)board.camera.x+=10;
		if(clickmousepos.y>wsize.y)board.camera.y+=10;
		sel_rect.SetP2(GetPos(clickmousepos));
		Refresh();
	}
	e.Skip();
}
void Canvas::OnMouseMotion(wxMouseEvent&e){
    Board &board=file.GetSelectedBoard();
	bool refresh=true;
	Vec2 mouse=GetMousePos(e);
	if(e.MiddleIsDown()){
		board.camera=clickboardpos+(clickmousepos-mouse);
		StabilizeCamera();
	}else if(e.LeftIsDown()){
		clickmousepos=mouse;
		if(selection==SEL_RECT){
			sel_rect.SetP2(GetPos(mouse));
		}else if(selection==SEL_OBJECT){
			board.first_selected().set_position(clickboardpos+board.to_grid(GetPos(mouse)-clickboardpos,shift,ctrl));
		}
	}else refresh=false;
	if(refresh)Refresh();
	e.Skip();
}
void Canvas::OnKey(wxKeyEvent &e){
	ctrl=e.ControlDown();
	shift=e.ShiftDown();
	if(selection==SEL_OBJECT)
		Refresh();
	e.Skip();
}
Vec2 Canvas::GetPos(Vec2 mouse){
    Board &board=file.GetSelectedBoard();

	return ((mouse+Vec2(board.camera.x,board.camera.y))/board.zoom).SwapY();
}
Vec2 Canvas::GetMousePos(wxMouseEvent&e){
	long mx,my;
	e.GetPosition(&mx,&my);
	return Vec2(mx,my);
}
void Canvas::StabilizeCamera(){
    Board &board=file.GetSelectedBoard();
	Vec2i wsize;
	GetSize(&wsize.width,&wsize.height);
	if(board.camera.x<-wsize.width+100)
		board.camera.x=-wsize.width+100;
	if(board.camera.y<-wsize.height+100)
		board.camera.y=-wsize.height+100;
	if(board.camera.x>board.zoom*board.size.width-100)
		board.camera.x=board.zoom*board.size.width-100;
	if(board.camera.y>board.zoom*board.size.height-100)
		board.camera.y=board.zoom*board.size.height-100;

}