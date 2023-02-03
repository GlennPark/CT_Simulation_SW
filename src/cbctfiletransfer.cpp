#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"

CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("192.168.0.20", 8002);
    if(CBCTSocket->waitForConnected())
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
    fileSocket->connectToHost("192.168.0.20", 8003);
    if(fileSocket->waitForConnected())
    {
        qDebug("File Transfer Ready");
        //        connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
        //        protocol->sendProtocol(fileSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
    }
    else
    {
        qDebug("File Transfer Not Ready");
    }

    //    FileSocket = new QTcpSocket(this);
    //    FileSocket->connectToHost("127.0.0.1", 8009);
    //    FileSocket->waitForConnected();
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
    if(panoValue >= countMax)
    {
        return;
    }
    if (panoValue >= 1000)
    {
        qDebug() << panoValue;
        fileName = QString("./Pano_Frame(1152x64)/%1.raw").arg(modality).arg(panoValue);
    }
    else if (panoValue < 1000 && panoValue >= 100)
    {
        qDebug() << panoValue;
        fileName = QString("./Pano_Frame(1152x64)/0%1.raw").arg(modality).arg(panoValue);
    }
    else if (panoValue < 100 && panoValue >= 10)
    {
        qDebug() << panoValue;
        fileName = QString("./Pano_Frame(1152x64)/00%1.raw").arg(modality).arg(panoValue);
    }
    else
    {
        qDebug() << panoValue;
        fileName = QString("./Pano_Frame(1152x64)/000%1.raw").arg(modality).arg(panoValue);
    }
    file->setFileName(fileName);
    if(!file->exists())
    {
        return;
    }
    file->open(QIODevice::ReadOnly);
    fileSocket->write(file->readAll());
    file->close();
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


    // PANO MODE
    if(cephValue >= countMax)
    {
        return;
    }
    if (cephValue >= 1000)
    {
        qDebug() << cephValue;
        fileName = QString("./Ceph_Frame(1152x64)/%1.raw").arg(modality).arg(cephValue);
    }
    else if (cephValue < 1000 && cephValue >= 100)
    {
        qDebug() << cephValue;
        fileName = QString("./Ceph_Frame(1152x64)/0%1.raw").arg(modality).arg(cephValue);
    }
    else if (cephValue < 100 && cephValue >= 10)
    {
        qDebug() << cephValue;
        fileName = QString("./Ceph_Frame(1152x64)/00%1.raw").arg(modality).arg(cephValue);
    }
    else
    {
        qDebug() << cephValue;
        fileName = QString("./Ceph_Frame(1152x64)/000%1.raw").arg(modality).arg(cephValue);
    }
    file->setFileName(fileName);
    if(!file->exists())
    {
        return;
    }
    file->open(QIODevice::ReadOnly);
    fileSocket->write(file->readAll());
    file->close();
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

        switch (control) {
        case 0:
            qDebug("RESET Received");
            emit resetSignal();
            break;
        case 1:
            qDebug("READY Received");

            if(modality == "PANO"){
                qDebug("Pano Modality Received");
                emit panoSignal();
            }
            else if (modality == "CEPH"){
                qDebug("Ceph Modlality Received");
                emit cephSignal();
            }
            else{
                qDebug("WRONG MODAL STATE");
            }
            emit readySignal();
            break;
        case 2:
            qDebug("START Received");
            emit startSignal();
            break;
        case 3:
            qDebug("STOP Received");
            emit stopSignal();
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
