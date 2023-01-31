#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "cbctfiletransfer.h"
//#include "cbctlogthread.h"
//#include "cbctrawimageviewer.h"
#include <QMessageBox>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QTransform>
#include <qstring.h>
#include <qevent.h>
#include <QThread>
/* 3D .Obj Visualization */

#include <vtkRenderWindow.h>
#include<vtkOBJExporter.h>
#include <vtkPlaneSource.h>
#include <vtkAxesActor.h>
#include <vtkOBJImporter.h>
#include <vtkObject.h>
#include <vtkOBJReader.h>


#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkImporter.h>

/* 3D .Obj Movement */
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>


#include <qvtkopenglstereowidget.h>
#include <QVTKRenderWidget.h>
#include <QVTKOpenGLWindow.h>
#include <vtkCamera.h>
#include <vtkNew.h>

#include <qopenglwidget.h>

/* 2D Visualization */
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <InteractionContext.h>
#include <QDebug>


//#define USE_DISPLAY_GLOBALAXIS


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Model Controller 생성.
    m_modelController = new CBCTModelController(ui);
    m_rawImageViewer = new CBCTRawImageViewer();
    m_fileTransfer = new CBCTFileTransfer(this);
    if (!m_modelController->initialize())
        qDebug() << "CBCTModelController initialize Fail ! ";

    /* 상승, 하강, 메인, 서브 버튼 클릭시 VTK model 기능 동작 */
    connect(ui->AscendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(ascending_Function()));
    connect(ui->DescendingPushButton, SIGNAL(clicked()), m_modelController, SLOT(descending_Function()));
    connect(ui->MainPushButton, SIGNAL(clicked()), m_modelController, SLOT(panorama_Module_Function()));
    connect(ui->SubPushButton, SIGNAL(clicked()), m_modelController, SLOT(cephalo_Module_Function()));

    /* 촬영 리셋, 준비, 시작, 정지 버튼 클릭시 VTK model 기능 동작 */
//    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), m_modelController, SLOT(reset_VTK_Function()));
//    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), m_modelController, SLOT(ready_VTK_Function()));
//    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), m_modelController, SLOT(start_VTK_Function()));
//    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), m_modelController, SLOT(stop_VTK_Function()));

    /* 촬영 리셋, 준비, 시작, 정지 버튼 클릭시 Raw Image Viewer 기능 동작 */
    connect(ui->CaptureResetPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureResetPushButton_clicked()));
    connect(ui->CaptureReadyPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureReadyPushButton_clicked()));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
    connect(ui->CaptureStopPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));

    /* 촬영 SW에서 Signal 받았을 시 VTK model 기능 동작 */
//    connect(m_fileTransfer, SIGNAL(resetSignal()), m_modelController, SLOT(reset_VTK_Function()));
//    connect(m_fileTransfer, SIGNAL(readySignal()), m_modelController, SLOT(ready_VTK_Function()));
//    connect(m_fileTransfer, SIGNAL(startSignal()), m_modelController, SLOT(start_VTK_Function()));
//    connect(m_fileTransfer, SIGNAL(stopSignal()), m_modelController, SLOT(stop_VTK_Function()));

    /* 촬영 SW에서 Signal 받았을 시 Raw Image Viewer 기능 동작 */
    connect(m_fileTransfer, SIGNAL(resetSignal()), this, SLOT(on_CaptureResetPushButton_clicked()));
    connect(m_fileTransfer, SIGNAL(readySignal()), this, SLOT(on_CaptureReadyPushButton_clicked()));
    connect(m_fileTransfer, SIGNAL(startSignal()), this, SLOT(on_CaptureStartPushButton_clicked()));
    connect(m_fileTransfer, SIGNAL(stopSignal()), this, SLOT(on_CaptureStopPushButton_clicked()));

    /* Raw Image Viewer 기능을 Mainwindow Ui 에 구현하기 위해 연결 */
//    connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
//    connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));

    connect(ui->PanoGraphicsView, SIGNAL(activa), m_rawImageViewer, SLOT(slot_panoImage(QImage*)));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), m_rawImageViewer, SLOT(slot_cephImage(QImage*)));


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
    auto Size = event->size();

    //ui->openGLWidget_All->resize(Size);
    QMainWindow::resizeEvent(event);
}


void MainWindow::on_CaptureResetPushButton_clicked()
{
    qDebug() << "RESET control received";
    m_modelController->reset_VTK_Function();
    m_rawImageViewer->resetPanoTimer();

    if(true)
    {

    }
    else
    {
        emit STARTSignal(RESET);
    }
}

void MainWindow::on_CaptureReadyPushButton_clicked()
{

    qDebug() << "READY control received";
    m_modelController->ready_VTK_Fucntion();
    m_rawImageViewer->readyPanoTimer();

    if(true)
    {

    }
    else
    {
        emit STARTSignal(READY);
    }
}

void MainWindow::on_CaptureStartPushButton_clicked()
{
    qDebug() << "START control received";
    if (ui->PanoCheckBox->isChecked())
    {
        if (ui->CephCheckBox->isChecked())
        {
            m_panoErrorMessage = new QMessageBox;
m_panoErrorMessage:ERROR_LOG_POLICY_CONFLICT;
        }
        else
        {
            qDebug() << __FUNCTION__;
            m_rawImageViewer->startPanoTimer();
            m_modelController->panorama_Module_VTK_Function();

        }

        //CBCTRawImageViewer m_rawImageViewer;
        //QPixmap panoPix = m_rawImageViewer.PanoImageViewer();
        //ui->PanoLabel->setPixmap(panoPix);
    }

    if (ui->CephCheckBox->isChecked())
    {
        if (ui->PanoCheckBox->isChecked())
        {
            m_cephErrorMessage = new QMessageBox;
m_cephErrorMessage:ERROR_LOG_POLICY_CONFLICT;
        }
        else
        {
            qDebug() << __FUNCTION__;
            m_rawImageViewer->startCephTimer();
            m_modelController->cephalo_Module_VTK_Function();
        }
        //        CBCTRawImageViewer m_rawImageViewer;
        //        QPixmap cephPix = m_rawImageViewer.CephImageViewer();
        //        m_rawImageViewer.CephImageViewer();
        //        ui->CephLabel->setPixmap(cephPix);

    }
    if(true)
    {

    }
    else
    {
        emit STARTSignal(START);
    }
}

void MainWindow::on_CaptureStopPushButton_clicked()
{
    qDebug() << " control received";
    m_rawImageViewer->stopPanoTimer();
    m_rawImageViewer->stopCephTimer();
    m_modelController->stop_VTK_Function();

    if(true)
    {

    }
    else
    {
        emit STARTSignal(STOP);
    }
}


//void MainWindow::slot_panoImage(QImage* pImg)
//{
//    qDebug() << __FUNCTION__;
//    QGraphicsScene* panoScene = new QGraphicsScene();


//    QImage pano_Image(*pImg);
//    QPixmap panoPix;
//    panoPix = QPixmap::fromImage(pano_Image, Qt::AutoColor);

//    /* 파노라마 이미지가 90도 회전되어 있으므로, 출력시 원상복구한다 */
//    QTransform panoTransform;
//    panoTransform.rotate(90);
//    panoScene->addPixmap(panoPix.transformed(panoTransform));
//    ui->PanoLabel->setPixmap(panoPix.transformed(panoTransform));
//    ui->PanoGraphicsView->setScene(panoScene);
//    /* 파노라마 Raw Image 전송상태를 표시해주는 ProgressBar */
//    int panoValue = ui->PanoProgressBar->value();
//    panoValue++;
//    ui->PanoProgressBar->setValue(panoValue);
//}

//void MainWindow::slot_cephImage(QImage* cImg)
//{
//    qDebug() << __FUNCTION__;
//    QGraphicsScene* cephScene = new QGraphicsScene();


//    QImage ceph_Image(*cImg);
//    QPixmap cephPix;
//    cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
//    cephScene->addPixmap(cephPix);
//    ui->CephLabel->setPixmap(cephPix);
//    ui->CephGraphicsView->setScene(cephScene);
//    /* 세팔로 Raw Image 전송상태를 표시해주는 ProgressBar */
//    int cephValue = ui->CephProgressBar->value();
//    cephValue++;
//    ui->CephProgressBar->setValue(cephValue);

//}



//void MainWindow::on_openGLWidget_All_resized()
//{
//    auto size = this->size();
//    ui->openGLWidget_All->resize(size);
//}

