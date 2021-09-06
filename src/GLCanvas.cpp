#include "GLCanvas.h"
#include "OpenLayoutApp.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
#include "earcut.hpp"

namespace mapbox {
namespace util {

template <>
struct nth<0, Vec2> {
	inline static auto get(const Vec2 &t) {
		return t.x;
	};
};
template <>
struct nth<1, Vec2> {
	inline static auto get(const Vec2 &t) {
		return t.y;
	};
};

}
}

GLCanvas::GLCanvas(wxWindow *parent)
	:Canvas(parent) {
	Bind(wxEVT_PAINT,&GLCanvas::Draw,this);
}

void GLCanvas::SetColor(const Color &color) {
	glColor4ub(color.Red(),color.Green(),color.Blue(),color.Alpha());
}

void GLCanvas::SetColor(uint8_t color) {
	SetColor(SETTINGS.get_color(color));
}
void GLCanvas::SetGroundColor(uint8_t color) {
	Color c=SETTINGS.get_color(color);
	const float dark_ratio=0.6f;
	if(SETTINGS.gen_settings[S_DARK_GROUND]) {
		c.Set(c.Red()*dark_ratio,
			  c.Green()*dark_ratio,
			  c.Blue()*dark_ratio,
			  c.Alpha());
	}
	SetColor(c);
}
void GLCanvas::SetLayerColor(const Object &o) {
	if(o.metalisation && o.is_copper()) {
		SetColor(COLOR_VIA);
	} else
		SetColor(o.layer-1);
}
void GLCanvas::SetDrillingsColor() {
	switch(SETTINGS.drill) {
	case DRILL_BGR:
		SetColor(COLOR_BGR);
		break;
	case DRILL_BLACK:
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	case DRILL_WHITE:
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}
}
void GLCanvas::DrawGrid() {
	double grid=BOARD.active_grid_val;
	if(shift && selection==SEL_OBJECT)grid/=2.0;
	if(grid*BOARD.zoom<=6.0)return;
	int subgrids_count=0;
	switch(SETTINGS.sub_grid) {
	case SUBGRID_2:
		subgrids_count=2;
		break;
	case SUBGRID_4:
		subgrids_count=4;
		break;
	case SUBGRID_5:
		subgrids_count=5;
		break;
	case SUBGRID_10:
		subgrids_count=10;
		break;
	}
	if(shift && selection==SEL_OBJECT)subgrids_count*=2;
	auto draw_line=[=](int n,float x1,float y1,float x2,float y2) {
		glLineWidth((n%subgrids_count==0)?2:1);
		glBegin(GL_LINES);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
		glEnd();
	};
	SetColor(COLOR_LINES);
	int n=0;
	for(double x=BOARD.anchor.x; x<BOARD.size.width; x+=grid)
		draw_line(n++,x,0,x,BOARD.size.height);
	n=0;
	for(double y=-BOARD.anchor.y; y<BOARD.size.height; y+=grid)
		draw_line(n++,0,y,BOARD.size.width,y);
	n=0;
	for(double x=BOARD.anchor.x; x>=0.0; x-=grid)
		draw_line(n++,x,0,x,BOARD.size.height);
	n=0;
	for(double y=-BOARD.anchor.y; y>=0.0; y-=grid)
		draw_line(n++,0,y,BOARD.size.width,y);
}
void GLCanvas::DrawConnections() {
	glLineWidth(1.5f);
	SetColor(COLOR_CON);
	for(const Object &o : BOARD.objects) {
		if(o.can_connect()) {
			for(const int32_t &n : o.connections) {
				const Object &o2=BOARD.objects[n];
				glBegin(GL_LINES);
				glVertex2f(o.pos.x,-o.pos.y);
				glVertex2f(o2.pos.x,-o2.pos.y);
				glEnd();
			}
		}
	}
}
void GLCanvas::DrawObject(const Object &o,float d) {
	switch(o.type) {
	case OBJ_LINE: {
		uint8_t styles[2]= {o.get_begin_style(),o.get_end_style()};
		float width=o.line_width;
		if(width==0)
			width=1/BOARD.zoom;
		for(int q=0; q<o.poly_points.size()-1; q++) {
			const Vec2&p1=o.poly_points[q];
			const Vec2&p2=o.poly_points[q+1];
			DrawLine(p1,p2,width+d*2.0f,q==0&&styles[0]==END_SQUARE,
					 q==o.poly_points.size()-2 &&styles[1]==END_SQUARE);
			if(q>0 || styles[0]==END_ROUND)
				DrawCircle(p1,width/2.0f+d);
		}
		if(styles[1]==END_ROUND)
			DrawCircle(vend(o.poly_points),width/2.0f+d);

	}
	break;
	case OBJ_CIRCLE: {
		/* Triangle strip:
				outer
			0---2---4---6---8 ...
			 \ / \ / \ / \ /  ...
			  1---3---5---7   ...
				inner
		*/
		float total_angle=delta_angle(o.start_angle,o.end_angle)/1000.0f;
		float inner=o.size.inner;
		float outer=o.size.outer;
		float r=o.get_radius();
		float width=outer-inner+d*2.0f;
		if(inner==outer)
			outer=inner+1.0/BOARD.zoom; //always 1 px width
		glPushMatrix();
		glTranslatef(o.pos.x,-o.pos.y,0.0f);
		glRotatef(360.0f-o.start_angle/1000.0f,0.0f,0.0f,1.0f);
		{
			if(o.fill) {
				glBegin(GL_TRIANGLE_FAN);
				glVertex2f(0.0f,0.0f);
			} else
				glBegin(GL_TRIANGLE_STRIP);
			for(float theta = 0; theta <= total_angle; theta+=(total_angle/SETTINGS.circle_quality)) {
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
		glPopMatrix();
		Vec2 circles[2];
		o.get_ending_circles(circles);
		for(int q=0; q<2; q++)
			DrawCircle(circles[q],width/2.0f);
	}
	break;
	case OBJ_POLY: {
		vector<vector<Vec2>> polygon;
		polygon.push_back(o.poly_points);
		vector<uint16_t> indices = mapbox::earcut<uint16_t>(polygon);
		glBegin(GL_TRIANGLES);
		for(int q=0; q<indices.size(); q++) {
			const Vec2&p=o.poly_points[indices[q]];
			glVertex2f(p.x,-p.y);
		}
		glEnd();
		if(o.line_width) {
			for(int q=0; q<o.poly_points.size(); q++) {
				const Vec2&p1=o.poly_points[q];
				const Vec2&p2=o.poly_points[(q+1)%o.poly_points.size()];
				DrawLine(p1,p2,o.line_width+d*2.0f,false,false);
				DrawCircle(p1,o.line_width/2.0f+d);
			}
		}
	}
	break;
	}
	DrawPad(o,d);
}
void GLCanvas::DrawPad(const Object &o,float d) {
	switch(o.type) {
	case OBJ_THT_PAD:
		switch(o.tht_shape) {
		case 1: //circle
			DrawCircle(o.pos,o.size.outer+d);
			break;
		case 4:
		case 7: { //oval
			for(const Vec2 &p : o.poly_points)
				DrawCircle(p,o.size.outer+d);
			Vec2 d=(o.poly_points[1]-o.poly_points[0]).Skew();
			d.Normalize(o.size.outer);
			const Vec2 points[]= {
				o.poly_points[0]-d,
				o.poly_points[1]-d,
				o.poly_points[1]+d,
				o.poly_points[0]+d
			};
			glBegin(GL_QUADS);
			for(const Vec2 &p : points)
				glVertex2f(p.x,-p.y);
			glEnd();
		}
		break;
		default: //square and octagon
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(o.pos.x,-o.pos.y);
			for(int q=0; q<=o.poly_points.size(); q++) {
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
		for(int q=0; q<=o.poly_points.size(); q++) {
			const Vec2 &v=o.poly_points[q%o.poly_points.size()];
			glVertex2f(v.x,-v.y);
		}
		glEnd();
		break;
	}
}
void GLCanvas::DrawDrillings(const Object &o) {
	if(!o.cutoff &&o.type==OBJ_THT_PAD) {
		SetDrillingsColor();
		DrawCircle(o.pos,o.size.inner);
		if(o.size.inner==o.size.outer) {
			if(o.selected)
				SetColor(COLOR_SELO);
			else
				glColor4f(1.0f,1.0f,1.0f,1.0f);
			glLineWidth(1.5f);
			glBegin(GL_LINE_LOOP);
			for(int i = 0; i < SETTINGS.circle_quality; i++) {
				float theta = 2.0f * M_PI * float(i) / float(SETTINGS.circle_quality);
				float x = o.size.inner * cosf(theta);
				float y = o.size.inner * sinf(theta);
				glVertex2f(x + o.pos.x, y - o.pos.y);
			}
			glEnd();
			if(!o.selected)
				SetLayerColor(o);
			float d=o.size.inner/1.6f;
			glBegin(GL_LINES);
			glVertex2f(o.pos.x+d,-o.pos.y+d);
			glVertex2f(o.pos.x-d,-o.pos.y-d);
			glVertex2f(o.pos.x+d,-o.pos.y-d);
			glVertex2f(o.pos.x-d,-o.pos.y+d);
			glEnd();
		}
	}
}
void GLCanvas::DrawCircle(Vec2 pos,float r) {
	pos.y=-pos.y;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(pos.x,pos.y);
	for(int i = 0; i < SETTINGS.circle_quality+1; i++) {
		float theta = 2.0f * M_PI * float(i) / float(SETTINGS.circle_quality);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(x + pos.x, y + pos.y);
	}
	glEnd();
}
void GLCanvas::DrawLine(Vec2 p1,Vec2 p2,float width,bool s1,bool s2) {
	Vec2 points[4];
	expand_line(p1,p2,width,s1,s2,points);
	glBegin(GL_QUADS);
	for(int q=0; q<4; q++)
		glVertex2f(points[q].x,-points[q].y);
	glEnd();
}
void GLCanvas::Draw(wxPaintEvent&) {
	static wxGLContext context(this);
	SetCurrent(context);

	glMatrixMode(GL_PROJECTION);

	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, GetSize().x, GetSize().y);

	glLoadIdentity();

	glOrtho(0.0f, GetSize().x/BOARD.zoom, GetSize().y/BOARD.zoom, 0.0f, 0.0f, 1.0f);
	glTranslated(-(BOARD.camera.x/BOARD.zoom),-(BOARD.camera.y/BOARD.zoom),0.0f);

	if(BOARD.ground_pane[BOARD.active_layer-1])
		SetGroundColor(BOARD.active_layer-1);
	else
		SetColor(COLOR_BGR);

	glRectf(0.0f,0.0f,BOARD.size.x,BOARD.size.y);

	for(Object &o : BOARD.objects) {
		if(o.layer==BOARD.active_layer && BOARD.ground_pane[BOARD.active_layer-1]) {
			SetColor(COLOR_BGR);
			if(o.cutoff)
				DrawObject(o,0.0f);
			else
				DrawObject(o,o.ground_distance);
		}
	}

	if(BOARD.ground_pane[BOARD.active_layer-1]) {
		//draw thermal pads
		SetGroundColor(BOARD.active_layer-1);
		for(Object &o : BOARD.objects) {
			if(o.thermobarier) {
				if(o.type==OBJ_THT_PAD) {
					float width=o.thsize/300.0f*o.size.outer;
					glPushMatrix();
					glTranslatef(o.pos.x,-o.pos.y,0.0f);
					glRotatef(-o.get_angle()+90.0f,0.0f,0.0f,1.0f);
					for(int q=0; q<8; q++) {
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
	DrawGrid();
	glEnable(GL_MULTISAMPLE);

	const uint8_t layers[4][7]= {
		{LAY_I1,LAY_I2,LAY_C2,LAY_C1,LAY_S2,LAY_S1,LAY_O},
		{LAY_C1,LAY_C2,LAY_I2,LAY_I1,LAY_S1,LAY_S2,LAY_O},
		{LAY_C1,LAY_C2,LAY_I1,LAY_I2,LAY_S1,LAY_S2,LAY_O},
		{LAY_I1,LAY_I2,LAY_C1,LAY_C2,LAY_S1,LAY_S2,LAY_O}
	};

	int n;
	switch(BOARD.active_layer) {
	case LAY_C1:
	case LAY_S1:
		n=0;
		break;
	case LAY_I1:
		n=1;
		break;
	case LAY_I2:
		n=2;
		break;
	default:
		n=3;
		break;
	}

	for(int q=0; q<7; q++)
		for(Object &o : BOARD.objects)
			if(!o.cutoff && o.layer==layers[n][q] && !o.can_connect()) {
				if(o.selected)
					SetColor(COLOR_SELO);
				else
					SetLayerColor(o);
				DrawObject(o,0.0f);
			}
	for(int q=0; q<7; q++) //pads are drawing after other objects
		for(Object &o : BOARD.objects)
			if(!o.cutoff && o.layer==layers[n][q] && o.can_connect()) {
				if(o.selected)
					SetColor(COLOR_SELO);
				else
					SetLayerColor(o);
				DrawPad(o,0.0f);
			}
	for(Object &o : BOARD.objects)//draw drillings
		DrawDrillings(o);

	DrawConnections();
	DrawAnchor();
	DrawSelection();

	glFlush();
	SwapBuffers();
}
void GLCanvas::DrawAnchor() {
	glDisable(GL_MULTISAMPLE);
	Vec2i anchor=Vec2i(int(BOARD.anchor.x*BOARD.zoom)/BOARD.zoom,
					   int(BOARD.anchor.y*BOARD.zoom)/BOARD.zoom);
	const float anchor_isize=anchor_size-1.0f;
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glVertex2f(anchor.x,-anchor.y-anchor_size/BOARD.zoom);
	glVertex2f(anchor.x,-anchor.y+anchor_size/BOARD.zoom);
	glVertex2f(anchor.x-anchor_size/BOARD.zoom,-anchor.y);
	glVertex2f(anchor.x+anchor_size/BOARD.zoom,-anchor.y);
	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glBegin(GL_LINES);
	glVertex2f(anchor.x,-anchor.y-anchor_isize/BOARD.zoom);
	glVertex2f(anchor.x,-anchor.y+anchor_isize/BOARD.zoom);
	glVertex2f(anchor.x-anchor_isize/BOARD.zoom,-anchor.y);
	glVertex2f(anchor.x+anchor_isize/BOARD.zoom,-anchor.y);
	glEnd();
	DrawCircle(anchor,4.0/BOARD.zoom);
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex2f(anchor.x-2.0f/BOARD.zoom,-anchor.y-2.0f/BOARD.zoom);
	glVertex2f(anchor.x+2.0f/BOARD.zoom,-anchor.y-2.0f/BOARD.zoom);
	glVertex2f(anchor.x+2.0f/BOARD.zoom,-anchor.y+2.0f/BOARD.zoom);
	glVertex2f(anchor.x-2.0f/BOARD.zoom,-anchor.y+2.0f/BOARD.zoom);
	glEnd();
	glEnable(GL_MULTISAMPLE);
}
void GLCanvas::DrawSelection() {
	if(selection==SEL_RECT) {
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
