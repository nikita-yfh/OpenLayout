#include "DirChooserButton.h"
#include "Locale.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QSize>

class SquarePushButton : public QPushButton {
public:
    SquarePushButton(QWidget *parent)
        : QPushButton("...", parent) {}
    QSize sizeHint() const override {
        QSize oldSize = QPushButton::sizeHint();
        return QSize(oldSize.height(), oldSize.height());
    }
};

DirChooserButton::DirChooserButton(const QString &label, char *_path, size_t _maxSize, QWidget *parent)
                    : QWidget(parent), path(_path), maxSize(_maxSize) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(new QLabel(label, this), 1, Qt::AlignLeft);

    textField = new QLineEdit(path, this);
    textField->setReadOnly(true);
    layout->addWidget(textField, 2);

    QPushButton *button = new SquarePushButton(this);
    connect(button, SIGNAL(clicked()), this, SLOT(OnClick()));
    layout->addWidget(button, 0);
}

void DirChooserButton::OnClick() {
    QFileDialog dialog(this, _("Select a new folder"), path);
    dialog.setFileMode(QFileDialog::Directory);

    if (dialog.exec() == QDialog::Accepted)
        setPath(dialog.selectedFiles().at(0).toUtf8()); 
}

const char *DirChooserButton::getPath() const {
    return path;
}

void DirChooserButton::setPath(const char *text) {
    strncpy(path, text, maxSize);
    textField->setText(text);
}

