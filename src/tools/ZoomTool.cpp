#include "ZoomTool.h"

#include "Locale.h"
#include "xpm/leftpanel/zoom.xpm"

bool ZoomTool::OnLeftDown (const Vec2 &mousePos) {
    board->Zoom(zoomRatioButtons, mousePos);
    return true;
}

bool ZoomTool::OnRightDown (const Vec2 &mousePos) {
    board->Zoom(1.0f / zoomRatioButtons, mousePos);
    return true;
}

bool ZoomTool::OnWheelRotated (const Vec2 &mousePos, int direction) {
	if(direction < 0)
        board->Zoom(1.0f / zoomRatioWheel, mousePos);
    else
        board->Zoom(zoomRatioWheel, mousePos);
    return true;
}

REGISTER_TOOL(ZoomTool, TOOL_ZOOM, _("Zoom"), zoom_xpm);
