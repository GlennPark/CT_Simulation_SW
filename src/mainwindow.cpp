#include "mainwindow.h"
#include "ui_mainwindow.h"

/* 3D .Obj Visualization */
#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>


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

    vtkSmartPointer<vtkOBJReader> readerAll = vtkSmartPointer<vtkOBJReader>::New();
    readerAll->SetFileName("CTObj.obj");
    readerAll->Update();
    vtkSmartPointer<vtkOBJReader> readerMain = vtkSmartPointer<vtkOBJReader>::New();
    readerMain->SetFileName("CTObj.obj");
    readerMain->Update();
    vtkSmartPointer<vtkOBJReader> readerSub = vtkSmartPointer<vtkOBJReader>::New();
    readerSub->SetFileName("CTObj.obj");
    readerSub->Update();

    vtkSmartPointer<vtkNamedColors> colorsAll = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorAll = colorsAll->GetColor3d("Black");
//    vtkColor3d actorColorAll = colorsAll->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsMain = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorMain = colorsMain->GetColor3d("Black");
//    vtkColor3d actorColorMain = colorsMain->GetColor3d("HoneyDew");
    vtkSmartPointer<vtkNamedColors> colorsSub = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorSub = colorsSub->GetColor3d("Black");
//    vtkColor3d actorColorSub = colorsSub->GetColor3d("HoneyDew");

    vtkSmartPointer<vtkPolyDataMapper> mapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperAll->SetInputConnection(readerAll->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> mapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperMain->SetInputConnection(readerMain->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> mapperSub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperSub->SetInputConnection(readerSub->GetOutputPort());


    vtkSmartPointer<vtkActor> actorAll = vtkSmartPointer<vtkActor>::New();
    actorAll->SetMapper(mapperAll);
    actorAll->GetProperty()->SetDiffuseColor(colorsMain->GetData());
    actorAll->GetProperty()->SetSpecular(5.0);
    actorAll->GetProperty()->SetSpecularPower(1.0);
    vtkSmartPointer<vtkActor> actorMain = vtkSmartPointer<vtkActor>::New();
    actorMain->SetMapper(mapperMain);
//    actorMain->GetProperty()->SetDiffuseColor(colorsMain->GetColor3d("LightGray").GetData());
    actorMain->GetProperty()->SetSpecular(5.0);
    actorMain->GetProperty()->SetSpecularPower(1.0);
    vtkSmartPointer<vtkActor> actorSub = vtkSmartPointer<vtkActor>::New();
    actorSub->SetMapper(mapperSub);
//    actorSub->GetProperty()->SetDiffuseColor(colorsSub->GetColor3d("Red").GetData());
    actorSub->GetProperty()->SetSpecular(5.0);
    actorSub->GetProperty()->SetSpecularPower(1.0);

    vtkSmartPointer<vtkRenderer> rendererAll = vtkSmartPointer<vtkRenderer>::New();
    rendererAll->AddActor(actorAll);
    rendererAll->SetBackground(backgroundColorAll.GetData());
    rendererAll->ResetCamera();
    rendererAll->GetActiveCamera()->Azimuth(320);
    rendererAll->GetActiveCamera()->Elevation(20);
    rendererAll->GetActiveCamera()->Zoom(1.2);
    rendererAll->ResetCameraClippingRange();
    vtkSmartPointer<vtkRenderer> rendererMain = vtkSmartPointer<vtkRenderer>::New();
    rendererMain->AddActor(actorMain);
    rendererMain->SetBackground(backgroundColorAll.GetData());
    rendererMain->ResetCamera();
    rendererMain->GetActiveCamera()->Azimuth(60);
    rendererMain->GetActiveCamera()->Elevation(40);
    rendererMain->GetActiveCamera()->Zoom(2.5);
    rendererMain->ResetCameraClippingRange();
    vtkSmartPointer<vtkRenderer> rendererSub = vtkSmartPointer<vtkRenderer>::New();
    rendererSub->AddActor(actorSub);
    rendererSub->SetBackground(backgroundColorAll.GetData());
    rendererSub->ResetCamera();
    rendererSub->GetActiveCamera()->Azimuth(240);
    rendererSub->GetActiveCamera()->Elevation(30);
    rendererSub->GetActiveCamera()->Zoom(2.2);
    rendererSub->ResetCameraClippingRange();


    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowAll = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowAll->AddRenderer(rendererAll);
    renderWindowAll->SetWindowName("readAll");
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowMain = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowMain->AddRenderer(rendererMain);
    renderWindowMain->SetWindowName("readMain");
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindowSub = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindowSub->AddRenderer(rendererSub);
    renderWindowSub->SetWindowName("readSub");

    rendererAll->AddActor(actorAll);
    rendererMain->AddActor(actorMain);
    rendererSub->AddActor(actorSub);

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

    vtkSmartPointer<vtkTransform> transformAll = vtkSmartPointer<vtkTransform>::New();
//    transformAll->Translate(5 / 50, 0, 100);
    actorAll->SetUserTransform(transformAll);

    vtkSmartPointer<vtkTransform> transformMain = vtkSmartPointer<vtkTransform>::New();
//   transformMain->RotateX(180);
    actorMain->SetUserTransform(transformMain);

    vtkSmartPointer<vtkTransform> transformSub = vtkSmartPointer<vtkTransform>::New();
// transformSub->Translate(5 / 50, 100, 0);
    actorSub->SetUserTransform(transformSub);

    renderWindowAll->Render();
    renderWindowMain->Render();
    renderWindowSub->Render();

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
        std::string fileName = "./images/pano frame/0000" + std::to_string(i) + ".raw";
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

