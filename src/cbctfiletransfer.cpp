#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"

CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("127.0.0.1", 8009);
    CBCTSocket->waitForConnected();

    connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveSubServerProtocol()));
    protocol->sendProtocol(CBCTSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");


//    FileSocket = new QTcpSocket(this);
//    FileSocket->connectToHost("127.0.0.1", 8009);
//    FileSocket->waitForConnected();
}

void CBCTFileTransfer::sendControl(int buttonIdx)
{

    protocol->sendProtocol(CBCTSocket, "CTL", buttonIdx, "Pano or Ceph");
}

void CBCTFileTransfer::receiveButtonControl()
{
    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(CBCTSocket);
}
CBCTFileTransfer::~CBCTFileTransfer()
{
    CBCTSocket->close();
    delete CBCTSocket;
    delete protocol;
}
void CBCTFileTransfer::sendCTProtocol()
{

}
