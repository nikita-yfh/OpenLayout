#pragma once
#include <QToolBar>
#include <QToolButton>
#include <QSignalMapper>
#include <QAction>

#include "Settings.h"

class GridPanel : public QToolBar {
    Q_OBJECT
public:
    GridPanel(Settings &settings, QWidget *parent = nullptr);
    ~GridPanel() {}

    static const int NORMAL_GRIDS_COUNT = 8;
    static const int METRIC_GRIDS_COUNT = 11;
private:
    Settings &settings;

    QToolButton *gridButton;
private slots:
    void OnClick();
    void OnGridSetted(int index);
};
