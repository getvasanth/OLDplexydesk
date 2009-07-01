#ifndef WALLPAPERCONFIG_H
#define WALLPAPERCONFIG_H

#include <QtGui>
#include "settings.h"
#include "constants.h"

class CustomTimeLine: public QTimeLine
{
private:
    qreal m_factor;
    qreal m_offset;
    qreal m_range;

public:
    bool useLogisticFunction;

    CustomTimeLine(): QTimeLine(), useLogisticFunction(false) {
        setFactor(4);
    }

    void setFactor(qreal factor) {
        m_factor = factor;
        qreal max = 1 / (1 + exp(-factor));
        m_offset = 1 / (1 + exp(factor));
        m_range = max - m_offset;
    }

    virtual qreal valueForTime(int msec) const {
        if (useLogisticFunction) {
            // this is normalized x
            qreal x = static_cast<qreal>(msec) / duration();

            // make it two-sided, 0 is the center
            qreal xs = x * 2 * m_factor - m_factor;

            // the simple logistic function
            qreal y = 1 / (1 + exp(-xs));

            // normalize to 0..1
            y = (y - m_offset) / m_range;

            return y;
        }
        else
            return QTimeLine::valueForTime(msec);
    }
};


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
     void setSlidePosition(int);

public slots:
     void changeState();

protected:
    void resizeEvent(QResizeEvent*);

private:
    Settings *settingsWindow;
    CustomTimeLine *m_timeLine;
    Ui::WallpaperConfigClass *ui;
};

#endif // WALLPAPERCONFIG_H
