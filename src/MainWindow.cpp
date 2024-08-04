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
    CreateMenuBar();
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
}

void MainWindow::CreateLeftPanel() {
    toolPanel = new ToolPanel(this);
    addToolBar(Qt::LeftToolBarArea, toolPanel);

    gridPanel = new GridPanel(this);
    addToolBar(Qt::LeftToolBarArea, gridPanel);
}

void MainWindow::CreateMenuBar() {
    QMenuBar *menuBar = this->menuBar();

    QAction *newAct              = new QAction(_("&New"), this);
    QAction *openAct             = new QAction(_("&Open"), this);
    QAction *saveAct             = new QAction(_("&Save"), this);
    QAction *saveasAct           = new QAction(_("&Save as"), this);
    QAction *saveMacroAct        = new QAction(_("Save as &macro"), this);
    QAction *autosaveAct         = new QAction(_("A&utoSave"), this);
    QAction *gerberImportAct     = new QAction(_("&Gerber-Import"), this);
    QAction *gerberExportAct     = new QAction(_("Gerber Export"), this);
    QAction *drillDataAct        = new QAction(_("Drill data (Excellon)"), this);
    QAction *isolationAct        = new QAction(_("Isolation milling (HPGL, *.plt)"), this);
    QAction *saveBmpAct          = new QAction(_("Bitmap (*.bmp)"), this);
    QAction *saveJpgAct          = new QAction(_("JPG (*.jpg)"), this);
    QAction *saveGifAct          = new QAction(_("GIF (*.gif)"), this);
    QAction *saveEmpAct          = new QAction(_("EMP (*.emp)"), this);
    QAction *savePngAct          = new QAction(_("PNG (*.png)"), this);
    QAction *directoriesAct      = new QAction(_("&Directories"), this);
    QAction *printSetupAct       = new QAction(_("P&rinter setup"), this);
    QAction *printAct            = new QAction(_("&Print"), this);
    QAction *exitAct             = new QAction(_("&Exit"), this);

    QAction *undoAct             = new QAction(_("&Undo"), this);
    QAction *redoAct             = new QAction(_("&Redo"), this);
    QAction *copyAct             = new QAction(_("C&opy"), this);
    QAction *cutAct              = new QAction(_("&Cut"), this);
    QAction *pasteAct            = new QAction(_("&Paste"), this);
    QAction *duplicateAct        = new QAction(_("Dup&licate"), this);
    QAction *deleteAct           = new QAction(_("&Delete"), this);
    QAction *selectallAct        = new QAction(_("Select &all"), this);

    QAction *boardNewAct         = new QAction(_("&Add new board"), this);
    QAction *boardPropertiesAct  = new QAction(_("&Properties"), this);
    QAction *boardCopyAct        = new QAction(_("&Copy board"), this);
    QAction *boardDeleteAct      = new QAction(_("&Delete board"), this);
    QAction *boardSetRightAct    = new QAction(_("Set board to &right"), this);
    QAction *boardSetLeftAct     = new QAction(_("Set board to &left"), this);
    QAction *boardMoveRightAct   = new QAction(_("&Move board to right"), this);
    QAction *boardMoveLeftAct    = new QAction(_("M&ove board to left"), this);
    QAction *boardImportAct      = new QAction(_("&Import pages from file"), this);
    QAction *boardSaveAct        = new QAction(_("&Save pages to file"), this);

    QAction *rotateAct           = new QAction(_("&Rotate"), this);
    QAction *hmirrorAct          = new QAction(_("Mirror &horisontal"), this);
    QAction *vmirrorAct          = new QAction(_("Mirror &vertical"), this);
    QAction *groupAct            = new QAction(_("Build &group"), this);
    QAction *ungroupAct          = new QAction(_("Split gro&up"), this);
    QAction *changeSideAct       = new QAction(_("&Change board side"), this);
    QAction *layerC1Act          = new QAction(_("&C1"), this);
    QAction *layerS1Act          = new QAction(_("S&1"), this);
    QAction *layerC2Act          = new QAction(_("C&2"), this);
    QAction *layerS2Act          = new QAction(_("&S2"), this);
    QAction *layerI1Act          = new QAction(_("I1"), this);
    QAction *layerI2Act          = new QAction(_("&I2"), this);
    QAction *layerOAct           = new QAction(_("&O"), this);
    QAction *snapGrAct           = new QAction(_("S&nap to grid"), this);
    QAction *massiveAct          = new QAction(_("&Tile / Arrange circular"), this);

    QAction *infoAct             = new QAction(_("&Project info"), this);
    QAction *listDrillingsAct    = new QAction(_("&List drillings"), this);
    QAction *scannedCopyAct      = new QAction(_("&Scanned copy"), this);
    QAction *footprintAct        = new QAction(_("&Footprint-Wizard"), this);
    QAction *resetMaskAct        = new QAction(_("R&eset solder mask"), this);
    QAction *removeConnectionsAct= new QAction(_("&Remove connections (rubberbands)"), this);
    QAction *deleteOutsideAct    = new QAction(_("&Delete elements outside the board"), this);
    QAction *elementExportAct    = new QAction(_("Te&xt-IO: Export elements"), this);
    QAction *elementImportAct    = new QAction(_("&Text-IO: Import elements"), this);

    QAction *propertiesAct       = new QAction(_("&General settings"), this);
    QAction *panelMacroAct       = new QAction(_("&Macro-Library"), this);
    QAction *panelPropertiesAct  = new QAction(_("&Properties-Panel"), this);
    QAction *panelDrcAct         = new QAction(_("&DRC-Panel"), this);
    QAction *panelComponentsAct  = new QAction(_("&Components-Panel"), this);
    QAction *panelSelectorAct    = new QAction(_("S&elector-Panel"), this);
    panelMacroAct     ->setCheckable(true);
    panelPropertiesAct->setCheckable(true);
    panelDrcAct       ->setCheckable(true);
    panelComponentsAct->setCheckable(true);
    panelSelectorAct  ->setCheckable(true);
    QAction *zoomBoardAct        = new QAction(_("Zoom &board"), this);
    QAction *zoomObjectsAct      = new QAction(_("Zoom &objects"), this);
    QAction *zoomSelectionAct    = new QAction(_("Zoom &selection"), this);

    QAction *aboutAct = new QAction(_("&About"), this);

    newAct        ->setShortcut(QKeySequence::New);
    openAct       ->setShortcut(QKeySequence::Open);
    saveAct       ->setShortcut(QKeySequence::Save);
    saveasAct     ->setShortcut(QKeySequence::SaveAs);
    printAct      ->setShortcut(QKeySequence::Print);
    exitAct       ->setShortcut(QKeySequence::Quit);
    undoAct       ->setShortcut(QKeySequence::Undo);
    redoAct       ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
    copyAct       ->setShortcut(QKeySequence::Copy);
    cutAct        ->setShortcut(QKeySequence::Cut);
    pasteAct      ->setShortcut(QKeySequence::Paste);
    duplicateAct  ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    deleteAct     ->setShortcut(QKeySequence::Delete);
    selectallAct  ->setShortcut(QKeySequence::SelectAll);
    rotateAct     ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    hmirrorAct    ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    vmirrorAct    ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    groupAct      ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    ungroupAct    ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U));
    changeSideAct ->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));

	{ //File
		QMenu *menu = menuBar->addMenu(_("&File"));
        menu->addAction(newAct);
        menu->addAction(openAct);
        menu->addAction(saveAct);
        menu->addAction(saveasAct);
		menu->addSeparator();
        menu->addAction(saveMacroAct);
		menu->addSeparator();
        menu->addAction(autosaveAct);
		menu->addSeparator();
        menu->addAction(gerberImportAct);
		menu->addSeparator();
		{ //File->Export
            QMenu *submenu = menu->addMenu(_("&Export..."));

            submenu->addAction(gerberExportAct);
            submenu->addAction(drillDataAct);
			submenu->addSeparator();
            submenu->addAction(isolationAct);
			submenu->addSeparator();
            submenu->addAction(saveBmpAct);
            submenu->addAction(saveJpgAct);
            submenu->addAction(saveGifAct);
            submenu->addAction(saveEmpAct);
            submenu->addAction(savePngAct);
		}
		menu->addSeparator();
        menu->addAction(directoriesAct);
		menu->addSeparator();
        menu->addAction(printSetupAct);
        menu->addAction(printAct);
		menu->addSeparator();
        menu->addAction(exitAct);
	}
	{ //Edit
		QMenu *menu = menuBar->addMenu(_("&Edit"));

        menu->addAction(undoAct);
        menu->addAction(redoAct);
		menu->addSeparator();
        menu->addAction(copyAct);
        menu->addAction(cutAct);
        menu->addAction(pasteAct);
        menu->addAction(duplicateAct);
        menu->addAction(deleteAct);
		menu->addSeparator();
        menu->addAction(selectallAct);
	}
	{ //Board
		QMenu *menu = menuBar->addMenu(_("&Board"));

        menu->addAction(boardNewAct);
        menu->addAction(boardPropertiesAct);
        menu->addAction(boardCopyAct);
        menu->addAction(boardDeleteAct);
		menu->addSeparator();
        menu->addAction(boardSetRightAct);
        menu->addAction(boardSetLeftAct);
		menu->addSeparator();
        menu->addAction(boardMoveRightAct);
        menu->addAction(boardMoveLeftAct);
        menu->addAction(boardImportAct);
        menu->addAction(boardSaveAct);
	}
	{ //Functions
		QMenu *menu = menuBar->addMenu(_("F&unctions"));

        menu->addAction(rotateAct);
		menu->addSeparator();
        menu->addAction(hmirrorAct);
        menu->addAction(vmirrorAct);
		menu->addSeparator();
        menu->addAction(groupAct);
        menu->addAction(ungroupAct);
		menu->addSeparator();
        menu->addAction(changeSideAct);
		{ //Functions->Set to layer
			QMenu *submenu = menu->addMenu(_("&Set to layer..."));

            submenu->addAction(layerC1Act);
            submenu->addAction(layerS1Act);
            submenu->addAction(layerC2Act);
            submenu->addAction(layerS2Act);
            submenu->addAction(layerI1Act);
            submenu->addAction(layerI2Act);
            submenu->addAction(layerOAct);
		}
		menu->addSeparator();
        menu->addAction(snapGrAct);
        menu->addAction(massiveAct);
	}
	{ //Extras
		QMenu *menu = menuBar->addMenu(_("E&xtras"));

        menu->addAction(infoAct);
        menu->addAction(listDrillingsAct);
		menu->addSeparator();
        menu->addAction(scannedCopyAct);
		menu->addSeparator();
        menu->addAction(footprintAct);
		menu->addSeparator();
        menu->addAction(resetMaskAct);
        menu->addAction(removeConnectionsAct);
        menu->addAction(deleteOutsideAct);
		menu->addSeparator();
        menu->addAction(elementImportAct);
        menu->addAction(elementExportAct);
	}
	{ //Options
		QMenu *menu = menuBar->addMenu(_("&Options"));
        menu->addAction(propertiesAct);
		menu->addSeparator();
        menu->addAction(panelMacroAct);
        menu->addAction(panelPropertiesAct);
        menu->addAction(panelDrcAct);
        menu->addAction(panelComponentsAct);
        menu->addAction(panelSelectorAct);
		menu->addSeparator();
        menu->addAction(zoomBoardAct);
        menu->addAction(zoomObjectsAct);
        menu->addAction(zoomSelectionAct);
	}
	{ //Help
		QMenu *menu = menuBar->addMenu(_("&Help"));
        menu->addAction(aboutAct);
	}
}

MainWindow::~MainWindow() {
}
