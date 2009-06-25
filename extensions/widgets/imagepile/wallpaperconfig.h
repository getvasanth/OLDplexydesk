#ifndef WALLPAPERCONFIG_H
#define WALLPAPERCONFIG_H

#include <QtGui/QDialog>
#include <QtDebug>
#include "settings.h"

namespace Ui
{
    class WallpaperConfigClass;
}

class WallpaperConfig : public QDialog
{
    Q_OBJECT

public:
    WallpaperConfig(QWidget *parent = 0);
    QStringList findFiles(const QString& path);
    ~WallpaperConfig();

private slots:
     void showSettings();

private:
    Settings *settingsWindow;
    Ui::WallpaperConfigClass *ui;
};

#endif // WALLPAPERCONFIG_H
