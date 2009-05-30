#include "wallpaperconfig.h"
#include "ui_wallpaperconfig.h"

WallpaperConfig::WallpaperConfig(QWidget *parent)
    : QDialog(parent), ui(new Ui::WallpaperConfigClass)
{
    ui->setupUi(this);
}

WallpaperConfig::~WallpaperConfig()
{
    delete ui;
}
