#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    QString folder = "C:/Qt_VTK_CT\build/Debug/Pano_Frame(1152x64)";
//    QPixmap folder = "C:/Qt_VTK_CT/Images/Pano Frame(1152x64)/0001.raw";
    imageIterator = new QDirIterator(folder, QDirIterator::Subdirectories);
    imageTimer = new QTimer(this);

    connect(imageTimer, &QTimer::timeout, this, &ImageViewer::showNextImage);
    imageTimer->start(1000);

    showNextImage();
}

void ImageViewer::showNextImage()
{
    if(imageIterator->hasNext()){
        imageIterator->next();
        QPixmap image(imageIterator->filePath());
//        QPixmap image("C:/Qt_VTK_CT/Images/Pano Frame(1152x64)/0001.raw");
        ui->label->setPixmap(image);
        ui->label->adjustSize();
    } else {
        //imageIterator->rewind();
    }
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

