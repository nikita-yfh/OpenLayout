#include "Tool.h"

Array<Tool::Uncreated> Tool::tools;

void Tool::OnDraw(const Vec2 &currentSize) const {
    board->Draw(settings, currentSize);
}

int Tool::RegisterTool(toolCreate_t func, int id, const char *name, xpm_t icon) {
    Uncreated tool { func, id, name, icon };
    tools.Add(tool);
    return tools.Size() - 1;
}

