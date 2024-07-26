#include "GridPanel.h"
#include "Locale.h"

#include "xpm/leftpanel/ch_grid.xpm"

static const int NORMAL_GRIDS_COUNT = 8;
static const double normalGrids[] = {
	0.0396875, 0.079375, 0.15875,
	0.3175, 0.635, 1.27, 2.54, 5.08
};

static const int METRIC_GRIDS_COUNT = 11;
static const double metricGrids[] = {
	0.01,	0.02,	0.025,	0.05,
	0.1,	0.2,	0.25,	0.5,
	1.0,	2.0,	2.5
};

GridPanel::GridPanel(QWidget *parent) : QToolBar(_("Grid"), parent) {
    QToolButton *gridButton = new QToolButton(this);
    gridButton->setIcon(QIcon(QPixmap(ch_grid_xpm)));
    gridButton->setText(_("1.27 mm"));
    gridButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    gridButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    gridAct = addWidget(gridButton);

    setOrientation(Qt::Vertical);
}
