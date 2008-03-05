/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Lahiru Lakmal <llahiru@gmail.com>
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

#include <plexy.h>
#include "youtubewidget.h"
#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <math.h>
#include <QBitmap>

namespace PlexyDesk
{

YouTubeWidget::YouTubeWidget (const QRectF &rect, QWidget *widget):
DesktopWidget(rect,widget)
{
  shade == 0;
  //getBaseData();

  setPath("/usr/share/plexy/skins/default/youtube/");
  setDockImage(QPixmap(prefix + "icon.png"));

  PlexyDesk::DataInterface * utubeEngine = (PlexyDesk::DataInterface*)
          PlexyDesk::PluginLoader::getInstance()->instance("utubeengine");
  
  //utubeEngine= loadData("utubeengine");
  if (utubeEngine) {
      connect(utubeEngine,SIGNAL(data(QVariant&)),this,SLOT(data(QVariant&)));
      //connect(utubeEngine,SIGNAL(success()),this,SLOT(drawItems()));
  }else {
      qDebug("DataSource Was Null");
  }
  
  connect(this,SIGNAL(dataChanged()),this,SLOT(drawItems()));
  drawWidget();
}

YouTubeWidget::~YouTubeWidget ()
{}

void YouTubeWidget::setPath(QString str)
{
  prefix  = str+"/";
}

void YouTubeWidget::data(QVariant& data)
{
    mVariantMap = data.toMap();
    
    VideoEntity videoentity;
    videoentity.title = (mVariantMap["title"]).toString();
    videoentity.link = (mVariantMap["link"]).toString();
    videoentity.desc = (mVariantMap["description"]).toString();
    videoentity.thumb = (mVariantMap["thumb"]).toString();
    
    //qDebug() << videoentity->title;
    
    mVideos.append(videoentity);
    
    emit dataChanged();
}

void 
        YouTubeWidget::drawWidget()
{
   m_bg = QImage (prefix + "bacground.png");
   
   mItem_bg = QImage (prefix + "videotag.png");

   /*gloss stuff*/
   //gloss = QImage (prefix + "boarderLarge.png");;
}

void
        YouTubeWidget::drawItems()
{
    //qDebug() << mVideos.count();
    update ();
    //getNxtData();
}


void
YouTubeWidget::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem
* e , QWidget *)
{

  QRectF r  = e->exposedRect;
    //p->setOpacity(0.8);
  p->setCompositionMode(QPainter::CompositionMode_Source);
  p->fillRect(rect(), Qt::transparent);

  p->drawImage (QRect(0,0,m_bg.width(),m_bg.height()),m_bg);
  p->setCompositionMode(QPainter::CompositionMode_SourceOver);
  p->setBackgroundMode(Qt::TransparentMode);
  p->save ();
  p->setRenderHint (QPainter::SmoothPixmapTransform);
  if (shade == 0)
    {
      p->drawPixmap (m_bg.rect (), QPixmap ().fromImage(m_bg));
      shade = 1;
    }

  p->restore ();

  p->setRenderHints(QPainter::SmoothPixmapTransform |QPainter::Antialiasing |QPainter::HighQualityAntialiasing);

  if(mVideos.size()>0){
    int texty=30; int tagy=10;
    for (int i = 0; i < mVideos.size(); ++i) {
        VideoEntity videoentity;
        videoentity = (VideoEntity) mVideos.at(i);
        
        p->drawImage( QRect(10,tagy,mItem_bg.width(),mItem_bg.height()),mItem_bg
);
        
        p->setPen( QColor(255,255,255) );
        p->setFont( QFont("Bitstream Charter",10,QFont::Bold) );
        p->drawText( 30,texty,300,30,Qt::AlignLeft,QString(videoentity.title) );
        
        p->setPen( QColor(255,255,255) );
        p->setFont( QFont("Bitstream Charter",9) );
        p->drawText(
                   
30,texty+30,250,100,Qt::AlignLeft|Qt::TextWordWrap,QString(videoentity.desc) );
        
        texty += mItem_bg.height(); tagy += mItem_bg.height();
    }
  }

//   p->setPen(QColor(255,255,255));
//   p->setFont(QFont("Bitstream Charter",10,QFont::Bold));
//   p->drawText(QRect(70,130,64,64), Qt::AlignCenter ,"CPU" );
}


void
YouTubeWidget::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e
, QWidget *)
{
    
}

} // namespace PlexyDesk
#include "youtubewidget.moc"