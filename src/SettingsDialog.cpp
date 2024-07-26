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

SettingsDialog::SettingsDialog(const Settings &oldSettings, QWidget *parent) : QDialog(parent) {
    setWindowTitle(_("General settings"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    VerticalTabWidget *tabs = new VerticalTabWidget(this);
    mainLayout->addWidget(tabs);

    {  // General settings
        QWidget *tab = new QWidget(tabs);
        QVBoxLayout *tabLayout = new QVBoxLayout(tab);

        {
            QHBoxLayout *hlayout = new QHBoxLayout();
            tabLayout->addLayout(hlayout);

            QLabel *unitsLabel = new QLabel(_("Base unit:"), tab);
            unitsLabel->setAlignment(Qt::AlignCenter);
            hlayout->addWidget(unitsLabel);

            QComboBox *units = new QComboBox(tab);
            units->addItem(_("mm"));
            units->addItem(_("mil (1/1000 of inch)"));
            hlayout->addWidget(units);

            QLabel *drillingsLabel = new QLabel(_("Drillings:"), tab);
            drillingsLabel->setAlignment(Qt::AlignCenter);
            hlayout->addWidget(drillingsLabel);

            QComboBox *drillings = new QComboBox(tab);
            drillings->addItem(_("Background color"));
            drillings->addItem(_("White"));
            drillings->addItem(_("Black"));
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

        boardZoom->setChecked(oldSettings.boardZoom);
        darkGround->setChecked(oldSettings.darkGround);
        allGround->setChecked(oldSettings.allGround);
        testConnections->setChecked(oldSettings.testConnections);
        testBlinking->setChecked(oldSettings.testBlinking);
        ctrlCaptureSize->setChecked(oldSettings.ctrlCaptureSize);
        limitTextHeight->setChecked(oldSettings.limitTextHeight);
        alwaysReadable->setChecked(oldSettings.alwaysReadable);
        optimize->setChecked(oldSettings.optimize);
        originLeftTop->setChecked(oldSettings.originLeftTop);
        originExport->setChecked(oldSettings.originExport);

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
    }
}
