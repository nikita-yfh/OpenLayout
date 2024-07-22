#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void CreateToolBars();
    QToolBar *toolBarMain, *toolBarHistory, *toolBarClipboard,
             *toolBarActions, *toolBarZoom, *toolBarPCB, *toolBarPanels;

    QAction *newAct, *openAct, *saveAct, *printAct, *undoAct, *redoAct,
            *cutAct, *copyAct, *pasteAct, *deleteAct, *duplicateAct,
            *rotate90Act, *rotate45Act, *rotate15Act, *rotate5Act, *rotateCustomAct,
            *mirrorHAct, *mirrorVAct,
            *alignTopAct, *alignBottomAct, *alignLeftAct, *alignRightAct, *alignHAct, *alignVAct,
            *snapGridAct, *removeConAct, *groupAct, *ungroupAct,
            *zoomPreviousAct, *zoomBoardAct, *zoomObjectsAct, *zoomSelectionAct,
            *transparentAct, *projectInfoAct, *bitmapAct,
            *selectorAct, *componentAct, *propertiesAct, *DRCAct, *macroAct;
};
