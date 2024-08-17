#include "GridPanel.h"
#include "Locale.h"

#include <QMenu>
#include <QActionGroup>
#include <QSignalMapper>

#include "xpm/leftpanel/ch_grid.xpm"

static const int GRID_VALUE_MASK = 0x0FFFF;
static const int GRID_TYPE_MASK  = 0xF0000;

enum {
    GRID_NORMAL_FLAG = 0x10000,
    GRID_METRIC_FLAG = 0x20000,
    GRID_USER_FLAG   = 0x40000
};

static const double normalGrids[] = {
	0.0396875, 0.079375, 0.15875,
	0.3175, 0.635, 1.27, 2.54, 5.08
};

static const double metricGrids[] = {
	0.01,	0.02,	0.025,	0.05,
	0.1,	0.2,	0.25,	0.5,
	1.0,	2.0,	2.5
};

GridPanel::GridPanel(Settings &settings, QWidget *parent)
        : QToolBar(_("Grid"), parent), settings(settings) {
    gridButton = new QToolButton(this);
    gridButton->setIcon(QIcon(QPixmap(ch_grid_xpm)));
    gridButton->setText("1.27 mm");
    gridButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    gridButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QAction *gridAct = addWidget(gridButton);
    connect(gridButton, SIGNAL(clicked()), this, SLOT(OnClick()));

    setOrientation(Qt::Vertical);
}

void GridPanel::OnClick() {
    QMenu *menu = new QMenu(this);
    QActionGroup *group = new QActionGroup(menu);

    QSignalMapper *setGridMapper = new QSignalMapper(menu);

    for(int i = 0; i < NORMAL_GRIDS_COUNT; i++) {
        QAction *action = new QAction(settings.GetGridStr(normalGrids[i]), menu);
        action->setCheckable(true);
        connect(action, SIGNAL(triggered(bool)), setGridMapper, SLOT(map()));
        setGridMapper->setMapping(action, i | GRID_NORMAL_FLAG);
        menu->addAction(action);
        group->addAction(action);
    }

    QMenu *metricMenu = menu->addMenu(_("Metric grids"));
    for(int i = 0; i < METRIC_GRIDS_COUNT; i++) {
        QAction *action = new QAction(settings.GetGridStr(metricGrids[i]), menu);
        action->setCheckable(true);
        connect(action, SIGNAL(triggered(bool)), setGridMapper, SLOT(map()));
        setGridMapper->setMapping(action, i | GRID_METRIC_FLAG);
        metricMenu->addAction(action);
        group->addAction(action);
    }

    QMenu *userMenu = menu->addMenu(_("User grids"));

    if(!settings.grids.Empty()) {
        for(int i = 0; i < settings.grids.Size(); i++) {
            QAction *action = new QAction(settings.GetGridStr(settings.grids[i]), menu);
            action->setCheckable(true);
            connect(action, SIGNAL(triggered(bool)), setGridMapper, SLOT(map()));
            setGridMapper->setMapping(action, i | GRID_USER_FLAG);
            userMenu->addAction(action);
            group->addAction(action);
        }
        userMenu->addSeparator();
    }

    QAction *userAdd = new QAction(_("Add new grid value"));
    userMenu->addAction(userAdd);

    if(!settings.grids.Empty()) {
        QMenu *userRemove = userMenu->addMenu(_("Remove"));
        for(int i = 0; i < settings.grids.Size(); i++) {
            QAction *action = new QAction(settings.GetGridStr(settings.grids[i]), menu);
            userRemove->addAction(action);
            group->addAction(action);
        }
    }

    connect(setGridMapper, SIGNAL(mappedInt(int)), this, SLOT(OnGridSetted(int)));

    menu->exec(gridButton->mapToGlobal(QPoint(gridButton->size().width(), 0)));
    delete menu;
}

void GridPanel::OnGridSetted(int value) {
    double grid = 0.0;

    int index = value & GRID_VALUE_MASK;
    if(value & GRID_NORMAL_FLAG)
        grid = normalGrids[index];
    else if(value & GRID_METRIC_FLAG)
        grid = metricGrids[index];
    else if(value & GRID_USER_FLAG)
        grid = settings.grids[index];
    else return;

    gridButton->setText(settings.GetGridStr(grid));
}

