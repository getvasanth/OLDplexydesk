#include <QApplication>
#include <QtGui>

#include "wallpaperconfig.h"




int main(int argc, char **argv)
{
        QApplication app(argc, argv);
        WallpaperConfig config;

       app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()) );
       config.show();

      return app.exec();
}
