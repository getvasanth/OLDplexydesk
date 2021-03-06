/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#include "desktopview.h"
#include "navigator.h"
#include <desktopwidget.h>
#include <backdropinterface.h>
#include <pluginloader.h>
#include <baseplugin.h>
#include <backdropplugin.h>
#include <widgetplugin.h>
#include <viewlayer.h>
#include <frameitem.h>

#include <QGLWidget>
#include <QGraphicsGridLayout>

namespace PlexyDesk
{
    class  DesktopView::Private
    {
    public:
        Private() {}
        ~Private() {}
        AbstractPluginInterface * bIface ;
        BackdropPlugin * bgPlugin;
        WidgetPlugin * widgets;
        QGraphicsGridLayout * gridLayout;
        ViewLayer *  layer;
        float row;
        float column;
        float margin;
        Frame * frm;
        bool openglOn;
    };

    bool getLessThanWidget(const QGraphicsItem* it1, const QGraphicsItem* it2)
    {
        return it1->zValue() < it2->zValue();
    }

    DesktopView::DesktopView(QGraphicsScene * scene, QWidget * parent):QGraphicsView(scene,parent),d(new Private)
    {
        /* setup */
        setWindowFlags(Qt::FramelessWindowHint);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setOptimizationFlag(QGraphicsView::DontClipPainter);
        setCacheMode(QGraphicsView::CacheBackground);
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setFrameStyle(QFrame::NoFrame);
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        d->openglOn = false;

        /* init */
        d->bgPlugin  = static_cast<BackdropPlugin*>(PluginLoader::getInstance()->instance("classicbackdrop"));
        d->widgets = 0;
        d->gridLayout = new QGraphicsGridLayout();
        d->row=d->column = 0.0;
        d->margin = 10.0;
        d->layer = new ViewLayer();

        connect(Config::getInstance(), SIGNAL(configChanged()), this, SLOT(backgroundChanged()));
        connect(Config::getInstance(), SIGNAL(widgetAdded()), this, SLOT(onNewWidget()));
    }

    void DesktopView::onNewWidget()
    {
          addExtension(Config::getInstance()->widgetList.last());
    }

    void DesktopView::enableOpenGL(bool state)
    {
        if (state) {
            setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            setViewport(new QGLWidget(new QGLContext(QGL::StencilBuffer | QGL::AlphaChannel)));
            d->openglOn = true;
        } else {
            setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
            setViewport(new QWidget);
            d->openglOn = false;
        }
    }
    DesktopView::~DesktopView()
    {
        delete d;
    }

    void DesktopView::backgroundChanged()
    {
        if (d->bgPlugin) {
            delete d->bgPlugin;
        }
        d->bgPlugin  =
            static_cast<BackdropPlugin*>(PluginLoader::getInstance()->instance("classicbackdrop"));
        if (!d->openglOn) {
            setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        }
        setCacheMode(QGraphicsView::CacheNone);
        invalidateScene();
        scene()->update();
        update();

        if (!d->openglOn) {
            setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        }

        setCacheMode(QGraphicsView::CacheBackground);
    }

    /*
    Adds an Widget Extension to Plexy Desktop, give the widget
    name in string i.e "clock" or "radio", the internals will
    take care of the loading the widget plugin name is correct
    */

    void DesktopView::addExtension(const QString& name)
    {

        d->widgets = static_cast<WidgetPlugin*>(PluginLoader::getInstance()->instance(name));
        if (d->widgets) {
            DesktopWidget * widget = (DesktopWidget*) d->widgets->item();
            if (widget) {
                widget->configState(DesktopWidget::DOCK);
                scene()->addItem(widget);
                widget->setPos(d->row,d->column);
                d->row += widget->boundingRect().width()+d->margin;
                d->layer->addItem("Widgets",widget);
            }
        }

    }

    void DesktopView::addCoreExtension(const QString& name)
    {

        d->widgets = static_cast<WidgetPlugin*>(PluginLoader::getInstance()->instance(name));
        if (d->widgets) {
            QGraphicsRectItem  * widget = (QGraphicsRectItem*) d->widgets->item();
            if (widget) {
                scene()->addItem(widget);
                widget->setPos(d->row,d->column);
                d->row += widget->boundingRect().width();
            }
        }

    }
    /*
    //small speed up , try if the speed is too low
    void DesktopView::paintEvent(QPaintEvent * event)
    {
    QPaintEvent *newEvent=new QPaintEvent(event->region().boundingRect());
    QGraphicsView::paintEvent(newEvent);
    delete newEvent;
    }
    */

    void DesktopView::drawBackground(QPainter * painter, const QRectF & rect)
    {

        painter->setCompositionMode(QPainter::CompositionMode_Source);
        painter->fillRect(rect, Qt::transparent);
        painter->save();
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->setClipRect(rect);
        if (d->bgPlugin) {
            d->bgPlugin->render(painter,QRectF(rect.x(),sceneRect().y(),rect.width(),rect.height()));
        }
        painter->restore();
    }

    void DesktopView::mousePressEvent(QMouseEvent *event)
    {
        setTopMostWidget(event->pos());

        QGraphicsView::mousePressEvent(event);
    }

    void DesktopView::setTopMostWidget(const QPoint &pt)
    {
        int i = 0;
        QGraphicsItem *clickedItem = scene()->itemAt(pt);
        if (clickedItem == 0)
            return;

        QList<QGraphicsItem *> itemsList = scene()->items();
        qStableSort(itemsList.begin(), itemsList.end(), getLessThanWidget);

        clickedItem->setZValue(itemsList.size());

        foreach(QGraphicsItem* item, itemsList) {
            if (item == clickedItem)
                continue;

            item->setZValue(i);
            i++;
        }

        clickedItem->update();
    }
}

