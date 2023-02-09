#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"


CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("10.222.0.164", 8002);
    if(CBCTSocket->waitForConnected(100))
    {
        qDebug("CBCT Connected");
        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
        //        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveModality()));
        protocol->sendProtocol(CBCTSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
    }
    else
    {
        qDebug("CBCT Not Connected");
    }

    fileSocket = new QTcpSocket(this);
    fileSocket->connectToHost("10.222.0.164", 8003);
    if(fileSocket->waitForConnected(100))
    {
        qDebug("File Transfer Ready");
        connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
        //        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveModality()));
        protocol->sendProtocol(fileSocket, "NEW", ConnectType::MODALITY, "CBCT FILE TRANSFER READY");
    }
    else
    {
        qDebug("File Transfer Not Ready");
    }
}

void CBCTFileTransfer::sendPanoFile(int panoValue)
{
    QString modality = protocol->packetData()->msg();

    qDebug() << protocol->packetData()->msg();

     int countMax = 0;

    if(modality == "PANO")
    {
        countMax = 1750;
    }
    // PANO MODE
        QFile *panoFile;
        panoFile = new QFile;
        QString panoFileName;
   

    if(panoValue >= countMax)
    {
        return;
    }
    if (panoValue >= 1000)
    {
    //    qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/resources/Pano_Frame(1152x64)/%1.raw").arg(panoValue);
    }
    else if (panoValue < 1000 && panoValue >= 100)
    {
   //     qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/resources/Pano_Frame(1152x64)/0%1.raw").arg(panoValue);
    }
    else if (panoValue < 100 && panoValue >= 10)
    {
   //     qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/resources/Pano_Frame(1152x64)/00%1.raw").arg(panoValue);
    }
    else
    {
   //     qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/resources/Pano_Frame(1152x64)/000%1.raw").arg(panoValue);
    }
    panoFile->setFileName(panoFileName);
  //  qDebug() << panoFileName;
    if(!panoFile->exists())
    {
        return;
    }
    qDebug() << panoFileName;
    qDebug() << panoValue;

    bool panoOpen = panoFile->open(QIODevice::ReadOnly);
    fileSocket->write(panoFile->readAll());

    if(panoOpen)
    {
    emit fileLogSignal("PANORAMA MODE", panoFileName);
    }
    else
    {
    emit fileLogSignal("No Panorama Files", panoFileName);
    }

    panoFile->close();
    panoFile->deleteLater();
}

void CBCTFileTransfer::sendCephFile(int cephValue)
{
    QString modality = protocol->packetData()->msg();

    qDebug() << protocol->packetData()->msg();

int countMax = 0;

    if(modality == "CEPH")
    {
        countMax = 1250;
    }


    // CEPH MODE
        QFile* cephFile;
        cephFile = new QFile;
        QString cephFileName;

    if(cephValue >= countMax)
    {
        return;
    }
    if (cephValue >= 1000)
    {
        qDebug() << cephValue;
        cephFileName = QString("C:/Qt_VTK_CT/resources/Ceph_Frame(48x2400)/%1.raw").arg(cephValue);
    }
    else if (cephValue < 1000 && cephValue >= 100)
    {
        qDebug() << cephValue;
        cephFileName = QString("C:/Qt_VTK_CT/resources/Ceph_Frame(48x2400)/0%1.raw").arg(cephValue);
    }
    else if (cephValue < 100 && cephValue >= 10)
    {
        qDebug() << cephValue;
        cephFileName = QString("C:/Qt_VTK_CT/resources/Ceph_Frame(48x2400)/00%1.raw").arg(cephValue);
    }
    else
    {
        qDebug() << cephValue;
        cephFileName = QString("C:/Qt_VTK_CT/resources/Ceph_Frame(48x2400)/000%1.raw").arg(cephValue);
    }
    cephFile->setFileName(cephFileName);
    if(!cephFile->exists())
    {
        return;
    }
    bool cephOpen = cephFile->open(QIODevice::ReadOnly);
    fileSocket->write(cephFile->readAll());

    /* 전송되는 이미지를 테이블위젯에 출력 */
    if(cephOpen)
    {
    emit fileLogSignal("CEPHALO MODE", cephFileName);
    }
    else
    {
    emit fileLogSignal("No Cephalo Files", cephFileName);
    }

    cephFile->close();
    cephFile->deleteLater();
}

void CBCTFileTransfer::sendingControl(int buttonIdx, QString msg)
{
    protocol->sendProtocol(CBCTSocket, "CTL", buttonIdx, msg);
}

void CBCTFileTransfer::receiveControl()
{
    qDebug("control receive test");
    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(CBCTSocket);

    if(protocol->packetData()->event() == "CTL")
    {
        int control = protocol->packetData()->type();
        QString modality = protocol->packetData()->msg();

        emit modality_Signal(modality);

        switch (control) {
        case 0:
            qDebug("RESET Received");
            emit receiveResetSignal();
            break;
        case 1:
            qDebug("READY Received");

            if(modality == "PANO"){
                qDebug("Pano Modality Received");
                emit receivePanoSignal();
            }
            else if (modality == "CEPH"){
                qDebug("Ceph Modlality Received");
                emit receiveCephSignal();
            }
            else{
                qDebug("WRONG MODAL STATE");
            }
            emit receiveReadySignal();
            break;
        case 2:
            qDebug("START Received");
            emit receiveStartSignal();
            break;
        case 3:
            qDebug("STOP Received");
            emit receiveStopSignal();
            break;
        }
    }
}


CBCTFileTransfer::~CBCTFileTransfer()
{
    CBCTSocket->close();
    fileSocket->close();
    delete CBCTSocket;
    delete fileSocket;
    delete protocol;
}
