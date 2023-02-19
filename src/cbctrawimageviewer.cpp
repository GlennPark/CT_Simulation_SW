#include "cbctrawimageviewer.h"
#include <QFile>
#include <QImage>
#include <QtWidgets>
#include <QPixmap>
#include <QDebug>


#define update_image();

CBCTRawImageViewer::CBCTRawImageViewer()
{
    m_parentUI = new Ui::MainWindow;
}



void CBCTRawImageViewer::resetPanoTimer()
{
    isRunningPano = true;
    qDebug() << __FUNCTION__;
}

void CBCTRawImageViewer::resetCephTimer()
{
    isRunningCeph = true;
    qDebug() << __FUNCTION__;
}

void CBCTRawImageViewer::startPanoTimer()
{
    qDebug() << __FUNCTION__;
   // isRunningPano = true;
    for (int i = 0; i < 1750; i++)
    {
        if (isRunningPano == false)
        {
            return;
        }

        QString panoFileName;
        if (i >= 1000)
        {
            panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/%1.raw").arg(i);
        }
        else if (i < 1000 && i >= 100)
        {
            panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/0%1.raw").arg(i);
        }
        else if (i < 100 && i >= 10)
        {
            panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/00%1.raw").arg(i);
        }
        else
        {
            panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/000%1.raw").arg(i);
        }
        
        if (i == 1749)
        {

        }
        QFile panoFile(panoFileName);
        if (!panoFile.open(QFile::ReadOnly))
            return;
        QByteArray pBa = panoFile.readAll();
        const uchar* pData = (const uchar*)pBa.constData();;
        panoFile.close();
        panoFile.deleteLater();
        QImage* panoImage = new QImage(pData, 100, 740, QImage::Format_Grayscale16);

        emit signals_panoImage(panoImage);
    }
}


void CBCTRawImageViewer::startCephTimer()
{
    // 타이머 함수 -> mainwindow로 이미지 파일 하나씩 읽어 보내주도록 한다.
    qDebug() << __FUNCTION__;
   // isRunningCeph = true;
    for (int i = 0; i < 1250; i++)
    {
        if (isRunningCeph == false)
        {
            return;
        }
        QString cephFileName;
        if (i >= 1000)
        {
            cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/%1.raw").arg(i);
        }
        else if (i < 1000 && i >= 100)
        {
            cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/0%1.raw").arg(i);
        }
        else if (i < 100 && i >= 10)
        {
            cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/00%1.raw").arg(i);
        }
        else
        {
            cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/000%1.raw").arg(i);
        }

        if (i == 1249)
        {

        }
        QFile cephFile(cephFileName);
        if (!cephFile.open(QFile::ReadOnly))
            return;
        QByteArray cBa = cephFile.readAll();
        const uchar* cData = (const uchar*)cBa.constData();;
        cephFile.close();
        cephFile.deleteLater();
        QImage* cephImage = new QImage(cData, 100, 740, QImage::Format_Grayscale16);

        emit signals_cephImage(cephImage);
    }
}


void CBCTRawImageViewer::stopPanoTimer()
{
    // 타이머 종료
    qDebug() << __FUNCTION__;
    isRunningPano = false;

}

void CBCTRawImageViewer::stopCephTimer()
{
    // 타이머 종료
    qDebug() << __FUNCTION__;
    isRunningCeph = false;
}

CBCTRawImageViewer::~CBCTRawImageViewer()
{

}
