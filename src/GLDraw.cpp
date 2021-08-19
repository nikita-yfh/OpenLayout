#include "GLDraw.h"

GLCanvas::GLCanvas(wxWindow *parent)
		:wxGLCanvas(parent, wxID_ANY, nullptr,  wxDefaultPosition, wxDefaultSize, 0){
	Bind(wxEVT_PAINT,&GLCanvas::Draw,this);
	Bind(wxEVT_MOUSEWHEEL,&GLCanvas::OnMouseWheel,this);
	Bind(wxEVT_MIDDLE_DOWN,&GLCanvas::OnMiddleDown,this);
	Bind(wxEVT_MOTION,&GLCanvas::OnMouseMotion,this);
}

void GLCanvas::SetColor(const Color &color){
	glColor4ub(color.Red(),color.Green(),color.Blue(),color.Alpha());
}

void GLCanvas::SetColor(uint8_t color){
	SetColor(s.get_color(color));
}
void GLCanvas::SetGroundColor(uint8_t color){
	Color c=s.get_color(color);
	const float dark_ratio=0.6f;
	if(s.gen_settings[S_DARK_GROUND]){
		c.Set(c.Red()*dark_ratio,
				c.Green()*dark_ratio,
				c.Blue()*dark_ratio,
				c.Alpha());
	}
	SetColor(c);
}
void GLCanvas::SetLayerColor(const Object &o){
	if(o.metalisation && o.is_copper()){
		SetColor(COLOR_VIA);
	}else
		SetColor(o.layer-1);
}
void GLCanvas::SetDrillingsColor(){
	switch(s.drill){
	case DRILL_BGR:
		SetColor(COLOR_BGR);
		break;
	case DRILL_BLACK:
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	case DRILL_WHITE:
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}
}
void GLCanvas::DrawGrid(const Board &board){
	if(board.active_grid_val*board.zoom<=6)return;
	int subgrids_count=0;
	switch(s.sub_grid){
		case SUBGRID_2:subgrids_count=2;break;
		case SUBGRID_4:subgrids_count=4;break;
		case SUBGRID_5:subgrids_count=5;break;
		case SUBGRID_10:subgrids_count=10;break;
	}
	auto draw_line=[=](int n,float x1,float y1,float x2,float y2){
		glLineWidth((n%subgrids_count==0)?2:1);
		glBegin(GL_LINES);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
		glEnd();
	};
	SetColor(COLOR_LINES);
	int n=0;
	for(double x=board.anchor.x;x<board.size.width;x+=board.active_grid_val)
		draw_line(n++,x,0,x,board.size.height);
	n=0;
	for(double y=-board.anchor.y;y<board.size.height;y+=board.active_grid_val)
		draw_line(n++,0,y,board.size.width,y);
	n=0;
	for(double x=board.anchor.x;x>=0.0;x-=board.active_grid_val)
		draw_line(n++,x,0,x,board.size.height);
	n=0;
	for(double y=-board.anchor.y;y>=0.0;y-=board.active_grid_val)
		draw_line(n++,0,y,board.size.width,y);

}
void GLCanvas::DrawConnections(const Board &board){
	glLineWidth(2);
	SetColor(COLOR_CON);
	for(const Object &o : board.objects){
		if(o.can_connect()){
			for(const int32_t &n : o.connections){
				const Object &o2=board.objects[n];
				glBegin(GL_LINES);
					glVertex2f(o.pos.x,-o.pos.y);
					glVertex2f(o2.pos.x,-o2.pos.y);
				glEnd();
			}
		}
	}
}
void GLCanvas::DrawObject(const Object &o,float d){
	switch(o.type){
	case OBJ_LINE:{
		uint8_t styles[2]={o.get_begin_style(),o.get_end_style()};
		for(int q=0;q<o.poly_points.size()-1;q++){
			const Vec2&p1=o.poly_points[q];
			const Vec2&p2=o.poly_points[q+1];
			DrawLine(p1,p2,o.line_width+d*2.0f,q==0&&styles[0]==END_SQUARE,
				q==o.poly_points.size()-2 &&styles[1]==END_SQUARE);
			if(q>0 || styles[0]==END_ROUND)
				DrawCircle(p1,o.line_width/2.0f+d);
		}
		if(styles[1]==END_ROUND)
			DrawCircle(vend(o.poly_points),o.line_width/2.0f+d);
		}break;
	}
}
void GLCanvas::DrawPad(const Object &o,float d){
	switch(o.type){
	case OBJ_THT_PAD:
		switch(o.tht_shape%3){
		case 1: //circle
			DrawCircle(o.pos,o.size.outer+d);
			break;
		case 2: case 0:
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(o.pos.x,-o.pos.y);
			for(int q=0;q<=o.poly_points.size();q++){
				const Vec2 &v=o.poly_points[q%o.poly_points.size()];
				glVertex2f(v.x,-v.y);
			}
			glEnd();
			break;
		}
		break;
	case OBJ_SMD_PAD:
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(o.pos.x,-o.pos.y);
		for(int q=0;q<=o.poly_points.size();q++){
			const Vec2 &v=o.poly_points[q%o.poly_points.size()];
			glVertex2f(v.x,-v.y);
		}
		glEnd();
		break;
	}
}
void GLCanvas::DrawCircle(Vec2 pos,float r){
	pos.y=-pos.y;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(pos.x,pos.y);
    for(int i = 0; i < s.circle_quality+1; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(s.circle_quality);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + pos.x, y + pos.y);
    }
    glEnd();
}
void GLCanvas::DrawLine(Vec2 p1,Vec2 p2,float width,bool s1,bool s2){
	Vec2 c=(p2-p1);
	c.Normalize(width/2.0f);

	Vec2 d=c.Skew();

	Vec2 c1(0.0f,0.0f);
	Vec2 c2(0.0f,0.0f);

	if(s1)c1=c;
	if(s2)c2=c;


	Vec2 points[]={
		p1-d-c1,
		p1+d-c1,
		p2+d+c2,
		p2-d+c2
	};
	glBegin(GL_QUADS);
	for(int q=0;q<4;q++){
		glVertex2f(points[q].x,-points[q].y);
	}
	glEnd();
}
void GLCanvas::Draw(wxPaintEvent&){
    static wxGLContext context(this);
	SetCurrent(context);

    glMatrixMode(GL_PROJECTION);

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, GetSize().x, GetSize().y);

	glLoadIdentity();


    const Board &board=file.GetSelectedBoard();

	glOrtho(0.0f, GetSize().x/board.zoom, GetSize().y/board.zoom, 0.0f, 0.0f, 1.0f);
	glTranslated(-(board.camera.x/board.zoom),-(board.camera.y/board.zoom),0.0f);

	if(board.ground_pane[board.active_layer-1])
		SetGroundColor(board.active_layer-1);
	else
		SetColor(COLOR_BGR);

	glRectf(0.0f,0.0f,board.size.x,board.size.y);

	for(Object &o : file.GetSelectedBoard().objects){
		if(o.layer!=board.active_layer)continue;
		SetColor(COLOR_BGR);
		DrawObject(o,o.ground_distance);
	}

	if(board.ground_pane[board.active_layer-1]){
		//draw thermal pads
		SetGroundColor(board.active_layer-1);
		for(Object &o : file.GetSelectedBoard().objects){
			if(o.thermobarier){
				if(o.type==OBJ_THT_PAD){
					float width=o.thsize/300.0f*o.size.outer;
					glPushMatrix();
					glTranslatef(o.pos.x,-o.pos.y,0.0f);
					glRotatef(-o.get_angle()+90.0f,0.0f,0.0f,1.0f);
					for(int q=0;q<8;q++){
						if((o.th_style[0]>>q)&1)
							glRectf(0,-width,-o.size.outer-o.ground_distance,width);
						glRotatef(45.0f,0.0f,0.0f,1.0f);
					}
					glPopMatrix();
				}
			}
		}
	}
	glDisable(GL_MULTISAMPLE);
	DrawGrid(board);
	glEnable(GL_MULTISAMPLE);

	const uint8_t layers[4][7]={
		{LAY_I1,LAY_I2,LAY_C2,LAY_C1,LAY_S2,LAY_S1,LAY_O},
		{LAY_C1,LAY_C2,LAY_I2,LAY_I1,LAY_S1,LAY_S2,LAY_O},
		{LAY_C1,LAY_C2,LAY_I1,LAY_I2,LAY_S1,LAY_S2,LAY_O},
		{LAY_I1,LAY_I2,LAY_C1,LAY_C2,LAY_S1,LAY_S2,LAY_O}
	};

	int n;
	switch(board.active_layer){
		case LAY_C1: case LAY_S1:	n=0; break;
		case LAY_I1:				n=1; break;
		case LAY_I2:				n=2; break;
		default:					n=3; break;
	}

	for(int q=0;q<7;q++)
		for(Object &o : file.GetSelectedBoard().objects)
			if(o.layer==layers[n][q] && !o.can_connect()){
				SetLayerColor(o);
				DrawObject(o,0.0f);
			}
	for(int q=0;q<7;q++)//pads are drawing after other objects
		for(Object &o : file.GetSelectedBoard().objects)
			if(o.layer==layers[n][q] && o.can_connect()){
				SetLayerColor(o);
				DrawPad(o,0.0f);
			}
	for(Object &o : file.GetSelectedBoard().objects)//draw drillings
		if(o.type==OBJ_THT_PAD){
			SetDrillingsColor();
			DrawCircle(o.pos,o.size.inner);
		}

	DrawConnections(board);

    glFlush();
    SwapBuffers();
}
void GLCanvas::OnMouseWheel(wxMouseEvent&e){ //zooming canvas
	Vec2 mouse=GetMousePos(e);
    Board &board=file.GetSelectedBoard();

    const float zoom_ratio=1.3f;
    float zoom_old=board.zoom;


	if(e.GetWheelRotation()>0)
		board.zoom*=zoom_ratio;
	else
		board.zoom/=zoom_ratio;
	if(board.zoom<0.0001)
		board.zoom=0.0001;

	board.camera=((mouse+board.camera)/zoom_old*board.zoom)-mouse;

	Refresh();
}
void GLCanvas::OnMiddleDown(wxMouseEvent&e){
	clickboardpos=file.GetSelectedBoard().camera;
	clickmousepos=GetMousePos(e);
}
void GLCanvas::OnMouseMotion(wxMouseEvent&e){
    Board &board=file.GetSelectedBoard();

	if(e.MiddleIsDown()){
		board.camera=clickboardpos+(clickmousepos-GetMousePos(e));
		Refresh();
	}
}
Vec2 GLCanvas::GetPos(Vec2 mouse){
    const Board &board=file.GetSelectedBoard();

	return ((mouse-Vec2(board.camera.x,board.camera.y))/board.zoom).SwapY();
}
Vec2 GLCanvas::GetMousePos(wxMouseEvent&e){
	long mx,my;
	e.GetPosition(&mx,&my);
	return Vec2(mx,my);
}
