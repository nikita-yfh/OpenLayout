#include "GridPanel.h"
#include "Locale.h"

#include "xpm/leftpanel/ch_grid.xpm"

GridPanel::GridPanel(QWidget *parent) : QToolBar(_("Grid"), parent) {
    QToolButton *gridButton = new QToolButton(this);
    gridButton->setIcon(QIcon(QPixmap(ch_grid_xpm)));
    gridButton->setText(_("1.27 mm"));
    gridButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    gridButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    gridAct = addWidget(gridButton);

    setOrientation(Qt::Vertical);
}
