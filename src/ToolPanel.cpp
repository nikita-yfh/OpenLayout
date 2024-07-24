#include "ToolPanel.h"
#include "Locale.h"

#include <QLayout>
#include <QToolButton>

#include "xpm/leftpanel/autoroute.xpm"
#include "xpm/leftpanel/ch_grid.xpm"
#include "xpm/leftpanel/ch_pad.xpm"
#include "xpm/leftpanel/ch_smd.xpm"
#include "xpm/leftpanel/ch_track.xpm"
#include "xpm/leftpanel/circle.xpm"
#include "xpm/leftpanel/connections.xpm"
#include "xpm/leftpanel/cross.xpm"
#include "xpm/leftpanel/edit.xpm"
#include "xpm/leftpanel/mask.xpm"
#include "xpm/leftpanel/measure.xpm"
#include "xpm/leftpanel/pad_circle_h.xpm"
#include "xpm/leftpanel/pad_circle_v.xpm"
#include "xpm/leftpanel/pad_circle.xpm"
#include "xpm/leftpanel/padm_circle_h.xpm"
#include "xpm/leftpanel/padm_circle_v.xpm"
#include "xpm/leftpanel/padm_circle.xpm"
#include "xpm/leftpanel/padm_octagon_h.xpm"
#include "xpm/leftpanel/padm_octagon_v.xpm"
#include "xpm/leftpanel/padm_octagon.xpm"
#include "xpm/leftpanel/padm_square_h.xpm"
#include "xpm/leftpanel/padm_square_v.xpm"
#include "xpm/leftpanel/padm_square.xpm"
#include "xpm/leftpanel/pad_octagon_h.xpm"
#include "xpm/leftpanel/pad_octagon_v.xpm"
#include "xpm/leftpanel/pad_octagon.xpm"
#include "xpm/leftpanel/pad_square_h.xpm"
#include "xpm/leftpanel/pad_square_v.xpm"
#include "xpm/leftpanel/pad_square.xpm"
#include "xpm/leftpanel/photoview.xpm"
#include "xpm/leftpanel/plus.xpm"
#include "xpm/leftpanel/polygon.xpm"
#include "xpm/leftpanel/rect_track.xpm"
#include "xpm/leftpanel/rect_zone.xpm"
#include "xpm/leftpanel/smd.xpm"
#include "xpm/leftpanel/special.xpm"
#include "xpm/leftpanel/swap_smd.xpm"
#include "xpm/leftpanel/test.xpm"
#include "xpm/leftpanel/text.xpm"
#include "xpm/leftpanel/track.xpm"
#include "xpm/leftpanel/zoom.xpm"

ToolPanel::ToolPanel (QWidget *parent) : QToolBar(_("Tools"), parent) {
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    actions[Edit]           = addAction(QIcon(QPixmap(edit_xpm)),          _("Edit"));
    actions[Zoom]           = addAction(QIcon(QPixmap(zoom_xpm)),          _("Zoom"));
    actions[Track]          = addAction(QIcon(QPixmap(track_xpm)),         _("Track"));
    actions[Pad]            = addAction(QIcon(QPixmap(pad_circle_xpm)),    _("Pad"));
    actions[SMDPad]         = addAction(QIcon(QPixmap(smd_xpm)),           _("SMD-Pad"));
    actions[Circle]         = addAction(QIcon(QPixmap(circle_xpm)),        _("Circle"));
    actions[Rect]           = addAction(QIcon(QPixmap(rect_track_xpm)),    _("Rectangle"));
    actions[Zone]           = addAction(QIcon(QPixmap(polygon_xpm)),       _("Zone"));
    actions[SpecialForm]    = addAction(QIcon(QPixmap(special_xpm)),       _("Special form"));
    actions[Text]           = addAction(QIcon(QPixmap(text_xpm)),          _("Text"));
    actions[SolderMask]     = addAction(QIcon(QPixmap(mask_xpm)),          _("Solder mask"));
    actions[Connections]    = addAction(QIcon(QPixmap(connections_xpm)),   _("Connections"));
    actions[Autoroute]      = addAction(QIcon(QPixmap(autoroute_xpm)),     _("Autoroute"));
    actions[Test]           = addAction(QIcon(QPixmap(test_xpm)),          _("Test"));
    actions[Measure]        = addAction(QIcon(QPixmap(measure_xpm)),       _("Measure"));
    actions[Photoview]      = addAction(QIcon(QPixmap(photoview_xpm)),     _("Photoview"));

    QActionGroup *group = new QActionGroup(this);
    for(QAction *action : actions) {
        action->setCheckable(true);
        action->setActionGroup(group);
        connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onChangeOrientation(Qt::Orientation)));
    }

    setOrientation(Qt::Vertical);
}

void ToolPanel::onChangeOrientation(Qt::Orientation orientation) {
    int minSize = 0;
    for(QAction *action : actions) {
        QToolButton *button = qobject_cast<QToolButton*>(widgetForAction(action));
        minSize = qMax(minSize, button->sizeHint().width());
    }
    for(QAction *action : actions) {
        QToolButton *button = qobject_cast<QToolButton*>(widgetForAction(action));
        if(button) {
            button->setMinimumWidth(minSize);
            if(orientation == Qt::Vertical) {
                button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
                button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            } else {
                button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
                button->setToolButtonStyle(Qt::ToolButtonIconOnly);
            }
        }
    }
}

