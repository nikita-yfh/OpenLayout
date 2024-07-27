#include "MainWindow.h"
#include "Locale.h"

#include "SettingsDialog.h"

#include <QIcon>
#include <QAction>

#include "xpm/toolbar/align_bottom.xpm"
#include "xpm/toolbar/align_hcenter.xpm"
#include "xpm/toolbar/align_left.xpm"
#include "xpm/toolbar/align_right.xpm"
#include "xpm/toolbar/align_top.xpm"
#include "xpm/toolbar/align_vcenter.xpm"
#include "xpm/toolbar/align.xpm"
#include "xpm/toolbar/bitmap.xpm"
#include "xpm/toolbar/components.xpm"
#include "xpm/toolbar/copy.xpm"
#include "xpm/toolbar/cut.xpm"
#include "xpm/toolbar/delete.xpm"
#include "xpm/toolbar/drc.xpm"
#include "xpm/toolbar/duplicate.xpm"
#include "xpm/toolbar/group_off.xpm"
#include "xpm/toolbar/group_on.xpm"
#include "xpm/toolbar/info.xpm"
#include "xpm/toolbar/macro.xpm"
#include "xpm/toolbar/mirror_h.xpm"
#include "xpm/toolbar/mirror_v.xpm"
#include "xpm/toolbar/new.xpm"
#include "xpm/toolbar/open.xpm"
#include "xpm/toolbar/paste.xpm"
#include "xpm/toolbar/print.xpm"
#include "xpm/toolbar/properties.xpm"
#include "xpm/toolbar/redo.xpm"
#include "xpm/toolbar/remove_con.xpm"
#include "xpm/toolbar/rotate.xpm"
#include "xpm/toolbar/save.xpm"
#include "xpm/toolbar/selector.xpm"
#include "xpm/toolbar/to_grid.xpm"
#include "xpm/toolbar/transparent.xpm"
#include "xpm/toolbar/undo.xpm"
#include "xpm/toolbar/zoom_any.xpm"
#include "xpm/toolbar/zoom_board.xpm"
#include "xpm/toolbar/zoom_objects.xpm"
#include "xpm/toolbar/zoom_selection.xpm"
#include "xpm/toolbar/zoom_prev.xpm"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    CreateToolBar();
    CreateLeftPanel();
}

void MainWindow::CreateToolBar() {
    toolBarMain = addToolBar(_("File"));
    newAct          =        toolBarMain->addAction(QIcon(QPixmap(new_xpm)),        _("New"));
    openAct         =        toolBarMain->addAction(QIcon(QPixmap(open_xpm)),       _("Open"));
    saveAct         =        toolBarMain->addAction(QIcon(QPixmap(save_xpm)),       _("Save"));
    printAct        =        toolBarMain->addAction(QIcon(QPixmap(print_xpm)),      _("Print"));

    toolBarHistory = addToolBar(_("History"));
    undoAct         =     toolBarHistory->addAction(QIcon(QPixmap(undo_xpm)),       _("Undo"));
    redoAct         =     toolBarHistory->addAction(QIcon(QPixmap(redo_xpm)),       _("Redo"));

    toolBarClipboard = addToolBar(_("Clipboard"));
    cutAct          =   toolBarClipboard->addAction(QIcon(QPixmap(cut_xpm)),        _("Cut"));
    copyAct         =   toolBarClipboard->addAction(QIcon(QPixmap(copy_xpm)),       _("Copy"));
    pasteAct        =   toolBarClipboard->addAction(QIcon(QPixmap(paste_xpm)),      _("Paste"));
    deleteAct       =   toolBarClipboard->addAction(QIcon(QPixmap(delete_xpm)),     _("Delete"));

    toolBarActions = addToolBar(_("Actions"));
    duplicateAct    =    toolBarActions->addAction(QIcon(QPixmap(duplicate_xpm)),   _("Duplicate"));
    QAction *rotateAct = toolBarActions->addAction(QIcon(QPixmap(rotate_xpm)),      _("Rotate"));

    { // Create menu for rotate button
        QMenu *rotateMenu = new QMenu();
        rotate90Act = rotateMenu->addAction(_("90°"));
        rotate15Act = rotateMenu->addAction(_("15°"));
        rotate45Act = rotateMenu->addAction(_("45°"));
        rotate5Act  = rotateMenu->addAction(_("5°"));
        rotateMenu->addSeparator();
        rotateCustomAct = rotateMenu->addAction(_("22.5° ..."));

        rotate90Act->setCheckable(true);
        rotate45Act->setCheckable(true);
        rotate15Act->setCheckable(true);
        rotate5Act->setCheckable(true);
        rotateCustomAct->setCheckable(true);

        QActionGroup *rotateGroup = new QActionGroup(this);
        rotateGroup->addAction(rotate90Act);
        rotateGroup->addAction(rotate45Act);
        rotateGroup->addAction(rotate15Act);
        rotateGroup->addAction(rotate5Act);
        rotateGroup->addAction(rotateCustomAct);
        rotateGroup->setExclusive(true);

        rotateAct->setMenu(rotateMenu);
        qobject_cast<QToolButton*>(toolBarActions->widgetForAction(rotateAct))->setPopupMode(QToolButton::MenuButtonPopup);
    }

    mirrorHAct      =    toolBarActions->addAction(QIcon(QPixmap(mirror_h_xpm)),    _("Mirror horizontal"));
    mirrorVAct      =    toolBarActions->addAction(QIcon(QPixmap(mirror_v_xpm)),    _("Mirror vertical"));
    QAction *alignAct =  toolBarActions->addAction(QIcon(QPixmap(align_xpm)),       _("Align elements"));
    { // Create menu for align button
        QMenu *alignMenu = new QMenu();
        alignTopAct    = alignMenu->addAction(QIcon(QPixmap(align_top_xpm)),        _("Align top"));
        alignBottomAct = alignMenu->addAction(QIcon(QPixmap(align_bottom_xpm)),     _("Align bottom"));
        alignLeftAct   = alignMenu->addAction(QIcon(QPixmap(align_left_xpm)),       _("Align left"));
        alignRightAct  = alignMenu->addAction(QIcon(QPixmap(align_right_xpm)),      _("Align right"));
        alignHAct      = alignMenu->addAction(QIcon(QPixmap(align_hcenter_xpm)),    _("Align horizontal-center"));
        alignVAct      = alignMenu->addAction(QIcon(QPixmap(align_vcenter_xpm)),    _("Align vertical-center"));
        alignAct->setMenu(alignMenu);
        qobject_cast<QToolButton*>(toolBarActions->widgetForAction(alignAct))->setPopupMode(QToolButton::InstantPopup);
    }

    snapGridAct    =    toolBarActions->addAction(QIcon(QPixmap(to_grid_xpm)),      _("Snap to grid"));
    toolBarActions->addSeparator();
    removeConAct   =    toolBarActions->addAction(QIcon(QPixmap(remove_con_xpm)),   _("Remove connections (rubberbands)"));
    toolBarActions->addSeparator();
    groupAct       =    toolBarActions->addAction(QIcon(QPixmap(group_on_xpm)),     _("Build group"));
    ungroupAct     =    toolBarActions->addAction(QIcon(QPixmap(group_off_xpm)),    _("Split group"));

    toolBarZoom = addToolBar(_("Zoom functions"));
    QAction *zoomAct =  toolBarZoom->addAction(QIcon(QPixmap(zoom_any_xpm)),        _("Zoom functions"));
    { // Create menu for zoom button
        QMenu *zoomMenu = new QMenu();
        zoomPreviousAct = zoomMenu->addAction(QIcon(QPixmap(zoom_prev_xpm)),        _("Zoom previous"));
        zoomMenu->addSeparator();
        zoomBoardAct    = zoomMenu->addAction(QIcon(QPixmap(zoom_board_xpm)),       _("Zoom board"));
        zoomObjectsAct  = zoomMenu->addAction(QIcon(QPixmap(zoom_objects_xpm)),     _("Zoom objects"));
        zoomSelectionAct= zoomMenu->addAction(QIcon(QPixmap(zoom_selection_xpm)),   _("Zoom selection"));
        zoomAct->setMenu(zoomMenu);
        qobject_cast<QToolButton*>(toolBarZoom->widgetForAction(zoomAct))->setPopupMode(QToolButton::InstantPopup);
    }

    toolBarPCB = addToolBar(_("PCB actions"));
    transparentAct =        toolBarPCB->addAction(QIcon(QPixmap(transparent_xpm)),  _("Toggle transparent mode"));
    transparentAct->setCheckable(true);
    projectInfoAct =        toolBarPCB->addAction(QIcon(QPixmap(info_xpm)),         _("Show project-info"));
    bitmapAct      =        toolBarPCB->addAction(QIcon(QPixmap(bitmap_xpm)),       _("Scanned copy..."));

    toolBarPanels = addToolBar(_("Panel visibility"));
    {
        QWidget* panelsSpacer = new QWidget();
        panelsSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        toolBarPanels->addWidget(panelsSpacer);
    }
    selectorAct    =     toolBarPanels->addAction(QIcon(QPixmap(selector_xpm)),     _("Show/hide Selector-Panel"));
    componentAct   =     toolBarPanels->addAction(QIcon(QPixmap(components_xpm)),   _("Show/hide Component-Panel"));
    propertiesAct  =     toolBarPanels->addAction(QIcon(QPixmap(properties_xpm)),   _("Show/hide Properties-Panel"));
    DRCAct         =     toolBarPanels->addAction(QIcon(QPixmap(drc_xpm)),          _("Show/hide DRC-Panel"));
    macroAct       =     toolBarPanels->addAction(QIcon(QPixmap(macro_xpm)),        _("Show/hide Macro-Library"));

    selectorAct->setCheckable(true);
    componentAct->setCheckable(true);
    propertiesAct->setCheckable(true);
    DRCAct->setCheckable(true);
    macroAct->setCheckable(true);


    SettingsDialog dialog(settings, this);
    dialog.exec();
}

void MainWindow::CreateLeftPanel() {
    toolPanel = new ToolPanel(this);
    addToolBar(Qt::LeftToolBarArea, toolPanel);

    gridPanel = new GridPanel(this);
    addToolBar(Qt::LeftToolBarArea, gridPanel);
}

MainWindow::~MainWindow() {
}
