#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbctfiletransfer.h"
#include "cbctlogthread.h"
#include "cbctrawimageviewer.h"

#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QTransform>

/* 3D .Obj Visualization */
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>

#include <vtkActor.h>
#include <vtkRenderer.h>

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 가시화 파이프라인 순서도 */
    /* PolyData -> Mapper -> Actor -> Renderer -> RenderWindow */

    /* 상부, 하부, 파노라마, 세팔로좌+우 총 4개 모듈 */


    /* Load Source */
    vtkSmartPointer<vtkOBJReader> readerOBJLowerBodyAll = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJLowerBodyAll->SetFileName("lowerbody.obj");
    readerOBJLowerBodyAll->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJUpperBodyAll = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJUpperBodyAll->SetFileName("upperbody.obj");
    readerOBJUpperBodyAll->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJPanoModuleAll = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJPanoModuleAll->SetFileName("panomodule.obj");
    readerOBJPanoModuleAll->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJCephModuleAll = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJCephModuleAll->SetFileName("cephmodule.obj");
    readerOBJCephModuleAll->Update();

    vtkSmartPointer<vtkOBJReader> readerOBJLowerBodyMain = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJLowerBodyMain->SetFileName("lowerbody.obj");
    readerOBJLowerBodyMain->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJUpperBodyMain = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJUpperBodyMain->SetFileName("upperbody.obj");
    readerOBJUpperBodyMain->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJPanoModuleMain = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJPanoModuleMain->SetFileName("panomodule.obj");
    readerOBJPanoModuleMain->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJCephModuleMain = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJCephModuleMain->SetFileName("cephmodule.obj");
    readerOBJCephModuleMain->Update();

    vtkSmartPointer<vtkOBJReader> readerOBJLowerBodySub = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJLowerBodySub->SetFileName("lowerbody.obj");
    readerOBJLowerBodySub->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJUpperBodySub = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJUpperBodySub->SetFileName("upperbody.obj");
    readerOBJUpperBodySub->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJPanoModuleSub = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJPanoModuleSub->SetFileName("panomodule.obj");
    readerOBJPanoModuleSub->Update();
    vtkSmartPointer<vtkOBJReader> readerOBJCephModuleSub = vtkSmartPointer<vtkOBJReader>::New();
    readerOBJCephModuleSub->SetFileName("cephmodule.obj");
    readerOBJCephModuleSub->Update();

    //import 로 mtl  파일 읽기
    vtkSmartPointer<vtkNamedColors> colorsAll = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorAll = colorsAll->GetColor3d("Black");
    vtkColor3d actorColorAll = colorsAll->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsMain = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorMain = colorsMain->GetColor3d("Black");
    vtkColor3d actorColorMain = colorsMain->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsSub = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorSub = colorsSub->GetColor3d("Black");
    vtkColor3d actorColorSub = colorsSub->GetColor3d("HoneyDew");

    //    vtkSmartPointer<vtkPointData> pointDataLowerBody = readerOBJLowerBodyAll->GetOutput()->GetPointData();
    //    vtkSmartPointer<vtkPointData> pointDataUpperBody = readerOBJUpperBodyAll->GetOutput()->GetPointData();
    //    vtkSmartPointer<vtkPointData> pointDataPanoModule = readerOBJPanoModuleAll->GetOutput()->GetPointData();
    //    vtkSmartPointer<vtkPointData> pointDataCephModule = readerOBJCephModuleAll->GetOutput()->GetPointData();

    //    vtkSmartPointer<vtkPolyDataNormals> normalsLowerBody = vtkSmartPointer<vtkPolyDataNormals>::New();
    //    normalsLowerBody->SetInputData(readerOBJLowerBody->GetOutput());
    //    normalsLowerBody->ComputePointNormalsOn();
    //    normalsLowerBody->ComputeCellNormalsOn();
    //    normalsLowerBody->Update();

    //    vtkSmartPointer<vtkPolyDataNormals> normalsUpperBody = vtkSmartPointer<vtkPolyDataNormals>::New();
    //    normalsLowerBody->SetInputData(readerOBJUpperBody->GetOutput());
    //    normalsLowerBody->ComputePointNormalsOn();
    //    normalsLowerBody->ComputeCellNormalsOn();
    //    normalsLowerBody->Update();

    //    vtkSmartPointer<vtkPolyDataNormals> normalsPanoModule = vtkSmartPointer<vtkPolyDataNormals>::New();
    //    normalsPanoModule->SetInputData(readerOBJPanoModule->GetOutput());
    //    normalsPanoModule->ComputePointNormalsOn();
    //    normalsPanoModule->ComputeCellNormalsOn();
    //    normalsPanoModule->Update();

    //    vtkSmartPointer<vtkPolyDataNormals> normalsCephModule = vtkSmartPointer<vtkPolyDataNormals>::New();
    //    normalsCephModule->SetInputData(readerOBJPanoModule->GetOutput());
    //    normalsCephModule->ComputePointNormalsOn();
    //    normalsCephModule->ComputeCellNormalsOn();
    //    normalsCephModule->Update();

    /* Create Mapper & Actor */
    vtkSmartPointer<vtkPolyDataMapper> mapperLowerBodyAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperLowerBodyAll->SetInputData(readerOBJLowerBodyAll->GetOutput());
    mapperLowerBodyAll->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperUpperBodyAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperUpperBodyAll->SetInputData(readerOBJUpperBodyAll->GetOutput());
    mapperUpperBodyAll->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperPanoModuleAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperPanoModuleAll->SetInputData(readerOBJPanoModuleAll->GetOutput());
    mapperPanoModuleAll->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperCephModuleAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperCephModuleAll->SetInputData(readerOBJCephModuleAll->GetOutput());
    mapperCephModuleAll->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapperLowerBodyMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperLowerBodyMain->SetInputData(readerOBJLowerBodyMain->GetOutput());
    mapperLowerBodyMain->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperUpperBodyMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperUpperBodyMain->SetInputData(readerOBJUpperBodyMain->GetOutput());
    mapperUpperBodyMain->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperPanoModuleMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperPanoModuleMain->SetInputData(readerOBJPanoModuleMain->GetOutput());
    mapperPanoModuleMain->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperCephModuleMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperCephModuleMain->SetInputData(readerOBJCephModuleMain->GetOutput());
    mapperCephModuleMain->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapperLowerBodySub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperLowerBodySub->SetInputData(readerOBJLowerBodySub->GetOutput());
    mapperLowerBodySub->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperUpperBodySub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperUpperBodySub->SetInputData(readerOBJUpperBodySub->GetOutput());
    mapperUpperBodySub->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperPanoModuleSub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperPanoModuleSub->SetInputData(readerOBJPanoModuleSub->GetOutput());
    mapperPanoModuleSub->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapperCephModuleSub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperCephModuleSub->SetInputData(readerOBJCephModuleSub->GetOutput());
    mapperCephModuleSub->Update();


    vtkSmartPointer<vtkActor> actorLowerBodyAll = vtkSmartPointer<vtkActor>::New();
    actorLowerBodyAll->SetMapper(mapperLowerBodyAll);
    actorLowerBodyAll->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Light Gray").GetData());
    actorLowerBodyAll->GetProperty()->SetSpecular(5.0);
    actorLowerBodyAll->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorUpperBodyAll = vtkSmartPointer<vtkActor>::New();
    actorUpperBodyAll->SetMapper(mapperUpperBodyAll);
    actorUpperBodyAll->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("White").GetData());
    actorUpperBodyAll->GetProperty()->SetSpecular(5.0);
    actorUpperBodyAll->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorPanoModuleAll = vtkSmartPointer<vtkActor>::New();
    actorPanoModuleAll->SetMapper(mapperPanoModuleAll);
    actorPanoModuleAll->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Red").GetData());
    actorPanoModuleAll->GetProperty()->SetSpecular(5.0);
    actorPanoModuleAll->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorCephModuleAll = vtkSmartPointer<vtkActor>::New();
    actorCephModuleAll->SetMapper(mapperCephModuleAll);
    actorCephModuleAll->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Blue").GetData());
    actorCephModuleAll->GetProperty()->SetSpecular(5.0);
    actorCephModuleAll->GetProperty()->SetSpecularPower(5.0);

    vtkSmartPointer<vtkActor> actorLowerBodyMain = vtkSmartPointer<vtkActor>::New();
    actorLowerBodyMain->SetMapper(mapperLowerBodyMain);
    actorLowerBodyMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Light Gray").GetData());
    actorLowerBodyMain->GetProperty()->SetSpecular(5.0);
    actorLowerBodyMain->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorUpperBodyMain = vtkSmartPointer<vtkActor>::New();
    actorUpperBodyMain->SetMapper(mapperUpperBodyMain);
    actorUpperBodyMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("White").GetData());
    actorUpperBodyMain->GetProperty()->SetSpecular(5.0);
    actorUpperBodyMain->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorPanoModuleMain = vtkSmartPointer<vtkActor>::New();
    actorPanoModuleMain->SetMapper(mapperPanoModuleMain);
    actorPanoModuleMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Red").GetData());
    actorPanoModuleMain->GetProperty()->SetSpecular(5.0);
    actorPanoModuleMain->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorCephModuleMain = vtkSmartPointer<vtkActor>::New();
    actorCephModuleMain->SetMapper(mapperCephModuleMain);
    actorCephModuleMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Blue").GetData());
    actorCephModuleMain->GetProperty()->SetSpecular(5.0);
    actorCephModuleMain->GetProperty()->SetSpecularPower(5.0);

    vtkSmartPointer<vtkActor> actorLowerBodySub = vtkSmartPointer<vtkActor>::New();
    actorLowerBodySub->SetMapper(mapperLowerBodySub);
    actorLowerBodySub->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Light Gray").GetData());
    actorLowerBodySub->GetProperty()->SetSpecular(5.0);
    actorLowerBodySub->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorUpperBodySub = vtkSmartPointer<vtkActor>::New();
    actorUpperBodySub->SetMapper(mapperUpperBodySub);
    actorUpperBodySub->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("White").GetData());
    actorUpperBodySub->GetProperty()->SetSpecular(5.0);
    actorUpperBodySub->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorPanoModuleSub = vtkSmartPointer<vtkActor>::New();
    actorPanoModuleSub->SetMapper(mapperPanoModuleSub);
    actorPanoModuleSub->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Red").GetData());
    actorPanoModuleSub->GetProperty()->SetSpecular(5.0);
    actorPanoModuleSub->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorCephModuleSub = vtkSmartPointer<vtkActor>::New();
    actorCephModuleSub->SetMapper(mapperCephModuleSub);
    actorCephModuleSub->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("Blue").GetData());
    actorCephModuleSub->GetProperty()->SetSpecular(5.0);
    actorCephModuleSub->GetProperty()->SetSpecularPower(5.0);

    vtkSmartPointer<vtkTransform> transformAll = vtkSmartPointer<vtkTransform>::New();
    transformAll->Translate(0.0, -1000.0, 10.0);
    transformAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
    actorLowerBodyAll->SetUserTransform(transformAll);
    actorUpperBodyAll->SetUserTransform(transformAll);
    actorPanoModuleAll->SetUserTransform(transformAll);
    actorCephModuleAll->SetUserTransform(transformAll);
    transformAll->Update();

    vtkSmartPointer<vtkTransform> transformMain = vtkSmartPointer<vtkTransform>::New();
    transformMain->Translate(0.0, -200.0, 100.0);
    transformMain->RotateWXYZ(10, 0.0, 1.0, 1.0);
    actorLowerBodyMain->SetUserTransform(transformMain);
    actorUpperBodyMain->SetUserTransform(transformMain);
    actorPanoModuleMain->SetUserTransform(transformMain);
    actorCephModuleMain->SetUserTransform(transformMain);
    transformMain->Update();

    vtkSmartPointer<vtkTransform> transformSub = vtkSmartPointer<vtkTransform>::New();
    transformSub->Translate(100.0, -100.0, -500.0);
    transformSub->RotateWXYZ(10, 5.0, 10.0, 5.0);
    actorLowerBodySub->SetUserTransform(transformSub);
    actorUpperBodySub->SetUserTransform(transformSub);
    actorPanoModuleSub->SetUserTransform(transformSub);
    actorCephModuleSub->SetUserTransform(transformSub);
    transformSub->Update();

    //    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterAll = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    //    transformFilterAll->SetInputConnection(readerObjAll->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJLowBody->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJMainBody->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJPano->GetOutputPort());
    //    transformFilterAll->SetInputConnection(readerOBJCeph->GetOutputPort());

    //    transformFilterAll->SetTransform(transformAll);
    //    transformFilterAll->Update();

    //    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterMain = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    //    transformFilterMain->SetInputConnection(readerObjMain->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJLowBody->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJMainBody->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJPano->GetOutputPort());
    //    transformFilterMain->SetInputConnection(readerOBJCeph->GetOutputPort());
    //    transformFilterMain->SetTransform(transformMain);
    //    transformFilterMain->Update();

    //    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterSub = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    //    transformFilterSub->SetInputConnection(readerObjSub->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJLowBody->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJMainBody->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJPano->GetOutputPort());
    //    transformFilterSub->SetInputConnection(readerOBJCeph->GetOutputPort());
    //    transformFilterSub->SetTransform(transformSub);
    //    transformFilterSub->Update();




    /* Visualize with Cam */
    vtkSmartPointer<vtkRenderer> rendererAll = vtkSmartPointer<vtkRenderer>::New();
    rendererAll->AddActor(actorLowerBodyAll);
    rendererAll->AddActor(actorUpperBodyAll);
    rendererAll->AddActor(actorPanoModuleAll);
    rendererAll->AddActor(actorCephModuleAll);
    rendererAll->SetBackground(backgroundColorAll.GetData());
    rendererAll->ResetCamera();
    rendererAll->LightFollowCameraOn();
    rendererAll->GetActiveCamera()->Azimuth(320);
    rendererAll->GetActiveCamera()->Elevation(20);
    rendererAll->GetActiveCamera()->Zoom(1.6);
    rendererAll->ResetCameraClippingRange();

    vtkSmartPointer<vtkRenderer> rendererMain = vtkSmartPointer<vtkRenderer>::New();
    rendererMain->AddActor(actorLowerBodyMain);
    rendererMain->AddActor(actorUpperBodyMain);
    rendererMain->AddActor(actorPanoModuleMain);
    rendererMain->AddActor(actorCephModuleMain);
    rendererMain->SetBackground(backgroundColorAll.GetData());
    rendererMain->ResetCamera();
    rendererMain->LightFollowCameraOn();
    rendererMain->GetActiveCamera()->Azimuth(60);
    rendererMain->GetActiveCamera()->Elevation(40);
    rendererMain->GetActiveCamera()->Zoom(2.5);
    rendererMain->ResetCameraClippingRange();
    vtkSmartPointer<vtkRenderer> rendererSub = vtkSmartPointer<vtkRenderer>::New();
    rendererSub->AddActor(actorLowerBodySub);
    rendererSub->AddActor(actorUpperBodySub);
    rendererSub->AddActor(actorPanoModuleSub);
    rendererSub->AddActor(actorCephModuleSub);
    rendererSub->SetBackground(backgroundColorAll.GetData());
    rendererSub->ResetCamera();
    rendererSub->LightFollowCameraOn();\
    rendererSub->GetActiveCamera()->Azimuth(240);
    rendererSub->GetActiveCamera()->Elevation(15);
    rendererSub->GetActiveCamera()->Zoom(2.2);
    rendererSub->ResetCameraClippingRange();


    /* Rendering */
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowAll = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowAll->AddRenderer(rendererAll);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowMain = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowMain->AddRenderer(rendererMain);
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowSub = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowSub->AddRenderer(rendererSub);


    ui->openGLWidget_All->setRenderWindow(renderWindowAll);
    ui->openGLWidget_Main->setRenderWindow(renderWindowMain);
    ui->openGLWidget_Sub->setRenderWindow(renderWindowSub);

    ui->openGLWidget_All->interactor()->ProcessEvents();
    ui->openGLWidget_Main->interactor()->ProcessEvents();
    ui->openGLWidget_Sub->interactor()->ProcessEvents();

    ui->openGLWidget_All->interactor()->Start();
    ui->openGLWidget_Main->interactor()->Start();
    ui->openGLWidget_Sub->interactor()->Start();

    renderWindowAll->Render();
    renderWindowMain->Render();
    renderWindowSub->Render();

    //connect(ui->MainPushButton, SIGNAL(clicked()), this, SLOT(on_MainPushButton_Clicked()));
    //connect(ui->SubPushButton, SIGNAL(clicked()), this, SLOT(on_SubPushButton_Clicked()));
    //    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(CaptureStartPushButton_clicked()));
    //    connect(ui->pushButton_12,SIGNAL(clicked(bool)), this, SLOT(pushbutton_12(bool)));
    connect(ui->CaptureStartPushButton, SIGNAL(clicked()), this, SLOT(on_CaptureStartPushButton_clicked()));
    //    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(on_CaptureStopPushButton_clicked()));


    m_rawImageViewer = new CBCTRawImageViewer();
    connect(m_rawImageViewer, SIGNAL(signals_panoImage(QImage*)), this, SLOT(slot_panoImage(QImage*)));
    connect(m_rawImageViewer, SIGNAL(signals_cephImage(QImage*)), this, SLOT(slot_cephImage(QImage*)));

    m_fileTransfer = new CBCTFileTransfer(this);
    connect(ui->READYPushButton,SIGNAL(clicked),m_fileTransfer,SLOT(on_READYPushButton_clicked()));
    connect(ui->RESETPushButton,SIGNAL(clicked),m_fileTransfer,SLOT(on_RESETPushButton_clicked()));
    connect(ui->STARTPushButton,SIGNAL(clicked),m_fileTransfer,SLOT(on_STARTPushButton_clicked()));
    connect(ui->STOPPushButton,SIGNAL(clicked),m_fileTransfer,SLOT(on_STOPPushButton_clicked()));

    connect(m_fileTransfer, SIGNAL(READYSignal(ControlType)), this, SLOT(sendControl(ControlType)));
    connect(m_fileTransfer, SIGNAL(RESETSignal(ControlType)), this, SLOT(sendControl(ControlType)));
    connect(m_fileTransfer, SIGNAL(STARTSignal(ControlType)), this, SLOT(sendControl(ControlType)));
    connect(m_fileTransfer, SIGNAL(STOPSignal(ControlType)), this, SLOT(sendControl(ControlType)));

    connect(this, SIGNAL(READYSignal(ControlType)),m_fileTransfer,SLOT(sendControl(ControlType)));
    connect(this, SIGNAL(READYSignal(ControlType)),m_fileTransfer,SLOT(sendControl(ControlType)));
    connect(this, SIGNAL(READYSignal(ControlType)),m_fileTransfer,SLOT(sendControl(ControlType)));
    connect(this, SIGNAL(READYSignal(ControlType)),m_fileTransfer,SLOT(sendControl(ControlType)));



    //    connect(CBCTFileTransfer, SIGNAL(sendButtonSignal()), this, SLOT(()));
}


void Load_OBJReader(std::vector<QString> filepaths, std::vector<vtkPolyData> &data)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_READYPushButton_clicked()
{
    emit READYSignal(READY);
}

void MainWindow::on_RESETPushButton_clicked()
{
    emit RESETSignal(RESET);
}

void MainWindow::on_STARTPushButton_clicked()
{
    emit STARTSignal(START);
}

void MainWindow::on_STOPPushButton_clicked()
{
    emit STOPSignal(STOP);
}

void rotateMain()
{

}

void MainWindow::on_CaptureStartPushButton_clicked()
{


    QGraphicsScene *panoScene = new QGraphicsScene();
    QGraphicsScene *cephScene = new QGraphicsScene();



    if(ui->PanoCheckBox->isChecked())
    {
        qDebug() << __FUNCTION__;
        m_rawImageViewer->startPanoTimer();
        //CBCTRawImageViewer m_rawImageViewer;
        //QPixmap panoPix = m_rawImageViewer.PanoImageViewer();
        //ui->PanoLabel->setPixmap(panoPix);
    }

    if(ui->CephCheckBox->isChecked())
    {
        qDebug() << __FUNCTION__;
        m_rawImageViewer->startCephTimer();
        //        CBCTRawImageViewer m_rawImageViewer;
        //        QPixmap cephPix = m_rawImageViewer.CephImageViewer();
        //        m_rawImageViewer.CephImageViewer();
        //        ui->CephLabel->setPixmap(cephPix);

    }
}

void MainWindow::on_CaptureStopPushButton_clicked()
{
    m_rawImageViewer->stopPanoTimer();
    m_rawImageViewer->stopCephTimer();
}

void MainWindow::slot_panoImage(QImage* pImg)
{
    qDebug() << __FUNCTION__;

    QImage pano_Image(*pImg);
    QPixmap panoPix;
    panoPix = QPixmap::fromImage(pano_Image, Qt::AutoColor);

    /* 파노라마 이미지가 90도 회전되어 있으므로, 출력시 원상복구한다 */
    QTransform panoTransform;
    panoTransform.rotate(90);
    ui->PanoLabel->setPixmap(panoPix.transformed(panoTransform));
}

void MainWindow::slot_cephImage(QImage* cImg)
{
    qDebug() << __FUNCTION__;

    QImage ceph_Image(*cImg);
    QPixmap cephPix;
    cephPix = QPixmap::fromImage(ceph_Image, Qt::AutoColor);
    ui->CephLabel->setPixmap(cephPix);
}

void MainWindow::on_MainPushButton_clicked()
{

}
void MainWindow::on_SubPushButton_clicked()
{

}


void loadObj()
{

}

/* 상하 이동 시 LowBodyModule 제외 모든 모듈이 같은 좌표대로 이동 */
void ModuleAscending()
{

}

void ModuleDescending()
{

}

/* 앞뒤 이동 시 CephModule 만 이동 */
void CephBackandForth()
{

}

/* 회전 이동 시 PanoModule 만 이동 */
void PanoRotation()
{

}

