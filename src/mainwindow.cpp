#include "mainwindow.h"
#include "ui_mainwindow.h"

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


    //    vtkSmartPointer<vtkImporter> importObjAll = vtkSmartPointer<vtkImporter>::New();
    vtkSmartPointer<vtkNamedColors> colorsAll = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorAll = colorsAll->GetColor3d("Black");
    //    vtkColor3d actorColorAll = colorsAll->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsMain = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorMain = colorsMain->GetColor3d("Black");
    //    vtkColor3d actorColorMain = colorsMain->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsSub = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorSub = colorsSub->GetColor3d("Black");
    //    vtkColor3d actorColorSub = colorsSub->GetColor3d("HoneyDew");


    vtkSmartPointer<vtkPointData> pointDataAll = readerObjAll->GetOutput()->GetPointData();
    vtkSmartPointer<vtkPointData> pointDataMain = readerObjMain->GetOutput()->GetPointData();
    vtkSmartPointer<vtkPointData> pointDataSub = readerObjSub->GetOutput()->GetPointData();

    vtkSmartPointer<vtkPolyDataNormals> normalsAll = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsAll->SetInputData(readerObjAll->GetOutput());
    normalsAll->ComputePointNormalsOn();
    normalsAll->ComputeCellNormalsOn();
    normalsAll->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalsMain = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsMain->SetInputData(readerObjMain->GetOutput());
    normalsMain->ComputePointNormalsOn();
    normalsMain->ComputeCellNormalsOn();
    normalsMain->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalsSub = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalsSub->SetInputData(readerObjSub->GetOutput());
    normalsSub->ComputePointNormalsOn();
    normalsSub->ComputeCellNormalsOn();
    normalsSub->Update();

    /* Create Mapper & Actor */
    vtkSmartPointer<vtkPolyDataMapper> mapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperAll->SetInputConnection(readerObjAll->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> mapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperMain->SetInputConnection(readerObjMain->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> mapperSub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperSub->SetInputConnection(readerObjSub->GetOutputPort());

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

    connect(ui->MainPushButton, SIGNAL(clicked), this, SLOT(MainPushButton_Clicked));
    connect(ui->SubPushButton, SIGNAL(clicked), this, SLOT(SubPushButton_Clicked));
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
    vtkSmartPointer<vtkImageReader2Factory> panoImageFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
    std::vector<vtkSmartPointer<vtkImageReader2>> panoImageReaders;
    int numImages = 2;
    for(int i = 0; i < numImages; i++)
    {
        std::string fileName = "./images/pano frame/000" + std::to_string(i) + ".raw";
        vtkSmartPointer<vtkImageReader2> panoImageReader =
                panoImageFactory->CreateImageReader2(fileName.c_str());
        panoImageReaders.push_back(panoImageReader);
    }
    std::vector<vtkSmartPointer<vtkImageViewer2>>panoImageViewers;
    for(int i = 0; i < numImages; i++)
    {
        vtkSmartPointer<vtkImageViewer2> panoImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
        panoImageViewer->SetInputConnection(panoImageReaders[i]->GetOutputPort());
        panoImageViewers.push_back(panoImageViewer);

        panoImageViewers[i]->Render();
        //        ui->CapturedGraphicsView->render(panoImageViewers[i]);
    }

}
void loadObj()
{

}

