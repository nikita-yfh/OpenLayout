#include "GridPanel.h"
#include "Locale.h"

#include <QMenu>
#include <QActionGroup>
#include <QSignalMapper>

#include "xpm/leftpanel/ch_grid.xpm"

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
        setGridMapper->setMapping(action, i);
        menu->addAction(action);
        group->addAction(action);
    }

    QMenu *metricMenu = menu->addMenu(_("Metric grids"));
    for(int i = 0; i < METRIC_GRIDS_COUNT; i++) {
        QAction *action = new QAction(settings.GetGridStr(metricGrids[i]), menu);
        action->setCheckable(true);
        connect(action, SIGNAL(triggered(bool)), setGridMapper, SLOT(map()));
        setGridMapper->setMapping(action, i + NORMAL_GRIDS_COUNT);
        metricMenu->addAction(action);
        group->addAction(action);
    }

    QMenu *userMenu = menu->addMenu(_("User grids"));

    if(!settings.grids.Empty()) {
        for(int i = 0; i < settings.grids.Size(); i++) {
            QAction *action = new QAction(settings.GetGridStr(settings.grids[i]), menu);
            action->setCheckable(true);
            connect(action, SIGNAL(triggered(bool)), setGridMapper, SLOT(map()));
            setGridMapper->setMapping(action, i + NORMAL_GRIDS_COUNT + METRIC_GRIDS_COUNT);
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

void GridPanel::OnGridSetted(int index) {
    double grid = 0.0;
    if(index < NORMAL_GRIDS_COUNT)
        grid = normalGrids[index];
    else if(index < METRIC_GRIDS_COUNT + NORMAL_GRIDS_COUNT)
        grid = metricGrids[index - NORMAL_GRIDS_COUNT];
    else if(index < settings.grids.Size() + METRIC_GRIDS_COUNT + NORMAL_GRIDS_COUNT)
        grid = settings.grids[index - NORMAL_GRIDS_COUNT - METRIC_GRIDS_COUNT];
    else return;

    gridButton->setText(settings.GetGridStr(grid));
}

