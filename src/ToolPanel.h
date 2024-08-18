#pragma once

#include "Locale.h"
#include "Settings.h"
#include "THTPad.h"

#include <QToolBar>
#include <QSignalMapper>

class ToolPanel : public QToolBar {
    Q_OBJECT
public:
    ToolPanel(Settings &settings, QWidget *parent = nullptr);
    ~ToolPanel() {}

    static const char *toolNames[TOOL_COUNT];

private:
    QAction *actions[TOOL_COUNT];
    QAction *padTypes[THTPad::S_COUNT];
    QAction *metallization;

    Settings &settings;

    void CreatePadShapeMenu();
    void CreateRectTypeMenu();
public slots:
    void OnChangeOrientation(Qt::Orientation orientation);
    void OnToggleMetallization(bool);
    void OnSettedRectTrack();
    void OnSettedRectZone();

    void OnPadShapeChanged(int);
    void OnToolChanged(int);
signals:
    void ToolChanged();
};
