#include "PadTool.h"

#include "THTPad.h"
#include "SMDPad.h"

#include "Locale.h"
#include "xpm/leftpanel/pad_circle.xpm"
#include "xpm/leftpanel/smd.xpm"

PadTool::~PadTool() {
    board->CancelPlacing();
}

bool PadTool::OnLeftDown (const Vec2 &mousePos) {
    board->UnselectAll();
    return true;
}

bool PadTool::OnMouseMoved (const Vec2 &mousePos) {
    Object *tmpPad = board->GetFirstPlaced();
    if(!tmpPad) {
        tmpPad = CreatePad(mousePos);
        board->PlaceObject(tmpPad);
        prevMousePos = mousePos;
    }

    Vec2 mouseGrid = board->ToActiveGrid(mousePos);
    tmpPad->Move(prevMousePos - mousePos);
    prevMousePos = mousePos;
    return false;
}

Pad *SMDPadTool::CreatePad(const Vec2 &position) {
    return new SMDPad(board->GetSelectedLayer(),
            settings.groundDistance, position, settings.smdSize);
}

Pad *THTPadTool::CreatePad(const Vec2 &position) {
    return new THTPad(board->GetSelectedLayer(),
            settings.groundDistance, position,
            settings.padSize, settings.padShape,
            settings.metallization);
}

REGISTER_TOOL(THTPadTool, TOOL_THT_PAD, _("Pad"),     pad_circle_xpm);
REGISTER_TOOL(SMDPadTool, TOOL_SMD_PAD, _("SMD-Pad"), smd_xpm);
