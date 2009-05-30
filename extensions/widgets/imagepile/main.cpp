#include <QApplication>
#include <QtGui>

#include "wallpaperconfig.h"
#include "browser.h"

  QStringList findFiles(const QString& path = QString())
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

int main(int argc, char **argv)
{
        QApplication app(argc, argv);
        WallpaperConfig config;
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

        QStringList files = (argc > 0) ? findFiles("/usr/share/wallpapers/Blue_Curl/contents/images") : findFiles();

          QImage img;
          for(int i = 0; i < (int)files.count(); i++)
            if(img.load(files[i]))
              w->addSlide(img);

          QHBoxLayout *browser_holder = new QHBoxLayout(&config);

          w->setCenterIndex(w->slideCount()/2);
          w->setBackgroundColor(Qt::black);
          browser_holder->addWidget(w);
          //browser_holder->addWidget(
          config.setLayout(browser_holder);

          app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );

          config.show();
          return app.exec();
}
