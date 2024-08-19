#include "EditTool.h"

#include "Locale.h"
#include "xpm/leftpanel/edit.xpm"

bool EditTool::OnLeftDown (const Vec2 &mousePos) {
    leftPressed = true;
    Object *object = board->TestPoint(mousePos);
    if(!object || !object->IsSelected() || shiftPressed) {
        if(!shiftPressed)
            board->UnselectAll();
        if(object)
            board->InvertSelectionGroup(object);
    }
    if(object) {
        lastMousePos = clickPosition = object->GetNearestPoint(mousePos);
        drag = mousePos - lastMousePos;
    }
    return true;
}

bool EditTool::OnLeftUp (const Vec2 &mousePos) {
    clickPosition = Vec2::Invalid();
    leftPressed = false;
    return true;
}

bool EditTool::OnMouseMoved (const Vec2 &mousePos) {
    if(board->IsSelected() && leftPressed) {
        Vec2 _mouse = board->ToActiveGrid(mousePos - drag, clickPosition);
        Vec2 delta = _mouse - lastMousePos;
        lastMousePos = _mouse;
        board->MoveSelected(delta);
    }
    return true;
}

bool EditTool::OnModifiersChanged(bool shift, bool ctrl) {
    shiftPressed = shift;
    return false;
}

REGISTER_TOOL(EditTool, TOOL_EDIT, _("Edit"), edit_xpm);
