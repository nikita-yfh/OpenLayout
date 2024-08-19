#include "ConnectionsTool.h"

#include "Locale.h"
#include "xpm/leftpanel/connections.xpm"

bool ConnectionsTool::OnLeftDown (const Vec2 &mousePos) {
    if(firstConnectionPointSelected && secondConnectionPad) {
        firstConnectionPad->AddConnection(secondConnectionPad);
        secondConnectionPad->AddConnection(firstConnectionPad);
        firstConnectionPad = secondConnectionPad = nullptr;
        firstConnectionPointSelected = false;
        board->UnselectAll();
    } else {
        if(firstConnectionPad)
            firstConnectionPointSelected = true;
    }
    return true;
}
bool ConnectionsTool::OnMouseMoved (const Vec2 &mousePos) {
    if(firstConnectionPointSelected) {
        if(secondConnectionPad)
            secondConnectionPad->Unselect();
        secondConnectionPad = board->TestPointPad(mousePos);
        if(secondConnectionPad)
            secondConnectionPad->Select();
    } else {
        if(firstConnectionPad)
            firstConnectionPad->Unselect();
        firstConnectionPad = board->TestPointPad(mousePos);
        if(firstConnectionPad)
            firstConnectionPad->Select();
    }
    return true;
}

REGISTER_TOOL(ConnectionsTool, TOOL_CONNECTIONS, _("Connections"), connections_xpm);
