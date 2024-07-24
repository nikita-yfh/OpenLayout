#include "ToolPanel.h"
#include "Locale.h"

#include <QLayout>

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

MyToolButton::MyToolButton(QWidget *parent, QIcon icon, const char *text) : QToolButton(parent) {
    setIcon(icon);
    setText(text);
    setCheckable(true);

    connect(parent, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(onChangeOrientation(Qt::Orientation)));
    connect(this, SIGNAL(pressed()), parent, SLOT(onButtonClicked()));
}

void MyToolButton::onChangeOrientation(Qt::Orientation orientation) {
    if(orientation == Qt::Vertical) {
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

ToolPanel::ToolPanel (QWidget *parent) : QToolBar(_("Tools"), parent) {
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    actions[Edit]           = addWidget(new MyToolButton(this, QIcon(QPixmap(edit_xpm)),          _("Edit")));
    actions[Zoom]           = addWidget(new MyToolButton(this, QIcon(QPixmap(zoom_xpm)),          _("Zoom")));
    actions[Track]          = addWidget(new MyToolButton(this, QIcon(QPixmap(track_xpm)),         _("Track")));
    actions[Pad]            = addWidget(new MyToolButton(this, QIcon(QPixmap(pad_circle_xpm)),    _("Pad")));
    actions[SMDPad]         = addWidget(new MyToolButton(this, QIcon(QPixmap(smd_xpm)),           _("SMD-Pad")));
    actions[Circle]         = addWidget(new MyToolButton(this, QIcon(QPixmap(circle_xpm)),        _("Circle")));
    actions[Rect]           = addWidget(new MyToolButton(this, QIcon(QPixmap(rect_track_xpm)),    _("Rectangle")));
    actions[Zone]           = addWidget(new MyToolButton(this, QIcon(QPixmap(polygon_xpm)),       _("Zone")));
    actions[SpecialForm]    = addWidget(new MyToolButton(this, QIcon(QPixmap(special_xpm)),       _("Special form")));
    actions[Text]           = addWidget(new MyToolButton(this, QIcon(QPixmap(text_xpm)),          _("Text")));
    actions[SolderMask]     = addWidget(new MyToolButton(this, QIcon(QPixmap(mask_xpm)),          _("Solder mask")));
    actions[Connections]    = addWidget(new MyToolButton(this, QIcon(QPixmap(connections_xpm)),   _("Connections")));
    actions[Autoroute]      = addWidget(new MyToolButton(this, QIcon(QPixmap(autoroute_xpm)),     _("Autoroute")));
    actions[Test]           = addWidget(new MyToolButton(this, QIcon(QPixmap(test_xpm)),          _("Test")));
    actions[Measure]        = addWidget(new MyToolButton(this, QIcon(QPixmap(measure_xpm)),       _("Measure")));
    actions[Photoview]      = addWidget(new MyToolButton(this, QIcon(QPixmap(photoview_xpm)),     _("Photoview")));

    setOrientation(Qt::Vertical);
}

void ToolPanel::onButtonClicked() {
    for(QAction *action : actions)
        action->setChecked(false);
}
