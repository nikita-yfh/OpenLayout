#pragma once
#include <QWidget>
#include <QLineEdit>

class DirChooserButton : public QWidget {
    Q_OBJECT
public:
    DirChooserButton(const QString &label, char *path, size_t maxSize, QWidget *parent);
    ~DirChooserButton() {}

    const char *getPath() const;
    void setPath(const char *text);
private:
    QLineEdit *textField;
    char *path;
    size_t maxSize;
private slots:
    void OnClick();
};
