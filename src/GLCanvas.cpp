#include "GLCanvas.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLCanvas::GLCanvas(wxWindow *parent)
		:Canvas(parent){
	Bind(wxEVT_PAINT,&GLCanvas::Draw,this);
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
	double grid=board.active_grid_val;
	if(shift && selection==SEL_OBJECT)grid/=2.0;
	if(grid*board.zoom<=6.0)return;
	int subgrids_count=0;
	switch(s.sub_grid){
		case SUBGRID_2:subgrids_count=2;break;
		case SUBGRID_4:subgrids_count=4;break;
		case SUBGRID_5:subgrids_count=5;break;
		case SUBGRID_10:subgrids_count=10;break;
	}
	if(shift && selection==SEL_OBJECT)subgrids_count*=2;
	auto draw_line=[=](int n,float x1,float y1,float x2,float y2){
		glLineWidth((n%subgrids_count==0)?2:1);
		glBegin(GL_LINES);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
		glEnd();
	};
	SetColor(COLOR_LINES);
	int n=0;
	for(double x=board.anchor.x;x<board.size.width;x+=grid)
		draw_line(n++,x,0,x,board.size.height);
	n=0;
	for(double y=-board.anchor.y;y<board.size.height;y+=grid)
		draw_line(n++,0,y,board.size.width,y);
	n=0;
	for(double x=board.anchor.x;x>=0.0;x-=grid)
		draw_line(n++,x,0,x,board.size.height);
	n=0;
	for(double y=-board.anchor.y;y>=0.0;y-=grid)
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
		float width=o.line_width;
		if(width==0)
			width=1/file.GetSelectedBoard().zoom;
		for(int q=0;q<o.poly_points.size()-1;q++){
			const Vec2&p1=o.poly_points[q];
			const Vec2&p2=o.poly_points[q+1];
			DrawLine(p1,p2,width+d*2.0f,q==0&&styles[0]==END_SQUARE,
				q==o.poly_points.size()-2 &&styles[1]==END_SQUARE);
			if(q>0 || styles[0]==END_ROUND)
				DrawCircle(p1,width/2.0f+d);
		}
		if(styles[1]==END_ROUND)
			DrawCircle(vend(o.poly_points),width/2.0f+d);
		}break;
	case OBJ_CIRCLE:{
		/* Triangle strip:
			0---2---4---6---8 ...
			 \ / \ / \ / \ /  ...
			  1---3---5---7   ...
		*/
		float total_angle=delta_angle(o.start_angle,o.end_angle)/1000.0f;
		float inner=o.size.inner;
		float outer=o.size.outer;
		float r=(inner+outer)/2.0f;
		float width=outer-inner+d*2.0f;
		if(inner==outer)
			outer=inner+1.0/file.GetSelectedBoard().zoom; //always 1 px width
		glPushMatrix();
			glTranslatef(o.pos.x,-o.pos.y,0.0f);
			glRotatef(360.0f-o.start_angle/1000.0f,0.0f,0.0f,1.0f);
			{
				if(o.fill){
					glBegin(GL_TRIANGLE_FAN);
					glVertex2f(0.0f,0.0f);
				}else
					glBegin(GL_TRIANGLE_STRIP);
				for(float theta = 0; theta <= total_angle; theta+=(total_angle/s.circle_quality)) {
					glVertex2f(
						(outer+d) * cosr(-theta),
						(outer+d) * sinr(-theta)
					);
					if(!o.fill)
					glVertex2f(
						(inner-d) * cosr(-theta),
						(inner-d) * sinr(-theta)
					);
				}
				if(o.fill)glVertex2f(r,0.0f);
				glEnd();
			}
			if(o.fill)
				DrawLine(Vec2(r,0.0f),Vec2(r*cosr(total_angle),r*sinr(total_angle)),width,false,false);

			DrawCircle(Vec2(r,0.0f),width/2.0f);
			glRotatef(-total_angle,0.0f,0.0f,1.0f);
			DrawCircle(Vec2(r,0.0f),width/2.0f);
		glPopMatrix();
		}break;
	}
	DrawPad(o,d);
}
void GLCanvas::DrawPad(const Object &o,float d){
	switch(o.type){
	case OBJ_THT_PAD:
		switch(o.tht_shape%3){
		case 1: //circle
			DrawCircle(o.pos,o.size.outer+d);
			break;
		case 2: case 0: //square and octagon
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
        float theta = 2.0f * M_PI * float(i) / float(s.circle_quality);
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
	for(int q=0;q<4;q++)
		glVertex2f(points[q].x,-points[q].y);
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
		if(o.layer==board.active_layer){
			SetColor(COLOR_BGR);
			if(o.cutoff)
				DrawObject(o,0.0f);
			else
				DrawObject(o,o.ground_distance);
		}
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
			if(!o.cutoff && o.layer==layers[n][q] && !o.can_connect()){
				if(o.selected)
					SetColor(COLOR_SELO);
				else
					SetLayerColor(o);
				DrawObject(o,0.0f);
			}
	for(int q=0;q<7;q++)//pads are drawing after other objects
		for(Object &o : file.GetSelectedBoard().objects)
			if(!o.cutoff && o.layer==layers[n][q] && o.can_connect()){
				if(o.selected)
					SetColor(COLOR_SELO);
				else
					SetLayerColor(o);
				DrawPad(o,0.0f);
			}
	for(Object &o : file.GetSelectedBoard().objects)//draw drillings
		if(!o.cutoff &&o.type==OBJ_THT_PAD){
			SetDrillingsColor();
			DrawCircle(o.pos,o.size.inner);
		}

	DrawConnections(board);
	DrawAnchor(board);
	DrawSelection(board);

    glFlush();
    SwapBuffers();
}
void GLCanvas::DrawAnchor(const Board &board){
	glDisable(GL_MULTISAMPLE);
	Vec2i anchor=Vec2i(int(board.anchor.x*board.zoom)/board.zoom,
						int(board.anchor.y*board.zoom)/board.zoom);
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
		glVertex2f(anchor.x,-anchor.y-15.0f/board.zoom);
		glVertex2f(anchor.x,-anchor.y+15.0f/board.zoom);
		glVertex2f(anchor.x-15.0f/board.zoom,-anchor.y);
		glVertex2f(anchor.x+15.0f/board.zoom,-anchor.y);
	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glBegin(GL_LINES);
		glVertex2f(anchor.x,-anchor.y-14.0f/board.zoom);
		glVertex2f(anchor.x,-anchor.y+14.0f/board.zoom);
		glVertex2f(anchor.x-14.0f/board.zoom,-anchor.y);
		glVertex2f(anchor.x+14.0f/board.zoom,-anchor.y);
	glEnd();
	DrawCircle(anchor,4.0/board.zoom);
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
		glVertex2f(anchor.x-2.0f/board.zoom,-anchor.y-2.0f/board.zoom);
		glVertex2f(anchor.x+2.0f/board.zoom,-anchor.y-2.0f/board.zoom);
		glVertex2f(anchor.x+2.0f/board.zoom,-anchor.y+2.0f/board.zoom);
		glVertex2f(anchor.x-2.0f/board.zoom,-anchor.y+2.0f/board.zoom);
	glEnd();
	glEnable(GL_MULTISAMPLE);
}
void GLCanvas::DrawSelection(const Board &board){
	if(selection==SEL_RECT){
		glEnable(GL_LINE_STIPPLE);
		glLineWidth (2);
		glLineStipple(1, 0x0F0F);
		SetColor(COLOR_SELR);
		glBegin(GL_LINE_LOOP);
			glVertex2f(sel_rect.x1,-sel_rect.y1);
			glVertex2f(sel_rect.x1,-sel_rect.y2);
			glVertex2f(sel_rect.x2,-sel_rect.y2);
			glVertex2f(sel_rect.x2,-sel_rect.y1);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		glEnable(GL_MULTISAMPLE);
	}
}
