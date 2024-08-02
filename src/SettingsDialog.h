#pragma once
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTreeWidget>

#include "Settings.h"
#include "ColorPickerButton.h"
#include "DirChooserButton.h"

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(const Settings &oldSettings, QWidget *parent);
    ~SettingsDialog() {}
private:
    ColorPickerButton *colorPickerButtons[COLOR_COUNT];
    QPushButton *resetColorsButton;
    QLineEdit *macroPath;
    QTreeWidget *keyList;
    QComboBox *keyChoose;

    Settings settings;
private slots:
    void OnUnitsChanged(int index);
    void OnDrillChanged(int index);

    void OnCheckboxToggled(int index);

    void OnColorSchemeChanged(int index);
    void OnResetColorScheme();

    void OnSameDirToggled(bool state);

    void OnMacroDirChange();
    void OnMacroDirReset();
    void OnMacroDirOpenFM();

    void OnUndoDepthChanged(int depth);

    void OnCopperThicknessChanged(int);
    void OnTempEnhanceChanged(int);

    void OnToolSelected(QTreeWidgetItem *item, int column);
    void OnKeyChanged(int index);
};
