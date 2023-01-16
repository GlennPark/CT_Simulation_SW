#include "cbctrawimageviewer.h"
//#include "qpixmap.h"
#include <QFile>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
CBCTRawImageViewer::CBCTRawImageViewer()
{

}


void CBCTRawImageViewer::PanoImageViewer()
{
    QString panoPath = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)/0001.raw";
    QFile panoFile(panoPath);
    if (!panoFile.open(QFile::ReadOnly))
        return;
    int PanoNum = 1249;
    QByteArray ba = panoFile.readAll();
    const uchar* data = (const uchar*) ba.constData();;
    panoFile.close();
    //QImage image(data, 30, 30, QImage::Format_RGB32);
    QImage* panoImage = new QImage(data, 150, 470, QImage::Format_RGB555);
    QImage pano_Image(*panoImage);
    QPixmap panoPix;
    panoPix = QPixmap::fromImage(pano_Image,Qt::AutoColor);
    //ui->PanoLabel->setPixmap(panoPix);

//    ui->PanoGraphicsView->setPixmap(pix);
}

void CBCTRawImageViewer::CephImageViewer()
{
    QString cephPath = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)/0001.raw";
    QFile cephFile(cephPath);
    if (!cephFile.open(QFile::ReadOnly))
        return;
    int CephNum = 1749;
    QByteArray ba = cephFile.readAll();
    const uchar* data = (const uchar*) ba.constData();;
    cephFile.close();
    //QImage image(data, 30, 30, QImage::Format_RGB32);
    QImage* cephImage = new QImage(data, 150, 470, QImage::Format_RGB555);
    QImage ceph_Image(*cephImage);
    QPixmap   cephPix;
    cephPix = QPixmap::fromImage(ceph_Image,Qt::AutoColor);
    //ui->CephLabel->setPixmap(cephPix);
}
