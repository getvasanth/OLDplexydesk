#include "settings.h"
#include "ui_settings.h"


Settings::Settings(QWidget *parent) :  QWidget(parent), ui(new Ui::SettingsClass)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    //setPalette(QPalette(QColor(Qt::black)));
    //ui->groupBox->resize((this->width()), (this->height()/2)-4);
    connect(ui->browse, SIGNAL(clicked()), this, SLOT(set_image_path()));

}

void Settings::set_image_path()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                             "/usr/share/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    get_image_path(dir);
    ui->imagePath->addItem(dir);

}

void Settings::get_image_path(QString path)
{
    image_path = path;
    qDebug () << "settings "<< image_path;
}

Settings::~Settings()
{
    delete ui;
}
