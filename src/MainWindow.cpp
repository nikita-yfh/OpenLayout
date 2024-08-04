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

    QAction *ID_NEW_ACT = new QAction(_("&New"), this);
    QAction *ID_OPEN_ACT = new QAction(_("&Open"), this);
    QAction *ID_SAVE_ACT = new QAction(_("&Save"), this);
    QAction *ID_SAVEAS_ACT = new QAction(_("&Save as"), this);
    QAction *ID_SAVE_MACRO_ACT = new QAction(_("Save as &macro"), this);
    QAction *ID_AUTOSAVE_ACT = new QAction(_("A&utoSave"), this);
    QAction *ID_GERBER_IMPORT_ACT = new QAction(_("&Gerber-Import"), this);
    QAction *ID_GERBER_EXPORT_ACT = new QAction(_("Gerber Export"), this);
    QAction *ID_DRILL_DATA_ACT = new QAction(_("Drill data (Excellon)"), this);
    QAction *ID_ISOLATION_ACT = new QAction(_("Isolation milling (HPGL, *.plt)"), this);
    QAction *ID_SAVE_BMP_ACT = new QAction(_("Bitmap (*.bmp)"), this);
    QAction *ID_SAVE_JPG_ACT = new QAction(_("JPG (*.jpg)"), this);
    QAction *ID_SAVE_GIF_ACT = new QAction(_("GIF (*.gif)"), this);
    QAction *ID_SAVE_EMP_ACT = new QAction(_("EMP (*.emp)"), this);
    QAction *ID_SAVE_PNG_ACT = new QAction(_("PNG (*.png)"), this);
    QAction *ID_DIRECTORIES_ACT = new QAction(_("&Directories"), this);
    QAction *ID_PRINT_SETUP_ACT = new QAction(_("P&rinter setup"), this);
    QAction *ID_PRINT_ACT = new QAction(_("&Print"), this);
    QAction *ID_EXIT_ACT = new QAction(_("&Exit"), this);

    QAction *ID_UNDO_ACT = new QAction(_("&Undo"), this);
    QAction *ID_REDO_ACT = new QAction(_("&Redo"), this);
    QAction *ID_COPY_ACT = new QAction(_("C&opy"), this);
    QAction *ID_CUT_ACT = new QAction(_("&Cut"), this);
    QAction *ID_PASTE_ACT = new QAction(_("&Paste"), this);
    QAction *ID_DUPLICATE_ACT = new QAction(_("Dup&licate"), this);
    QAction *ID_DELETE_ACT = new QAction(_("&Delete"), this);
    QAction *ID_SELECTALL_ACT = new QAction(_("Select &all"), this);

    QAction *ID_NEW_ACT = new QAction(_("&Add new board"), this);
    QAction *ID_BOARD_PROPERTIES_ACT = new QAction(_("&Properties"), this);
    QAction *ID_BOARD_COPY_ACT = new QAction(_("&Copy board"), this);
    QAction *ID_BOARD_DELETE_ACT = new QAction(_("&Delete board"), this);
    QAction *ID_BOARD_SET_RIGHT_ACT = new QAction(_("Set board to &right"), this);
    QAction *ID_BOARD_SET_LEFT_ACT = new QAction(_("Set board to &left"), this);
    QAction *ID_BOARD_MOVE_RIGHT_ACT = new QAction(_("&Move board to right"), this);
    QAction *ID_BOARD_MOVE_LEFT_ACT = new QAction(_("M&ove board to left"), this);
    QAction *ID_BOARD_IMPORT_ACT = new QAction(_("&Import pages from file"), this);
    QAction *ID_BOARD_SAVE_ACT = new QAction(_("&Save pages to file"), this);

    QAction *ID_ROTATE_ACT = new QAction(_("&Rotate"), this);
    QAction *ID_HMIRROR_ACT = new QAction(_("Mirror &horisontal"), this);
    QAction *ID_VMIRROR_ACT = new QAction(_("Mirror &vertical"), this);
    QAction *ID_GROUP_ACT = new QAction(_("Build &group"), this);
    QAction *ID_UNGROUP_ACT = new QAction(_("Split gro&up"), this);
    QAction *ID_CHANGE_SIDE_ACT = new QAction(_("&Change board side"), this);
    QAction *ID_LAYER_C1_ACT = new QAction(_("&C1"), this);
    QAction *ID_LAYER_S1_ACT = new QAction(_("S&1"), this);
    QAction *ID_LAYER_C2_ACT = new QAction(_("C&2"), this);
    QAction *ID_LAYER_S2_ACT = new QAction(_("&S2"), this);
    QAction *ID_LAYER_I1_ACT = new QAction(_("I1"), this);
    QAction *ID_LAYER_I2_ACT = new QAction(_("&I2"), this);
    QAction *ID_LAYER_O_ACT = new QAction(_("&O"), this);
    QAction *ID_SNAP_GRID_ACT = new QAction(_("S&nap to grid"), this);
    QAction *ID_MASSIVE_ACT = new QAction(_("&Tile / Arrange circular"), this);

    QAction *ID_INFO_ACT = new QAction(_("&Project info"), this);
    QAction *ID_LIST_DRILLINGS_ACT = new QAction(_("&List drillings"), this);
    QAction *ID_SCANNED_COPY_ACT = new QAction(_("&Scanned copy"), this);
    QAction *ID_FOOTPRINT_ACT = new QAction(_("&Footprint-Wizard"), this);
    QAction *ID_RESET_MASK_ACT = new QAction(_("R&eset solder mask"), this);
    QAction *ID_REMOVE_CONNECTIONS_ACT = new QAction(_("&Remove connections (rubberbands)"), this);
    QAction *ID_DELETE_OUTSIDE_ACT = new QAction(_("&Delete elements outside the board"), this);
    QAction *ID_ELEMENT_EXPORT_ACT = new QAction(_("Te&xt-IO: Export elements"), this);
    QAction *ID_ELEMENT_IMPORT_ACT = new QAction(_("&Text-IO: Import elements"), this);

    QAction *ID_PROPERTIES_ACT = new QAction(_("&General settings"), this);
    QAction *ID_PANEL_MACRO_ACT = new QAction(_("&Macro-Library"), this);
    QAction *ID_PANEL_PROPERTIES_ACT = new QAction(_("&Properties-Panel"), this);
    QAction *ID_PANEL_DRC_ACT = new QAction(_("&DRC-Panel"), this);
    QAction *ID_PANEL_COMPONENTS_ACT = new QAction(_("&Components-Panel"), this);
    QAction *ID_PANEL_SELECTOR_ACT = new QAction(_("S&elector-Panel"), this);
    ID_PANEL_MACRO_ACT->setCheckable(true);
    ID_PANEL_PROPERTIES_ACT->setCheckable(true);
    ID_PANEL_DRC_ACT->setCheckable(true);
    ID_PANEL_COMPONENTS_ACT->setCheckable(true);
    ID_PANEL_SELECTOR_ACT->setCheckable(true);
    QAction *ID_ZOOM_BOARD_ACT = new QAction(_("Zoom &board"), this);
    QAction *ID_ZOOM_OBJECTS_ACT = new QAction(_("Zoom &objects"), this);
    QAction *ID_ZOOM_SELECTION_ACT = new QAction(_("Zoom &selection"), this);

    QAction *ID_ABOUT_ACT = new QAction(_("&About"), this);

    ID_NEW_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    ID_OPEN_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    ID_SAVE_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    ID_SAVEAS_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    ID_PRINT_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    ID_EXIT_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    ID_UNDO_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
    ID_REDO_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
    ID_COPY_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
    ID_CUT_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_X));
    ID_PASTE_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_V));
    ID_DUPLICATE_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    ID_DUPLICATE_ACT->setShortcut(QKeySequence(Qt::Key_Delete));
    ID_SELECTALL_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
    ID_ROTATE_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    ID_HMIRROR_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
    ID_VMIRROR_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    ID_GROUP_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    ID_UNGROUP_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U));
    ID_CHANGE_SIDE_ACT->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));

	{ //File
		QMenu *menu = menuBar->addMenu(_("&File"));
        menu->addAction(ID_NEW_ACT);
        menu->addAction(ID_OPEN_ACT);
        menu->addAction(ID_SAVE_ACT);
        menu->addAction(ID_SAVEAS_ACT);
		menu->addSeparator();
        menu->addAction(ID_SAVE_MACRO_ACT);
		menu->addSeparator();
        menu->addAction(ID_AUTOSAVE_ACT);
		menu->addSeparator();
        menu->addAction(ID_GERBER_IMPORT_ACT);
		menu->addSeparator();
		{ //File->Export
            QMenu *submenu = menu->addMenu(_("&Export..."));

            submenu->addAction(ID_GERBER_EXPORT_ACT);
            submenu->addAction(ID_DRILL_DATA_ACT);
			submenu->addSeparator();
            submenu->addAction(ID_ISOLATION_ACT);
			submenu->addSeparator();
            submenu->addAction(ID_SAVE_BMP_ACT);
            submenu->addAction(ID_SAVE_JPG_ACT);
            submenu->addAction(ID_SAVE_GIF_ACT);
            submenu->addAction(ID_SAVE_EMP_ACT);
            submenu->addAction(ID_SAVE_PNG_ACT);
		}
		menu->addSeparator();
        menu->addAction(ID_DIRECTORIES_ACT);
		menu->addSeparator();
        menu->addAction(ID_PRINT_SETUP_ACT);
        menu->addAction(ID_PRINT_ACT);
		menu->addSeparator();
        menu->addAction(ID_EXIT_ACT);
	}
	{ //Edit
		QMenu *menu = menuBar->addMenu(_("&Edit"));

        menu->addAction(ID_UNDO_ACT);
        menu->addAction(ID_REDO_ACT);
		menu->addSeparator();
        menu->addAction(ID_COPY_ACT);
        menu->addAction(ID_CUT_ACT);
        menu->addAction(ID_PASTE_ACT);
        menu->addAction(ID_DUPLICATE_ACT);
        menu->addAction(ID_DELETE_ACT);
		menu->addSeparator();
        menu->addAction(ID_SELECTALL_ACT);
	}
	{ //Board
		QMenu *menu = menuBar->addMenu(_("&Board"));

        menu->addAction(ID_NEW_ACT);
        menu->addAction(ID_BOARD_PROPERTIES_ACT);
        menu->addAction(ID_BOARD_COPY_ACT);
        menu->addAction(ID_BOARD_DELETE_ACT);
		menu->addSeparator();
        menu->addAction(ID_BOARD_SET_RIGHT_ACT);
        menu->addAction(ID_BOARD_SET_LEFT_ACT);
		menu->addSeparator();
        menu->addAction(ID_BOARD_MOVE_RIGHT_ACT);
        menu->addAction(ID_BOARD_MOVE_LEFT_ACT);
        menu->addAction(ID_BOARD_IMPORT_ACT);
        menu->addAction(ID_BOARD_SAVE_ACT);
	}
	{ //Functions
		QMenu *menu = menuBar->addMenu(_("F&unctions"));

        menu->addAction(ID_ROTATE_ACT);
		menu->addSeparator();
        menu->addAction(ID_HMIRROR_ACT);
        menu->addAction(ID_VMIRROR_ACT);
		menu->addSeparator();
        menu->addAction(ID_GROUP_ACT);
        menu->addAction(ID_UNGROUP_ACT);
		menu->addSeparator();
        menu->addAction(ID_CHANGE_SIDE_ACT);
		{ //Functions->Set to layer
			QMenu *submenu = menu->addMenu(_("&Set to layer..."));

            submenu->addAction(ID_LAYER_C1_ACT);
            submenu->addAction(ID_LAYER_S1_ACT);
            submenu->addAction(ID_LAYER_C2_ACT);
            submenu->addAction(ID_LAYER_S2_ACT);
            submenu->addAction(ID_LAYER_I1_ACT);
            submenu->addAction(ID_LAYER_I2_ACT);
            submenu->addAction(ID_LAYER_O_ACT);
		}
		menu->addSeparator();
        menu->addAction(ID_SNAP_GRID_ACT);
        menu->addAction(ID_MASSIVE_ACT);
	}
	{ //Extras
		QMenu *menu = menuBar->addMenu(_("E&xtras"));

        menu->addAction(ID_INFO_ACT);
        menu->addAction(ID_LIST_DRILLINGS_ACT);
		menu->addSeparator();
        menu->addAction(ID_SCANNED_COPY_ACT);
		menu->addSeparator();
        menu->addAction(ID_FOOTPRINT_ACT);
		menu->addSeparator();
        menu->addAction(ID_RESET_MASK_ACT);
        menu->addAction(ID_REMOVE_CONNECTIONS_ACT);
        menu->addAction(ID_DELETE_OUTSIDE_ACT);
		menu->addSeparator();
        menu->addAction(ID_ELEMENT_IMPORT_ACT);
        menu->addAction(ID_ELEMENT_EXPORT_ACT);
	}
	{ //Options
		QMenu *menu = menuBar->addMenu(_("&Options"));
        menu->addAction(ID_PROPERTIES_ACT);
		menu->addSeparator();
        menu->addAction(ID_PANEL_MACRO_ACT);
        menu->addAction(ID_PANEL_PROPERTIES_ACT);
        menu->addAction(ID_PANEL_DRC_ACT);
        menu->addAction(ID_PANEL_COMPONENTS_ACT);
        menu->addAction(ID_PANEL_SELECTOR_ACT);
		menu->addSeparator();
        menu->addAction(ID_ZOOM_BOARD_ACT);
        menu->addAction(ID_ZOOM_OBJECTS_ACT);
        menu->addAction(ID_ZOOM_SELECTION_ACT);
	}
	{ //Help
		QMenu *menu = menuBar->addMenu(_("&Help"));
        menu->addAction(ID_ABOUT_ACT);
	}
}

MainWindow::~MainWindow() {
}
