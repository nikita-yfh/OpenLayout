#include "SettingsDialog.h"
#include "Locale.h"
#include "ToolPanel.h"

#include <QLabel>
#include <QStringList>
#include <QProcess>
#include <QTabWidget>
#include <QStylePainter>
#include <QStyleOptionTab>
#include <QTabBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QSignalMapper>
#include <QFileDialog>
#include <QSpinBox>
#include <QTreeWidget>
#include <QStringList>
#include <QMessageBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDialogButtonBox>

class VerticalTabBar : public QTabBar {
public:
    QSize tabSizeHint(int index) const {
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return s;
    }
protected:
    void paintEvent(QPaintEvent * /*event*/) {
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for(int i = 0; i < count(); i++) {
            initStyleOption(&opt,i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
            painter.restore();
        }
    }
};

class VerticalTabWidget : public QTabWidget {
public:
    VerticalTabWidget(QWidget *parent=0) : QTabWidget(parent) {
        setTabBar(new VerticalTabBar);
        setTabPosition(QTabWidget::West);
    }
};

SettingsDialog::SettingsDialog(const Settings &oldSettings, QWidget *parent)
                                    : QDialog(parent), settings(oldSettings) {
    setWindowTitle(_("General settings"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    VerticalTabWidget *tabs = new VerticalTabWidget(this);
    mainLayout->addWidget(tabs);

    {  // General settings
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        tabLayout->setSpacing(2);

        {
            QHBoxLayout *hlayout = new QHBoxLayout();
            tabLayout->addLayout(hlayout);

            hlayout->addWidget(new QLabel(_("Base unit:"), tab));

            QComboBox *units = new QComboBox(tab);
            units->addItem(_("mm"));
            units->addItem(_("mil (1/1000 of inch)"));
            units->setCurrentIndex(settings.units);
            hlayout->addWidget(units);

            hlayout->addWidget(new QLabel(_("Drillings:"), tab));

            QComboBox *drillings = new QComboBox(tab);
            drillings->addItem(_("Background color"));
            drillings->addItem(_("White"));
            drillings->addItem(_("Black"));
            units->setCurrentIndex(settings.drill);
            hlayout->addWidget(drillings);
        }

        const int CHECKBOXES_COUNT = 11;
		const char *checkboxText[]= {
			_("Show Board-Zoom"),
			_("Darken ground-plane"),
			_("Show ground-plane of all layers simultaneos"),
			_("Consider connections (rubberbands) in TEST mode"),
			_("Blinking TEST mode"),
			_("Double-click takes size parameters of elements"),
			_("Limit text height (track width min. 0.15 mm)"),
			_("Components ID and value always readable after rotating"),
			_("Optimize nodes of a track automatically"),
			_("Origin top/left (instead of bottom/left)"),
			_("Use origin in CAM-export (Gerber/Excellon/HPGL)")
		};

        QSignalMapper *mapper = new QSignalMapper(this);

        for(int i = 0; i < CHECKBOXES_COUNT; i++) {
            QCheckBox *checkbox = new QCheckBox(checkboxText[i], tab);
            checkbox->setChecked(settings[i]);
            connect(checkbox, SIGNAL(toggled(bool)), mapper, SLOT(map()));
            mapper->setMapping(checkbox, i);
            tabLayout->addWidget(checkbox);
        }

        connect(mapper, SIGNAL(mappedInt(int)), this, SLOT(OnCheckboxToggled(int)));

        tabs->addTab(tab, _("General settings"));
    }
    {  // Colors
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        {
            QHBoxLayout *hlayout = new QHBoxLayout();
            tabLayout->addLayout(hlayout);

            hlayout->addWidget(new QLabel(_("Color scheme:"), tab));

            QComboBox *scheme = new QComboBox(tab);
            scheme->addItem(_("Standart"));
            for(int i = 1; i < COLOR_SCHEME_COUNT; i++)
                scheme->addItem(QString::asprintf(_("User %d"), i));
            connect(scheme, SIGNAL(currentIndexChanged(int)), this, SLOT(OnColorSchemeChanged(int)));
            hlayout->addWidget(scheme);
            hlayout->addStretch(1);
        }

		const char* colorNames[]= {
			_("C1 (Copper-Top)"),
			_("S1 (Silkscreen-Top)"),
			_("C2 (Copper-Bottom)"),
			_("S2 (Silkscreen-Bottom)"),
			_("I1 (Copper-Inner 1)"),
			_("I2 (Copper-Inner 2)"),
			_("O (Outline)"),
			_("Background"),
			_("Grid-lines"),
			_("Grid-dots"),
			_("Connections"),
			_("Via"),
			_("Selected object"),
			_("Selection zone")
		};

        QGridLayout *grid = new QGridLayout();
        tabLayout->addLayout(grid);

        const ColorScheme &colorScheme = settings.GetColorScheme();

        for(int i = 0; i < COLOR_COUNT; i++) {
            const int DIV = ((COLOR_COUNT + 1) / 2);
            colorPickerButtons[i] = new ColorPickerButton(colorScheme[i], tab);
            colorPickerButtons[i]->setEnabled(false);
            grid->addWidget(colorPickerButtons[i], i % DIV, i / DIV * 2);
            grid->addWidget(new QLabel(colorNames[i], tab), i % DIV, i / DIV * 2 + 1);
        }
        grid->setColumnStretch(1, 1);
        grid->setColumnStretch(3, 1);

        resetColorsButton = new QPushButton(_("Reset scheme to default"), tab);
        connect(resetColorsButton, SIGNAL(clicked()), this, SLOT(OnResetColorScheme()));
        tabLayout->addWidget(resetColorsButton, 0, Qt::AlignRight);

        tabs->addTab(tab, _("Colors"));
    }
    { // Working directories
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        DirChooserButton *layExport     = new DirChooserButton("Layout files",   settings.layExport,     sizeof(settings.layExport), tab);
        DirChooserButton *gbrExport     = new DirChooserButton("Gerber export",  settings.gbrExport,     sizeof(settings.gbrExport), tab);
        DirChooserButton *bmpExport     = new DirChooserButton("Bitmap export",  settings.bmpExport,     sizeof(settings.bmpExport), tab);
        DirChooserButton *hpglExport    = new DirChooserButton("HPGL export",    settings.hpglExport,    sizeof(settings.hpglExport), tab);
        DirChooserButton *scannedCopies = new DirChooserButton("Scanned copies", settings.scannedCopies, sizeof(settings.scannedCopies), tab);

        tabLayout->addWidget(layExport);
        tabLayout->addWidget(gbrExport);
        tabLayout->addWidget(bmpExport);
        tabLayout->addWidget(hpglExport);
        tabLayout->addWidget(scannedCopies);

        tabLayout->addWidget(new QLabel("Leave this fields empty, if you want OpenLayout to remember\n"
										"the last used directories.", tab));

        QCheckBox *sameDir = new QCheckBox(_("Use the same folder for all file types"), tab);
        sameDir->setChecked(settings.sameDir);
        tabLayout->addWidget(sameDir);

        tabLayout->addStretch(1);

        connect(sameDir, SIGNAL(toggled(bool)), gbrExport,     SLOT(setDisabled(bool)));
        connect(sameDir, SIGNAL(toggled(bool)), bmpExport,     SLOT(setDisabled(bool)));
        connect(sameDir, SIGNAL(toggled(bool)), hpglExport,    SLOT(setDisabled(bool)));
        connect(sameDir, SIGNAL(toggled(bool)), scannedCopies, SLOT(setDisabled(bool)));
        connect(sameDir, SIGNAL(toggled(bool)), this,          SLOT(OnSameDirToggled(bool)));

        emit sameDir->toggled(settings.sameDir);

        tabs->addTab(tab, _("Working directories"));
    }
    { // Macro-Directory
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        tabLayout->setSpacing(15);

        tabLayout->addWidget(new QLabel(_("Root-Directory for macros:"), tab));

        macroPath = new QLineEdit(settings.macroDir, tab);
        macroPath->setReadOnly(true);
        tabLayout->addWidget(macroPath);

        QGridLayout *grid = new QGridLayout();
        tabLayout->addLayout(grid);

        QPushButton *changeButton = new QPushButton(_("Change..."),    tab);
        QPushButton *resetButton  = new QPushButton(_("Reset"),        tab);
        QPushButton *fmButton     = new QPushButton(_("File manager"), tab);
        connect(changeButton, SIGNAL(clicked()), this, SLOT(OnMacroDirChange()));
        connect(resetButton,  SIGNAL(clicked()), this, SLOT(OnMacroDirReset()));
        connect(fmButton,     SIGNAL(clicked()), this, SLOT(OnMacroDirOpenFM()));

        grid->addWidget(changeButton, 0, 0);
        grid->addWidget(resetButton,  1, 0);
        grid->addWidget(fmButton,     2, 0);
        grid->addWidget(new QLabel(_("Only change this setting, if you have moved\n"
                                        "your Macro-Directory to another drive or directory"),  tab), 0, 1);
        grid->addWidget(new QLabel(_("This will reset the Macro-Directory to default setting"), tab), 1, 1);
        grid->addWidget(new QLabel(_("Opens the Macro-Directory in the file manager"),          tab), 2, 1);
        grid->setColumnStretch(1, 1);
        tabLayout->addStretch(1);

        tabs->addTab(tab, _("Macro-Directory"));
    }
    { // Undo-Depth
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        tabLayout->setSpacing(20);

        QSpinBox *spinBox = new QSpinBox(tab);
        spinBox->setMinimum(1);
        spinBox->setMaximum(MAX_UNDO_DEPTH);
        spinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        spinBox->setValue(settings.undoDepth);
        connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(OnUndoDepthChanged(int)));
        tabLayout->addWidget(spinBox);

        tabLayout->addWidget(new QLabel(
            QString::asprintf(_("Here you can define the maximum number of UNDO operations.\n"
                                "The maximum is %d operations\n"
                                "If your system runs very slowly while working with big layouts, \n"
                                "you can decrease this value down to 1."), MAX_UNDO_DEPTH), tab));
        tabLayout->addStretch(1);

        tabs->addTab(tab, _("Undo-Depth"));
    }
    { // I-max
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        QGridLayout *grid = new QGridLayout();
        tabLayout->addLayout(grid);

        grid->addWidget(new QLabel(_("Copper-Thickness in µm"),     tab), 0, 0);
        grid->addWidget(new QLabel(_("Temperature enchance in °C"), tab), 1, 0);

        QSpinBox *copperThickness = new QSpinBox(tab);
        copperThickness->setMinimum(1);
        copperThickness->setMaximum(299);
        copperThickness->setValue(settings.copperThickness);
        connect(copperThickness, SIGNAL(valueChanged(int)), this, SLOT(OnCopperThicknessChanged(int)));

        QSpinBox *tempEnhance = new QSpinBox(tab);
        tempEnhance->setMinimum(1);
        tempEnhance->setMaximum(299);
        tempEnhance->setValue(settings.tempEnhance);
        connect(tempEnhance, SIGNAL(valueChanged(int)), this, SLOT(OnTempEnhanceChanged(int)));

        grid->addWidget(copperThickness, 0, 1);
        grid->addWidget(tempEnhance,     1, 1);

        tabLayout->addWidget(new QLabel(
                _("The calculation of the max. current-carrying capacity for a track, \n"
                  "is only a coarse approximate value.\n"
                  "The real value is depending on many other factors and the environment.\n"), tab), 1);
        tabs->addTab(tab, _("Imax"));
    }
    { // Hotkeys
        QWidget *tab = new QWidget(tabs);
        QHBoxLayout *tabLayout = new QHBoxLayout(tab);

        keyList = new QTreeWidget(tab);
        QStringList header;
        header << _("Mode") << _("Key");
        keyList->setHeaderLabels(header);

        connect(keyList, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(OnToolSelected(QTreeWidgetItem*, int)));

        for(int i = 0; i < TOOL_COUNT; i++) {
            QTreeWidgetItem *item = new QTreeWidgetItem(keyList);
            item->setText(0, ToolPanel::toolNames[i]);
            if(settings.toolKeys[i] == Qt::Key_Escape)
                item->setText(1, "ESC");
            else if(settings.toolKeys[i] >= Qt::Key_A && settings.toolKeys[i] <= Qt::Key_Z)
                item->setText(1, QString(settings.toolKeys[i]));
            keyList->addTopLevelItem(item);
        }
        tabLayout->addWidget(keyList, 2);

        QVBoxLayout *vlayout = new QVBoxLayout();
        tabLayout->addLayout(vlayout, 1);
        vlayout->addStretch(1);
        vlayout->addWidget(new QLabel(_("Change hotkey"), tab));

        keyChoose = new QComboBox(tab);
        keyChoose->setEnabled(false);
        keyChoose->addItem("ESC");
        for(char i = 'A'; i <= 'Z'; i++)
            keyChoose->addItem(QString(i));
        connect(keyChoose, SIGNAL(activated(int)), this, SLOT(OnKeyChanged(int)));
        vlayout->addWidget(keyChoose);
        vlayout->addStretch(1);

        tabs->addTab(tab, _("Hotkeys"));
    }
    { // Crosshair
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        QCheckBox *measure45Lines = new QCheckBox(_("Show 45° lines"), tab);
        measure45Lines->setChecked(settings.measure45Lines);
        tabLayout->addWidget(measure45Lines);

        QCheckBox *measureShow = new QCheckBox(_("Show coordinates on crosshair"), tab);
        measureShow->setChecked(settings.measureShow);
        tabLayout->addWidget(measureShow);

        {   
            QWidget *coordWidget = new QWidget(tab);
            coordWidget->setEnabled(settings.measureShow);
            tabLayout->addWidget(coordWidget);

            QVBoxLayout *coordLayout = new QVBoxLayout(coordWidget);
            connect(measureShow, SIGNAL(clicked(bool)), coordWidget, SLOT(setEnabled(bool)));
            coordLayout->setContentsMargins(25, 0, 0, 0);

            QCheckBox *measureTp = new QCheckBox(_("Transparent"), coordWidget);
            measureTp->setChecked(settings.measureTp);
            coordLayout->addWidget(measureTp);

            QCheckBox *measureBig = new QCheckBox(_("Big Text"), coordWidget);
            measureBig->setChecked(settings.measureBig);
            coordLayout->addWidget(measureBig);

            {
                QHBoxLayout *textLayout = new QHBoxLayout();
                coordLayout->addLayout(textLayout);

                QVBoxLayout *radioLayout = new QVBoxLayout();
                textLayout->addLayout(radioLayout);
                
                {
                    QRadioButton *blackButton = new QRadioButton(_("Black background"), coordWidget);
                    radioLayout->addWidget(blackButton);
                    blackButton->setChecked(!settings.measureLight);

                    QRadioButton *whiteButton = new QRadioButton(_("White background"), coordWidget);
                    radioLayout->addWidget(whiteButton);
                    whiteButton->setChecked(settings.measureLight);

                    QButtonGroup *buttonGroup = new QButtonGroup(coordWidget);
                    buttonGroup->setExclusive(true);
                    buttonGroup->addButton(blackButton, 0);
                    buttonGroup->addButton(whiteButton, 1);

                    connect(buttonGroup, SIGNAL(idClicked(int)), this, SLOT(OnExampleColorChanged(int)));
                }

                exampleText = new QLabel(_("X:  12.280 mm\nY:  14.640 mm"), coordWidget);
                exampleText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
                textLayout->addWidget(exampleText);
                OnExampleColorChanged(settings.measureLight);
            }
        }

        tabLayout->addStretch(1);

        tabs->addTab(tab, _("Crosshair"));
    }
    { // AutoSave
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);
        tabLayout->setSpacing(15);

        tabLayout->addWidget(new QLabel(_("Save layout periodical\n"
                                    "to a separate backup file (*.bak)"), tab));

        QCheckBox *autosave = new QCheckBox(_("Activate AutoSave"), tab);
        tabLayout->addWidget(autosave);

        {
            QWidget *intervalWidget = new QWidget(tab);
            intervalWidget->setEnabled(settings.autosave);
            connect(autosave, SIGNAL(toggled(bool)), intervalWidget, SLOT(setEnabled(bool)));
            connect(autosave, SIGNAL(toggled(bool)), this, SLOT(OnAutosaveToggled(bool)));
            tabLayout->addWidget(intervalWidget);

            QHBoxLayout *intervalLayout = new QHBoxLayout(intervalWidget);
            intervalLayout->setContentsMargins(25, 0, 0, 0);

            intervalLayout->addWidget(new QLabel(_("Interval:"), intervalWidget));

            QSpinBox *autosaveTimer = new QSpinBox(intervalWidget);
            autosaveTimer->setMinimum(1);
            autosaveTimer->setMaximum(60);
            autosaveTimer->setValue(settings.autosaveTimer);
            connect(autosaveTimer, SIGNAL(valueChanged(int)), this, SLOT(OnAutosaveTimerChanged(int)));
            intervalLayout->addWidget(autosaveTimer);

            intervalLayout->addWidget(new QLabel(_("min"), intervalWidget));
            intervalLayout->addStretch(1);
        }

        autosave->setChecked(settings.autosave);
        tabLayout->addStretch(1);

        tabs->addTab(tab, _("AutoSave"));
    }
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addWidget(buttons);
}

void SettingsDialog::OnUnitsChanged(int index) {
    settings.units = index;
}

void SettingsDialog::OnDrillChanged(int index) {
    settings.drill = index;
}

void SettingsDialog::OnCheckboxToggled(int index) {
    settings[index] = !settings[index];
}

void SettingsDialog::OnColorSchemeChanged(int index) {
    for(int i = 0; i < COLOR_COUNT; i++)
        settings.GetColorScheme()[i] = colorPickerButtons[i]->GetColor();

    settings.selectedColorScheme = index;
    for(int i = 0; i < COLOR_COUNT; i++) {
        colorPickerButtons[i]->setEnabled(index != COLOR_SCHEME_DEFAULT);
        colorPickerButtons[i]->SetColor(settings.GetColorScheme()[i]);
    }
    resetColorsButton->setEnabled(index != COLOR_SCHEME_DEFAULT);
}

void SettingsDialog::OnResetColorScheme() {
    settings.GetColorScheme().SetDefault();
    for(int i = 0; i < COLOR_COUNT; i++)
        colorPickerButtons[i]->SetColor(settings.GetColorScheme()[i]);
}

void SettingsDialog::OnSameDirToggled(bool state) {
    settings.sameDir = state;
}

void SettingsDialog::OnMacroDirChange() {
    QFileDialog dialog(this, _("Select a new folder"), settings.macroDir);
    dialog.setFileMode(QFileDialog::Directory);

    if (dialog.exec() == QDialog::Accepted) {
        strncpy(settings.macroDir, dialog.selectedFiles().at(0).toUtf8(), sizeof(settings.macroDir));
        macroPath->setText(settings.macroDir);
    }
}

void SettingsDialog::OnMacroDirReset() {
    settings.SetDefaultMacroPath();
    macroPath->setText(settings.macroDir);
}

void SettingsDialog::OnMacroDirOpenFM() {
#ifdef _WIN32
    const char *fm = "explorer";
#else
    const char *fm = "xdg-open";
#endif
    QProcess::startDetached(fm, QStringList(settings.macroDir));
}

void SettingsDialog::OnUndoDepthChanged(int depth) {
    settings.undoDepth = depth;
}

void SettingsDialog::OnCopperThicknessChanged(int value) {
    settings.copperThickness = value;
}

void SettingsDialog::OnTempEnhanceChanged(int value) {
    settings.tempEnhance = value;
}

void SettingsDialog::OnToolSelected(QTreeWidgetItem *item, int column) {
    int tool = keyList->indexOfTopLevelItem(item);
    Qt::Key key = settings.toolKeys[tool];
    if(key == Qt::Key_Escape)
        keyChoose->setCurrentIndex(0);
    if(key >= Qt::Key_A && key <= Qt::Key_Z)
        keyChoose->setCurrentIndex(key - Qt::Key_A + 1);
    keyChoose->setEnabled(true);
}

void SettingsDialog::OnKeyChanged(int index) {
    Qt::Key key = Qt::Key_Escape;
    QString keyStr = "ESC";

    if(index > 0) {
        key = Qt::Key(Qt::Key_A + index - 1);
        keyStr = QString(key);
    }

    for(int i = 0; i < TOOL_COUNT; i++) {
        if(settings.toolKeys[i] == key) {
            QMessageBox(QMessageBox::Warning, QString(), QString(_("The hotkey \"%1\" is already in use")).arg(keyStr),
                    QMessageBox::Ok, this).exec();
            return;
        }
    }

    keyList->currentItem()->setText(1, keyStr);
    settings.toolKeys[keyList->indexOfTopLevelItem(keyList->currentItem())] = key;
}

void SettingsDialog::OnMeasure45Toggled  (bool value) { settings.measure45Lines = value; }
void SettingsDialog::OnMeasureShowToggled(bool value) { settings.measureShow    = value; }
void SettingsDialog::OnMeasureTpToggled  (bool value) { settings.measureTp      = value; }
void SettingsDialog::OnMeasureBigToggled (bool value) { settings.measureBig     = value; }

void SettingsDialog::OnExampleColorChanged(int light) {
    if(light)
        exampleText->setStyleSheet("QLabel { background-color: white; color: black;"
                                   "border-style: solid; border-width: 1px; border-color: black }");
    else
        exampleText->setStyleSheet("QLabel { background-color: black; color: white;"
                                   "border-style: solid; border-width: 1px; border-color: black }");

    settings.measureLight = light;
}

void SettingsDialog::OnAutosaveToggled     (bool value) { settings.autosave      = value; }
void SettingsDialog::OnAutosaveTimerChanged(int  value) { settings.autosaveTimer = value; }
