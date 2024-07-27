#include "SettingsDialog.h"
#include "Locale.h"

#include <QLabel>
#include <QTabWidget>
#include <QStylePainter>
#include <QStyleOptionTab>
#include <QTabBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>

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

        QCheckBox *boardZoom        = new QCheckBox(_("Show Board-Zoom"), tab);
        QCheckBox *darkGround       = new QCheckBox(_("Darken ground-plane"), tab);
        QCheckBox *allGround        = new QCheckBox(_("Show ground-plane of all layers simulteneous"), tab);
        QCheckBox *testConnections  = new QCheckBox(_("Consider connections (rubberbands) in TEST mode"), tab);
        QCheckBox *testBlinking     = new QCheckBox(_("Blinking TEST mode"), tab);
        QCheckBox *ctrlCaptureSize  = new QCheckBox(_("Double-click takes size parameter of elements"), tab);
        QCheckBox *limitTextHeight  = new QCheckBox(_("Limit text height (track width min. 0.15 mm)"), tab);
        QCheckBox *alwaysReadable   = new QCheckBox(_("Component ID and value always readable after rotating"), tab);
        QCheckBox *optimize         = new QCheckBox(_("Optimize nodes of a track automatically"), tab);
        QCheckBox *originLeftTop    = new QCheckBox(_("Orogin top/left (instead of bottom/left)"), tab);
        QCheckBox *originExport     = new QCheckBox(_("Use origin in CAM-export (Gerber/Excellon/HPGL)"), tab);

        boardZoom->setChecked(settings.boardZoom);
        darkGround->setChecked(settings.darkGround);
        allGround->setChecked(settings.allGround);
        testConnections->setChecked(settings.testConnections);
        testBlinking->setChecked(settings.testBlinking);
        ctrlCaptureSize->setChecked(settings.ctrlCaptureSize);
        limitTextHeight->setChecked(settings.limitTextHeight);
        alwaysReadable->setChecked(settings.alwaysReadable);
        optimize->setChecked(settings.optimize);
        originLeftTop->setChecked(settings.originLeftTop);
        originExport->setChecked(settings.originExport);

        tabLayout->addWidget(boardZoom);
        tabLayout->addWidget(darkGround);
        tabLayout->addWidget(allGround);
        tabLayout->addWidget(testConnections);
        tabLayout->addWidget(testBlinking);
        tabLayout->addWidget(ctrlCaptureSize);
        tabLayout->addWidget(limitTextHeight);
        tabLayout->addWidget(alwaysReadable);
        tabLayout->addWidget(optimize);
        tabLayout->addWidget(originLeftTop);
        tabLayout->addWidget(originExport);

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

        resetColorsButton = new QPushButton(_("Reset scheme to default"), this);
        connect(resetColorsButton, SIGNAL(clicked()), this, SLOT(OnResetColorScheme()));
        tabLayout->addWidget(resetColorsButton, 0, Qt::AlignRight);

        tabs->addTab(tab, _("Colors"));
    }
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

void SettingsDialog::OnUnitsChanged(int index) {
    settings.units = index;
}

void SettingsDialog::OnDrillChanged(int index) {
    settings.drill = index;
}

void SettingsDialog::OnResetColorScheme() {
    settings.GetColorScheme().SetDefault();
    for(int i = 0; i < COLOR_COUNT; i++)
        colorPickerButtons[i]->SetColor(settings.GetColorScheme()[i]);
}
