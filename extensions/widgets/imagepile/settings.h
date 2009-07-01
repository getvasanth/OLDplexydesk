#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui/QWidget>
#include <QtGui/QFileDialog>
#include <QtDebug>

namespace Ui
{
    class SettingsClass;
}
class Settings : public QWidget {
    Q_OBJECT

public:
    Settings(QWidget *parent = 0);
    ~Settings();
    void get_image_path(QString);
    QString image_path;

private slots:
    void set_image_path();

protected:
    void resizeEvent(QResizeEvent*);

private:
    Ui::SettingsClass  *ui;
    QString dir;
};

#endif // SETTINGS_H
