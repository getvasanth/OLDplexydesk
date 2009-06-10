#include "wallpaperconfig.h"
#include "ui_wallpaperconfig.h"
#include "browser.h"

WallpaperConfig::WallpaperConfig(QWidget *parent)
    : QDialog(parent), ui(new Ui::WallpaperConfigClass)
{
    ui->setupUi(this);
    Browser *w = new Browser();

    #if defined(_WS_QWS) || defined(Q_WS_QWS)
        w->showFullScreen();
            int ww = w->width();
            int wh = w->height();
            int dim = (ww > wh) ? wh : ww;
            dim = dim * 3 / 4;
            w->setSlideSize(QSize(3*dim/5, dim));
        #else
            w->setSlideSize(QSize(3*40, 5*40));
            w->resize(750, 270);
        #endif

        QStringList files = findFiles("/usr/share/wallpapers/Blue_Curl/contents/images");
          QImage img;
          for(int i = 0; i < (int)files.count(); i++)
            if(img.load(files[i]))
              {
                w->addSlide(img);
                }
          w->setCenterIndex(w->slideCount()/2);
          w->setBackgroundColor(Qt::black);

     QGridLayout *browser_holder = new QGridLayout;

     browser_holder->addWidget(ui->layoutWidget, 0, 0);
     browser_holder->addWidget(w, 1, 0, 1, 2);
     browser_holder->addWidget(ui->layoutWidget1, 2, 0);

     setLayout(browser_holder);
     show();

}

QStringList WallpaperConfig::findFiles(const QString& path)
{
  QStringList files;
  QDir dir = QDir::current();
  if(!path.isEmpty())
    dir = QDir(path);

  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
  QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i)
  {
    QFileInfo fileInfo = list.at(i);
    files.append(dir.absoluteFilePath(fileInfo.fileName()));
  }
  return files;
}
WallpaperConfig::~WallpaperConfig()
{
    delete ui;
}
