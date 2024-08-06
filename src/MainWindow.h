#pragma once
#include <QMainWindow>
#include <QtWidgets>

#include "ToolPanel.h"
#include "GridPanel.h"
#include "Settings.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void CreateActions();
    void CreateMenuBar();
    void CreateToolBar();
    void CreateLeftPanel();

    Settings settings;

    QToolBar *toolBarMain, *toolBarHistory, *toolBarClipboard,
             *toolBarActions, *toolBarZoom, *toolBarPCB, *toolBarPanels;
    ToolPanel *toolPanel;
    GridPanel *gridPanel;

    QAction *aboutAct, *alignAct, *alignBottomAct, *alignHAct, *alignLeftAct,
            *alignRightAct, *alignTopAct, *alignVAct, *autosaveAct, *bitmapAct,
            *boardCopyAct, *boardDeleteAct, *boardImportAct, *boardMoveLeftAct,
            *boardMoveRightAct, *boardNewAct, *boardPropAct, *boardSaveAct,
            *boardSetLeftAct, *boardSetRightAct, *changeSideAct, *copyAct,
            *cutAct, *deleteAct, *deleteOutsideAct, *directoriesAct, *drillDataAct,
            *duplicateAct, *elementExportAct, *elementImportAct, *exitAct,
            *footprintAct, *gerberExportAct, *gerberImportAct, *groupAct,
            *hmirrorAct, *isolationAct, *layerC1Act, *layerC2Act, *layerI1Act,
            *layerI2Act, *layerOAct, *layerS1Act, *layerS2Act, *listDrillingsAct,
            *massiveAct, *newAct, *openAct, *panelComponentsAct, *panelDrcAct,
            *panelMacroAct, *panelPropertiesAct, *panelSelectorAct, *pasteAct,
            *printAct, *printSetupAct, *projectInfoAct, *redoAct, *removeConAct,
            *resetMaskAct, *rotate15Act, *rotate45Act, *rotate5Act, *rotate90Act,
            *rotateAct, *rotateCustomAct, *saveAct, *saveBmpAct, *saveEmpAct,
            *saveGifAct, *saveJpgAct, *saveMacroAct, *savePngAct, *saveasAct,
            *selectallAct, *settingsAct, *snapGridAct, *transparentAct, *undoAct,
            *ungroupAct, *vmirrorAct, *zoomAct, *zoomBoardAct, *zoomObjectsAct,
            *zoomPreviousAct, *zoomSelectionAct;
};
