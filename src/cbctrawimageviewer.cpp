#include "cbctrawimageviewer.h"

#include <QFile>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDirIterator>
#include <QTimer>
#include <QPixmap>


#define update_image();

CBCTRawImageViewer::CBCTRawImageViewer()
{

    QString folder = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)";
    dirIterator = new QDirIterator(folder, QDirIterator::Subdirectories);
    timer = new QTimer(this);

connect(timer, &QTimer::timeout, this, &CBCTRawImageViewer::showNextImage);
    timer->start(1000);



}

QPixmap CBCTRawImageViewer::PanoImageViewer()
{
    QString panoPath = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)/0001.raw";
    QFile panoFile(panoPath);
    if (!panoFile.open(QFile::ReadOnly))
        return QPixmap();
    int PanoNum = 1249;
    QByteArray ba = panoFile.readAll();
    const uchar* data = (const uchar*) ba.constData();;
    panoFile.close();
    //QImage image(data, 30, 30, QImage::Format_RGB32);
    QImage* panoImage = new QImage(data, 150, 470, QImage::Format_RGB555);
    QImage pano_Image(*panoImage);
    QPixmap panoPix;
    panoPix = QPixmap::fromImage(pano_Image,Qt::AutoColor);
    return panoPix;

    //ui->PanoLabel->setPixmap(panoPix);

    //    ui->PanoGraphicsView->setPixmap(pix);
}

void CBCTRawImageViewer::showNextImage()
{

}
QPixmap CBCTRawImageViewer::CephImageViewer()
{
    QString cephPath = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)/0001.raw";
    QFile cephFile(cephPath);
    if (!cephFile.open(QFile::ReadOnly))
        return QPixmap();
    int CephNum = 1749;
    QByteArray ba = cephFile.readAll();
    const uchar* data = (const uchar*) ba.constData();;
    cephFile.close();
    //QImage image(data, 30, 30, QImage::Format_RGB32);
    QImage* cephImage = new QImage(data, 150, 470, QImage::Format_RGB555);
    QImage ceph_Image(*cephImage);
    QPixmap cephPix;
    cephPix = QPixmap::fromImage(ceph_Image,Qt::AutoColor);
    return cephPix;
    //ui->CephLabel->setPixmap(cephPix);
}

CBCTRawImageViewer::~CBCTRawImageViewer()
{

}
