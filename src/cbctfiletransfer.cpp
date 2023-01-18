#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"

CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("192.168.0.46", 8001);
    CBCTSocket->waitForConnected();

    connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveSubServerProtocol()));
    protocol->sendProtocol(CBCTSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");



//    FileSocket = new QTcpSocket(this);
//    FileSocket->connectToHost("127.0.0.1", 8009);
//    FileSocket->waitForConnected();
}

void CBCTFileTransfer::sendButtonControl(int buttonIdx)
{

    protocol->sendProtocol(CBCTSocket, "CTL", buttonIdx, "Pano or Ceph");

}

void CBCTFileTransfer::receiveControl()
{
    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(CBCTSocket);

    if(protocol->packetData()->event() == "CTL")
    {
        emit sendButtonSignal(protocol->packetData()->type());
    }

}



CBCTFileTransfer::~CBCTFileTransfer()
{
    CBCTSocket->close();
    delete CBCTSocket;
    delete protocol;
}
//void CBCTFileTransfer::sendCTProtocol()
//{

//}
