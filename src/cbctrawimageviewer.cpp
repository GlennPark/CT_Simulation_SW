#include "cbctrawimageviewer.h"
#include "mainWindow.h"
#include <QFile>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtWidgets>
#include <QTimer>
#include <QPixmap>


#define update_image();

CBCTRawImageViewer::CBCTRawImageViewer()
{

}



QPixmap CBCTRawImageViewer::PanoImageViewer()
{

    QString folder = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)";
    panoImageIterator = new QDirIterator(folder, QDirIterator::Subdirectories);

    panoImageTimer = new QTimer(this);
    connect(panoImageTimer, &QTimer::timeout, this, &CBCTRawImageViewer::ShowNextPanoImage);
    panoImageTimer->start(10);

    if (panoImageIterator->hasNext())
    {
        panoImageIterator->next();

        qDebug() << __FUNCTION__ << panoImageIterator->filePath();

//        QPixmap panoImage = PanoImageViewer(panoImageIterator->filePath());
        QPixmap panoPix(panoImageIterator->filePath());
        return panoPix;
    }
    else
    {
        //imageIterator->rewind();
    }
//    ShowNextPanoImage();
//    QString panoPath = "C:/Qt_VTK_CT/build/Debug/Pano_Frame(1152x64)/0001.raw";
//    QFile panoFile(panoPath);
//    if (!panoFile.open(QFile::ReadOnly))
//        return QPixmap();
//    int PanoNum = 1249;
//    QByteArray ba = panoFile.readAll();
//    const uchar* data = (const uchar*) ba.constData();;
//    panoFile.close();
//    //QImage image(data, 30, 30, QImage::Format_RGB32);
//    QImage* panoImage = new QImage(data, 150, 470, QImage::Format_RGB555);
//    QImage pano_Image(*panoImage);
//    QPixmap panoPix;
//    panoPix = QPixmap::fromImage(pano_Image,Qt::AutoColor);
//    return panoPix;

    //ui->PanoLabel->setPixmap(panoPix);

    //    ui->PanoGraphicsView->setPixmap(pix);
}

//QPixmap CBCTRawImageViewer::ShowNextPanoImage()
//{
//    if (panoImageIterator->hasNext())
//    {
//        panoImageIterator->next();

//        qDebug() << __FUNCTION__ << panoImageIterator->filePath();

//       QPixmap panoImage = PanoImageViewer(panoImageIterator->filePath());
//        QPixmap panoPix(panoImageIterator->filePath());
//        return panoPix;
//    }
//    else
//    {
//        //imageIterator->rewind();
//    }

//}

QPixmap CBCTRawImageViewer::CephImageViewer()
{
    QString folder = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)";
    cephImageIterator = new QDirIterator(folder, QDirIterator::Subdirectories);

    cephImageTimer = new QTimer(this);
    connect(cephImageTimer, &QTimer::timeout, this, &CBCTRawImageViewer::ShowNextCephImage);
    cephImageTimer->start(10);

    if (cephImageIterator->hasNext())
    {
        cephImageIterator->next();

        qDebug() << __FUNCTION__ << cephImageIterator->filePath();

       // QPixmap panoPix = PanoImageViewer(cephImageIterator->filePath());
        QPixmap cephPix(cephImageIterator->filePath());
        return cephPix;
    }
    else
    {
        //imageIterator->rewind();
    }

//    ShowNextCephImage();
//    QString cephPath = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)/0001.raw";
//    QFile cephFile(cephPath);
//    if (!cephFile.open(QFile::ReadOnly))
//        return QPixmap();
//    int CephNum = 1749;
//    QByteArray ba = cephFile.readAll();
//    const uchar* data = (const uchar*) ba.constData();;
//    cephFile.close();
//    QImage image(data, 30, 30, QImage::Format_RGB32);
//    QImage* cephImage = new QImage(data, 150, 470, QImage::Format_RGB555);
//    QImage ceph_Image(*cephImage);
//    QPixmap cephPix;
//    cephPix = QPixmap::fromImage(ceph_Image,Qt::AutoColor);
//    return cephPix;
    //ui->CephLabel->setPixmap(cephPix);
}

//QPixmap CBCTRawImageViewer::ShowNextCephImage()
//{
//    if (cephImageIterator->hasNext())
//    {
//        cephImageIterator->next();

//        qDebug() << __FUNCTION__ << cephImageIterator->filePath();

//       // QPixmap panoPix = PanoImageViewer(cephImageIterator->filePath());
//        QPixmap cephPix(cephImageIterator->filePath());
//        return cephPix;
//    }
//    else
//    {r
//        //imageIterator->rewind();
//    }

//}
CBCTRawImageViewer::~CBCTRawImageViewer()
{

}
