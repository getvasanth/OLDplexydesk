#include "settings.h"
#include "constants.h"
#include "ui_settings.h"


Settings::Settings(QWidget *parent) :  QWidget(parent), ui(new Ui::SettingsClass)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    //setPalette(QPalette(QColor(Qt::black)));
    //ui->groupBox->resize((this->width()), (this->height()/2)-4);
    connect(ui->browse, SIGNAL(clicked()), this, SLOT(set_image_path()));
}

QStringList Settings::findFiles(const QString& path)
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

void Settings::set_image_path()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                             "/usr/share/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    get_image_path(dir);
    ui->imagePath->addItem(dir);
    ui->imagePath->setCurrentIndex(ui->imagePath->count()-1);
}

void Settings::get_image_path(QString path)
{
    image_path = path;
    qDebug () << "settings "<< image_path;
}

void Settings::resizeEvent(QResizeEvent*)
{
        if (this->height() != height())
            this->resize(PANEL_WIDTH, height());
}

Settings::~Settings()
{
    delete ui;
}
