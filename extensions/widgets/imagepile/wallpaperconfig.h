#ifndef WALLPAPERCONFIG_H
#define WALLPAPERCONFIG_H

#include <QtGui/QDialog>

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

private:
    Ui::WallpaperConfigClass *ui;
};

#endif // WALLPAPERCONFIG_H
