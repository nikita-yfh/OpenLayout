#include "ToolPanel.h"
#include "Locale.h"

#include <QLayout>
#include <QToolButton>
#include <QMenu>

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

const char *ToolPanel::toolNames[TOOL_COUNT] = {
    _("Edit"),
    _("Zoom"),
    _("Track"),
    _("Pad"),
    _("SMD-Pad"),
    _("Circle"),
    _("Rectangle"),
    _("Zone"),
    _("Special form"),
    _("Text"),
    _("Solder mask"),
    _("Connections"),
    _("Autoroute"),
    _("Test"),
    _("Measure"),
    _("Photoview")
};

static const char * const *padBitmaps[2][ToolPanel::PAD_COUNT] = {
	{
		pad_circle_xpm,		pad_octagon_xpm,	pad_square_xpm,
		pad_circle_h_xpm,	pad_octagon_h_xpm,	pad_square_h_xpm,
		pad_circle_v_xpm,	pad_octagon_v_xpm,	pad_square_v_xpm,
	},
	{
		padm_circle_xpm,	padm_octagon_xpm,	padm_square_xpm,
		padm_circle_h_xpm,	padm_octagon_h_xpm,	padm_square_h_xpm,
		padm_circle_v_xpm,	padm_octagon_v_xpm,	padm_square_v_xpm,
	}
};

static const char * const *toolBitmaps[ToolPanel::TOOL_COUNT] = {
    edit_xpm,   
    zoom_xpm,   
    track_xpm,  
    pad_circle_xpm,
    smd_xpm,    
    circle_xpm, 
    rect_track_xpm,
    polygon_xpm,
    special_xpm,
    text_xpm,   
    mask_xpm,   
    connections_xpm,
    autoroute_xpm,
    test_xpm,   
    measure_xpm,
    photoview_xpm,
};

ToolPanel::ToolPanel (QWidget *parent) : QToolBar(_("Tools"), parent) {
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QActionGroup *group = new QActionGroup(this);

    for(int i = 0; i < TOOL_COUNT; i++) {
        actions[i] = addAction(QIcon(QPixmap(toolBitmaps[i])), toolNames[i]);
        actions[i]->setCheckable(true);
        actions[i]->setActionGroup(group);

        QToolButton *button = qobject_cast<QToolButton*>(widgetForAction(actions[i]));
    }

    connect(this, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(OnChangeOrientation(Qt::Orientation)));

    CreatePadTypeMenu();
    CreateRectTypeMenu();

    currentMetallization = false;
    currentRectFill = false;
    currentPadType = PAD_CIRCLE;

    setOrientation(Qt::Vertical);
}

void ToolPanel::CreatePadTypeMenu() {
    qobject_cast<QToolButton*>(widgetForAction(actions[TOOL_PAD]))->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu *padMenu = new QMenu(this);
    padTypes[PAD_CIRCLE]     = padMenu->addAction(_("Circular"));
    padTypes[PAD_OCTAGON]    = padMenu->addAction(_("Octagon"));
    padTypes[PAD_SQUARE]     = padMenu->addAction(_("Square"));
    padMenu->addSeparator();
    padTypes[PAD_CIRCLE_H]   = padMenu->addAction(_("Rounded, horizontal"));
    padTypes[PAD_OCTAGON_H]  = padMenu->addAction(_("Octagon, horizontal"));
    padTypes[PAD_SQUARE_H]   = padMenu->addAction(_("Rectangle, horizontal"));
    padMenu->addSeparator();
    padTypes[PAD_CIRCLE_V]   = padMenu->addAction(_("Rounded, vertical"));
    padTypes[PAD_OCTAGON_V]  = padMenu->addAction(_("Octagon, vertical"));
    padTypes[PAD_SQUARE_V]   = padMenu->addAction(_("Rectangle, vertical"));
    padMenu->addSeparator();

    padMapper = new QSignalMapper(padMenu);
    for(int i = 0; i < PAD_COUNT; i++) {
        connect(padTypes[i], SIGNAL(triggered(bool)), padMapper, SLOT(map()));
        padMapper->setMapping(padTypes[i], i);
        padTypes[i]->setIcon(QIcon(QPixmap(padBitmaps[0][i])));
    }
    connect(padMapper, SIGNAL(mappedInt(int)), this, SLOT(OnSettedPadType(int)));

    metallization            = padMenu->addAction(_("Through pad"));
    metallization->setCheckable(true);
    metallization->setShortcut(QKeySequence(Qt::Key_F12));
    connect(metallization, SIGNAL(toggled(bool)), this, SLOT(OnToggleMetallization(bool)));

    actions[TOOL_PAD]->setMenu(padMenu);
}

void ToolPanel::CreateRectTypeMenu() {
    qobject_cast<QToolButton*>(widgetForAction(actions[TOOL_RECT]))->setPopupMode(QToolButton::MenuButtonPopup);

    QMenu *rectMenu = new QMenu(this);
    QAction *rectTrackAct =  rectMenu->addAction(QIcon(QPixmap(rect_track_xpm)), _("Track"));
    QAction *rectZoneAct  =  rectMenu->addAction(QIcon(QPixmap(rect_zone_xpm)),  _("Zone"));

    connect(rectTrackAct, SIGNAL(triggered(bool)), this, SLOT(OnSettedRectTrack()));
    connect(rectZoneAct,  SIGNAL(triggered(bool)), this, SLOT(OnSettedRectZone()));

    actions[TOOL_RECT]->setMenu(rectMenu);
}

void ToolPanel::OnChangeOrientation(Qt::Orientation orientation) {
    int minSize = 0;
    if(orientation == Qt::Vertical)
        for(QAction *action : actions) {
            QToolButton *button = qobject_cast<QToolButton*>(widgetForAction(action));
            button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
            button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            minSize = qMax(minSize, button->sizeHint().width());
        }
    for(QAction *action : actions) {
        QToolButton *button = qobject_cast<QToolButton*>(widgetForAction(action));
        if(button) {
            button->setMinimumWidth(minSize);
            if(orientation == Qt::Horizontal) {
                button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
                button->setToolButtonStyle(Qt::ToolButtonIconOnly);
            }
        }
    }
}

void ToolPanel::OnToggleMetallization(bool checked) {
    currentMetallization = checked;
    for(int i = 0; i < PAD_COUNT; i++)
        padTypes[i]->setIcon(QIcon(QPixmap(padBitmaps[checked][i])));
    actions[TOOL_PAD]->setIcon(QIcon(QPixmap(padBitmaps[currentMetallization][currentPadType])));
}

void ToolPanel::OnSettedPadType(int type) {
    currentPadType = (PadType) type;
    actions[TOOL_PAD]->setIcon(QIcon(QPixmap(padBitmaps[currentMetallization][currentPadType])));
}

void ToolPanel::OnSettedRectTrack() {
    currentRectFill = false;
    actions[TOOL_RECT]->setIcon(QIcon(QPixmap(rect_track_xpm)));
}

void ToolPanel::OnSettedRectZone() {
    currentRectFill = true;
    actions[TOOL_RECT]->setIcon(QIcon(QPixmap(rect_zone_xpm)));
}
