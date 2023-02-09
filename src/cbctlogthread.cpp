#include "cbctlogthread.h"
#include <QDebug>
#include <QThread>

CBCTLogThread::CBCTLogThread()
{
 }


void CBCTLogThread::run(){

}

void CBCTLogThread::runMethod()
{
    m_modelController = new CBCTModelController;
    m_rawImageViewer = new CBCTRawImageViewer;

    QThread* modelThread = new QThread;
    QThread* viewThread = new QThread;

    m_modelController->moveToThread(modelThread);
    m_rawImageViewer->moveToThread(viewThread);

    connect(modelThread, &QThread::started, m_modelController, &CBCTModelController::on_MainPushButton_clicked);
    connect(viewThread, &QThread::started, m_rawImageViewer, &CBCTRawImageViewer::viewer_Output);

}

//void CBCTLogThread_2::run()
//{
//    m_rawImageViewer->startCephTimer();
//    m_modelController->on_SubPushButton_clicked();
//}

//void CBCTLogThread_1::pano_Capture_Start_Thread()
//{
//    m_modelController->on_MainPushButton_clicked();
//}

//void CBCTLogThread_1::pano_Capture_Start_Slot()
//{
//    CBCTLogThread_1 *logThread_1 = new CBCTLogThread_1();
//    connect(logThread_1, &CBCTLogThread_1::finished, logThread_1, &QObject::deleteLater);
//    logThread_1->start();
//}

//void CBCTLogThread_2::ceph_Capture_Start_Thread()
//{
//    m_modelController->on_SubPushButton_clicked();
//}

//void CBCTLogThread_2::ceph_Capture_Start_Slot()
//{
//    CBCTLogThread_2 *logThread_2 = new CBCTLogThread_2();
//    connect(logThread_2, &CBCTLogThread_2::finished, logThread_2, &QObject::deleteLater);
//    logThread_2->start();
//}
// Load Obj 를 Thread 안에 넣을 것 -> 효율성 증가
