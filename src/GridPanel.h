#pragma once
#include <QToolBar>
#include <QToolButton>
#include <QSignalMapper>

class GridPanel : public QToolBar {
    Q_OBJECT
public:
    GridPanel(QWidget *parent = nullptr);
    ~GridPanel() {}
private:
    QAction *gridAct;
};
