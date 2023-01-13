#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

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

#include <QVTKOpenGLStereoWidget.h>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 가시화 파이프라인 순서도 */
    /* PolyData -> Mapper -> Actor -> Renderer -> RenderWindow */

    /* 상부, 하부, 파노라마, 세팔로좌+우 총 4개 모듈 */


    /* Load Source */
    vtkSmartPointer<vtkOBJReader> readerObjAll = vtkSmartPointer<vtkOBJReader>::New();
    readerObjAll->SetFileName("CTObj.obj");
    readerObjAll->Update();
    vtkSmartPointer<vtkOBJReader> readerObjMain = vtkSmartPointer<vtkOBJReader>::New();
    readerObjMain->SetFileName("CTObj.obj");
    readerObjMain->Update();
    vtkSmartPointer<vtkOBJReader> readerObjSub = vtkSmartPointer<vtkOBJReader>::New();
    readerObjSub->SetFileName("CTObj.obj");
    readerObjSub->Update();

//    vtkSmartPointer<vtkOBJReader> readerOBJLowBody = vtkSmartPointer<vtkOBJReader>::New();
//    readerOBJLowBody->SetFileName("LowBodyModule.obj");
//    readerOBJLowBody->Update();
//    vtkSmartPointer<vtkOBJReader> readerOBJMainBody = vtkSmartPointer<vtkOBJReader>::New();
//    readerOBJMainBody->SetFileName("MainBodyModule.obj");
//    readerOBJMainBody->Update();
//    vtkSmartPointer<vtkOBJReader> readerOBJCeph = vtkSmartPointer<vtkOBJReader>::New();
//    readerOBJCeph->SetFileName("CephModule.obj");
//    readerOBJCeph->Update();
//    vtkSmartPointer<vtkOBJReader> readerOBJPano = vtkSmartPointer<vtkOBJReader>::New();
//    readerOBJPano->SetFileName("PanoModule.obj");
//    readerOBJPano->Update();

//      vtkSmartPointer<vtkImporter> importObjAll = vtkSmartPointer<vtkImporter>::New();
    vtkSmartPointer<vtkNamedColors> colorsAll = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorAll = colorsAll->GetColor3d("Black");
        vtkColor3d actorColorAll = colorsAll->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsMain = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorMain = colorsMain->GetColor3d("Black");
        vtkColor3d actorColorMain = colorsMain->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsSub = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorSub = colorsSub->GetColor3d("Black");
        vtkColor3d actorColorSub = colorsSub->GetColor3d("HoneyDew");


    vtkSmartPointer<vtkPointData> pointDataAll = readerObjAll->GetOutput()->GetPointData();
    vtkSmartPointer<vtkPointData> pointDataMain = readerObjMain->GetOutput()->GetPointData();
    vtkSmartPointer<vtkPointData> pointDataSub = readerObjSub->GetOutput()->GetPointData();

//    vtkSmartPointer<vtkPointData> pointDataLowBody = readerOBJLowBody->GetOutput()->GetPointData();
//    vtkSmartPointer<vtkPointData> pointDataMainBody = readerOBJMainBody->GetOutput()->GetPointData();
//    vtkSmartPointer<vtkPointData> pointDataPano = readerOBJPano->GetOutput()->GetPointData();
//    vtkSmartPointer<vtkPointData> pointDataCeph = readerOBJCeph->GetOutput()->GetPointData();

    vtkSmartPointer<vtkPolyDataNormals> normalsAll = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsAll->SetInputData(readerObjAll->GetOutput());
//    normalsAll->SetInputData(readerOBJLowBody->GetOutput());
//    normalsAll->SetInputData(readerOBJMainBody->GetOutput());
//    normalsAll->SetInputData(readerOBJPano->GetOutput());
//    normalsAll->SetInputData(readerOBJCeph->GetOutput());
    normalsAll->ComputePointNormalsOn();
    normalsAll->ComputeCellNormalsOn();
    normalsAll->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalsMain = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsMain->SetInputData(readerObjMain->GetOutput());
//    normalsMain->SetInputData(readerOBJLowBody->GetOutput());
//    normalsMain->SetInputData(readerOBJMainBody->GetOutput());
//    normalsMain->SetInputData(readerOBJPano->GetOutput());
//    normalsMain->SetInputData(readerOBJCeph->GetOutput());
    normalsMain->ComputePointNormalsOn();
    normalsMain->ComputeCellNormalsOn();
    normalsMain->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalsSub = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsSub->SetInputData(readerObjSub->GetOutput());
//    normalsSub->SetInputData(readerOBJLowBody->GetOutput());
//    normalsSub->SetInputData(readerOBJMainBody->GetOutput());
//    normalsSub->SetInputData(readerOBJPano->GetOutput());
//    normalsSub->SetInputData(readerOBJCeph->GetOutput());
    normalsSub->ComputePointNormalsOn();
    normalsSub->ComputeCellNormalsOn();
    normalsSub->Update();

    /* Create Mapper & Actor */
    vtkSmartPointer<vtkPolyDataMapper> mapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperAll->SetInputConnection(readerObjAll->GetOutputPort());
//    mapperAll->SetInputData(readerOBJLowBody->GetOutput());
//    mapperAll->SetInputData(readerOBJMainBody->GetOutput());
//    mapperAll->SetInputData(readerOBJPano->GetOutput());
//    mapperAll->SetInputData(readerOBJCeph->GetOutput());
    vtkSmartPointer<vtkPolyDataMapper> mapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperMain->SetInputConnection(readerObjMain->GetOutputPort());
//    mapperMain->SetInputData(readerOBJLowBody->GetOutput());
//    mapperMain->SetInputData(readerOBJMainBody->GetOutput());
//    mapperMain->SetInputData(readerOBJPano->GetOutput());
//    mapperMain->SetInputData(readerOBJCeph->GetOutput());
    vtkSmartPointer<vtkPolyDataMapper> mapperSub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperSub->SetInputConnection(readerObjSub->GetOutputPort());
//    mapperSub->SetInputData(readerOBJLowBody->GetOutput());
//    mapperSub->SetInputData(readerOBJMainBody->GetOutput());
//    mapperSub->SetInputData(readerOBJPano->GetOutput());
//    mapperSub->SetInputData(readerOBJCeph->GetOutput());

    vtkSmartPointer<vtkActor> actorAll = vtkSmartPointer<vtkActor>::New();
    actorAll->SetMapper(mapperAll);
    //    actorAll->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("LightGray").GetData());
    actorAll->GetProperty()->SetSpecular(5.0);
    actorAll->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorMain = vtkSmartPointer<vtkActor>::New();
    actorMain->SetMapper(mapperMain);
    //    actorMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("LightGray").GetData());
    actorMain->GetProperty()->SetSpecular(5.0);
    actorMain->GetProperty()->SetSpecularPower(5.0);
    vtkSmartPointer<vtkActor> actorSub = vtkSmartPointer<vtkActor>::New();
    actorSub->SetMapper(mapperSub);
    //    actorSub->GetProperty()->SetDiffuseColor(colorsSub->GetColor3d("Red").GetData());
    actorSub->GetProperty()->SetSpecular(5.0);
    actorSub->GetProperty()->SetSpecularPower(5.0);

    /* Visualize with Cam */
    vtkSmartPointer<vtkRenderer> rendererAll = vtkSmartPointer<vtkRenderer>::New();
    rendererAll->AddActor(actorAll);
    rendererAll->SetBackground(backgroundColorAll.GetData());
    rendererAll->ResetCamera();
    rendererAll->LightFollowCameraOn();
    rendererAll->GetActiveCamera()->Azimuth(320);
    rendererAll->GetActiveCamera()->Elevation(20);
    rendererAll->GetActiveCamera()->Zoom(1.6);
    rendererAll->ResetCameraClippingRange();
    vtkSmartPointer<vtkRenderer> rendererMain = vtkSmartPointer<vtkRenderer>::New();
    rendererMain->AddActor(actorMain);
    rendererMain->SetBackground(backgroundColorAll.GetData());
    rendererMain->ResetCamera();
    rendererMain->LightFollowCameraOn();
    rendererMain->GetActiveCamera()->Azimuth(60);
    rendererMain->GetActiveCamera()->Elevation(40);
    rendererMain->GetActiveCamera()->Zoom(2.5);
    rendererMain->ResetCameraClippingRange();
    vtkSmartPointer<vtkRenderer> rendererSub = vtkSmartPointer<vtkRenderer>::New();
    rendererSub->AddActor(actorSub);
    rendererSub->SetBackground(backgroundColorAll.GetData());
    rendererSub->ResetCamera();
    rendererSub->LightFollowCameraOn();\
    rendererSub->GetActiveCamera()->Azimuth(240);
    rendererSub->GetActiveCamera()->Elevation(15);
    rendererSub->GetActiveCamera()->Zoom(2.2);
    rendererSub->ResetCameraClippingRange();

    vtkSmartPointer<vtkTransform> transformAll = vtkSmartPointer<vtkTransform>::New();
    transformAll->Translate(0.0, -350.0, 10.0);
    transformAll->RotateWXYZ(5, 0.0, 1.0, 0.0);
    actorAll->SetUserTransform(transformAll);

    vtkSmartPointer<vtkTransform> transformMain = vtkSmartPointer<vtkTransform>::New();
    transformMain->Translate(0.0, -200.0, 100.0);
    transformMain->RotateWXYZ(10, 0.0, 1.0, 1.0);
    actorMain->SetUserTransform(transformMain);

    vtkSmartPointer<vtkTransform> transformSub = vtkSmartPointer<vtkTransform>::New();
    transformSub->Translate(100.0, -100.0, -300.0);
    transformSub->RotateWXYZ(10, 5.0, 10.0, 5.0);
    actorSub->SetUserTransform(transformSub);

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterAll = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilterAll->SetInputConnection(readerObjAll->GetOutputPort());
//    transformFilterAll->SetInputConnection(readerOBJLowBody->GetOutputPort());
//    transformFilterAll->SetInputConnection(readerOBJMainBody->GetOutputPort());
//    transformFilterAll->SetInputConnection(readerOBJPano->GetOutputPort());
//    transformFilterAll->SetInputConnection(readerOBJCeph->GetOutputPort());

    transformFilterAll->SetTransform(transformAll);
    transformFilterAll->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterMain = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilterMain->SetInputConnection(readerObjMain->GetOutputPort());
//    transformFilterMain->SetInputConnection(readerOBJLowBody->GetOutputPort());
//    transformFilterMain->SetInputConnection(readerOBJMainBody->GetOutputPort());
//    transformFilterMain->SetInputConnection(readerOBJPano->GetOutputPort());
//    transformFilterMain->SetInputConnection(readerOBJCeph->GetOutputPort());
    transformFilterMain->SetTransform(transformMain);
    transformFilterMain->Update();

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilterSub = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilterSub->SetInputConnection(readerObjSub->GetOutputPort());
//    transformFilterSub->SetInputConnection(readerOBJLowBody->GetOutputPort());
//    transformFilterSub->SetInputConnection(readerOBJMainBody->GetOutputPort());
//    transformFilterSub->SetInputConnection(readerOBJPano->GetOutputPort());
//    transformFilterSub->SetInputConnection(readerOBJCeph->GetOutputPort());
    transformFilterSub->SetTransform(transformSub);
    transformFilterSub->Update();


    /* Rendering */
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowAll = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowAll->AddRenderer(rendererAll);
//    renderWindowAll->Render();
    renderWindowAll->SetWindowName("readAll");
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowMain = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowMain->AddRenderer(rendererMain);
//    renderWindowMain->Render();
    renderWindowMain->SetWindowName("readMain");
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowSub = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowSub->AddRenderer(rendererSub);
//    renderWindowSub->Render();
    renderWindowSub->SetWindowName("readSub");


    ui->openGLWidget_All->setRenderWindow(renderWindowAll);
    ui->openGLWidget_Main->setRenderWindow(renderWindowMain);
    ui->openGLWidget_Sub->setRenderWindow(renderWindowSub);

    ui->openGLWidget_All->interactor()->SetRenderWindow(renderWindowAll);
    ui->openGLWidget_Main->interactor()->SetRenderWindow(renderWindowMain);
    ui->openGLWidget_Sub->interactor()->SetRenderWindow(renderWindowSub);

    ui->openGLWidget_All->interactor()->ProcessEvents();
    ui->openGLWidget_Main->interactor()->ProcessEvents();
    ui->openGLWidget_Sub->interactor()->ProcessEvents();

    ui->openGLWidget_All->interactor()->Start();
    ui->openGLWidget_Main->interactor()->Start();
    ui->openGLWidget_Sub->interactor()->Start();

    connect(ui->MainPushButton, SIGNAL(clicked), this, SLOT(on_MainPushButton_Clicked()));
    connect(ui->SubPushButton, SIGNAL(clicked), this, SLOT(on_SubPushButton_Clicked()));
//    connect(ui->CaptureStartPushButton, SIGNAL(clicked), this, SLOT(on_CaptureStartPushButton_clicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void rotateMain()
{

}


void MainWindow::on_MainPushButton_clicked()
{

    QString panoPath = "C:/Qt_VTK_CT/build/Debug/Pano Frame(1152x64)/0001.raw";
    QFile panoFile(panoPath);
    if (!panoFile.open(QFile::ReadOnly))
        return;
    QByteArray ba = panoFile.readAll();
    const uchar* data = (const uchar*) ba.constData();;
    panoFile.close();
    //QImage image(data, 30, 30, QImage::Format_RGB32);
    QImage* panoImage = new QImage(data, 100, 470, QImage::Format_RGB555);
    QImage pano_Image(*panoImage);
    QPixmap panoPix;
    panoPix = QPixmap::fromImage(pano_Image,Qt::AutoColor);
    ui->PanoLabel->setPixmap(panoPix);

//    ui->PanoGraphicsView->setPixmap(pix);
}
void MainWindow::on_SubPushButton_clicked()
{
    QString cephPath = "C:/Qt_VTK_CT/build/Debug/Ceph_Frame(48x2400)/0001.raw";
    QFile cephFile(cephPath);
    if (!cephFile.open(QFile::ReadOnly))
        return;
    QByteArray ba = cephFile.readAll();
    const uchar* data = (const uchar*) ba.constData();;
    cephFile.close();
    //QImage image(data, 30, 30, QImage::Format_RGB32);
    QImage* cephImage = new QImage(data, 100, 470, QImage::Format_RGB555);
    QImage ceph_Image(*cephImage);
    QPixmap cephPix;
    cephPix = QPixmap::fromImage(ceph_Image,Qt::AutoColor);
    ui->CephLabel->setPixmap(cephPix);
}

//void MainWindow::on_CaptureStartPushButton_clicked()
//{
//    vtkSmartPointer<vtkImageReader2Factory> panoImageFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
//    std::vector<vtkSmartPointer<vtkImageReader2>> panoImageReaders;
//    int numImages = 2;
//    for(int i = 0; i < numImages; i++)
//    {
//        std::string fileName = "./images/pano frame/000" + std::to_string(i) + ".raw";
//        vtkSmartPointer<vtkImageReader2> panoImageReader =
//                panoImageFactory->CreateImageReader2(fileName.c_str());
//        panoImageReaders.push_back(panoImageReader);
//    }
//    std::vector<vtkSmartPointer<vtkImageViewer2>>panoImageViewers;
//    for(int i = 0; i < numImages; i++)
//    {
//        vtkSmartPointer<vtkImageViewer2> panoImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
//        panoImageViewer->SetInputConnection(panoImageReaders[i]->GetOutputPort());
//        panoImageViewers.push_back(panoImageViewer);

//        panoImageViewers[i]->Render();
//        //        ui->CapturedGraphicsView->render(panoImageViewers[i]);
//    }

//}

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

