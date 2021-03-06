#include "wallpaperconfig.h"
#include "ui_wallpaperconfig.h"
#include "browser.h"

WallpaperConfig::WallpaperConfig(QWidget *parent)
    : QDialog(parent), ui(new Ui::WallpaperConfigClass)
{
    ui->setupUi(this);
    settingsWindow = new Settings(this);
    Browser *w = new Browser();
    m_timeLine = new CustomTimeLine;
    //setAutoFillBackground(true);
    //setPalette(QPalette(QColor(Qt::black)));

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

   QGridLayout *browser_holder = new QGridLayout;
   browser_holder->addWidget(ui->layoutWidget, 0, 0);
   browser_holder->addWidget(w, 1, 0, 1, 2);
   browser_holder->addWidget(ui->layoutWidget1, 2, 0);

   QStringList files = settingsWindow->findFiles("/usr/share/wallpapers/Blue_Curl/contents/images");
   QImage img;
   for(int i = 0; i < (int)files.count(); i++)
        if(img.load(files[i])) {
           w->addSlide(img);
         }
   w->setCenterIndex(w->slideCount()/2);
   w->setBackgroundColor(Qt::black);

   setLayout(browser_holder);

   m_timeLine->setCurveShape(QTimeLine::LinearCurve);
   m_timeLine->setFrameRange(0, PANEL_WIDTH);
   settingsWindow->setGeometry(this->width()/4, 0, this->width()/2, this->height());
   settingsWindow->hide();

   connect(m_timeLine, SIGNAL(frameChanged(int)), SLOT(setSlidePosition(int)));
   connect(m_timeLine, SIGNAL(stateChanged(QTimeLine::State)), SLOT(changeState()));
   connect(m_timeLine, SIGNAL(finished()), m_timeLine, SLOT(toggleDirection()));
   ui->hideButton->hide();
   ui->hideButton->setCheckable(true);
   ui->showButton->setCheckable(true);
   connect(ui->hideButton, SIGNAL(clicked()), m_timeLine, SLOT(start()));
   connect(ui->showButton, SIGNAL(clicked()), m_timeLine, SLOT(start()));

}

 void WallpaperConfig::changeState()
 {
        int speed = 50;
        m_timeLine->setDuration(1000 - speed*10);
        bool notrunning = m_timeLine->state() == QTimeLine::NotRunning;
        ui->showButton->setEnabled(notrunning);
        ui->hideButton->setEnabled(notrunning);
        ui->showButton->setChecked(!m_timeLine->direction() == QTimeLine::Backward);
        ui->hideButton->setChecked(ui->showButton->isVisible());
        settingsWindow->show();

 }

void WallpaperConfig::resizeEvent(QResizeEvent*) {
        settingsWindow->resize(settingsWindow->width(), height() - 2 * PANEL_MARGIN);
    }

 void WallpaperConfig::setSlidePosition(int pos)
 {
        settingsWindow->resize(settingsWindow->height(), pos );
        settingsWindow->setGeometry(this->width()/4, 0, this->width()/2, pos);
        qreal t = static_cast<qreal>(m_timeLine->currentTime());
        qreal x = t * PANEL_WIDTH / m_timeLine->duration();
 }


WallpaperConfig::~WallpaperConfig()
{
    delete ui;
}
