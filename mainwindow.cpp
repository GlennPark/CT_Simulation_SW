#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkOBJReader.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInterActor.h>
#include <vtkSmartPointer.h>
#include <QVTKOpenGLStereoWidget.h>
#include <QVTKRenderWidget.h>
#include <QVTKOpenGLWindow.h>
#include <vtkCamera.h>
#include <vtkNew.h>
#include <vtkNamedColors.h>
#include <QOpenGLWidget.h>

#include <vtkOpenGLRenderWindow.h>
#include <vtkRenderWindow.h>
#include <InteractionContext.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    vtkSmartPointer<vtkNamedColors> colorsAll = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorAll = colorsAll->GetColor3d("SpringGreen");
    vtkColor3d actorColorAll = colorsAll->GetColor3d("HoneyDew");

    vtkSmartPointer<vtkNamedColors> colorsMain = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorMain = colorsMain->GetColor3d("SpringGreen");
    vtkColor3d actorColorMain = colorsMain->GetColor3d("HoneyDew");

    vtkSmartPointer<vtkNamedColors> colorsSub = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d backgroundColorSub = colorsSub->GetColor3d("SpringGreen");
    vtkColor3d actorColorSub = colorsSub->GetColor3d("HoneyDew");

    vtkSmartPointer<vtkOBJReader> readerAll = vtkSmartPointer<vtkOBJReader>::New();
    readerAll->SetFileName("IronMan.obj");
    readerAll->Update();
    vtkSmartPointer<vtkOBJReader> readerMain = vtkSmartPointer<vtkOBJReader>::New();
    readerMain->SetFileName("IronMan.obj");
    readerMain->Update();
    vtkSmartPointer<vtkOBJReader> readerSub = vtkSmartPointer<vtkOBJReader>::New();
    readerSub->SetFileName("IronMan.obj");
    readerSub->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperAll->SetInputConnection(readerAll->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> mapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperMain->SetInputConnection(readerMain->GetOutputPort());
    vtkSmartPointer<vtkPolyDataMapper> mapperSub = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperSub->SetInputConnection(readerSub->GetOutputPort());

    vtkSmartPointer<vtkActor> actorAll = vtkSmartPointer<vtkActor>::New();
    actorAll->SetMapper(mapperAll);
    //actorAll->GetProperty()->setDiffuseColor(actorColorAll.GetData());

    vtkSmartPointer<vtkActor> actorMain = vtkSmartPointer<vtkActor>::New();
    actorMain->SetMapper(mapperMain);
    vtkSmartPointer<vtkActor> actorSub = vtkSmartPointer<vtkActor>::New();
    actorSub->SetMapper(mapperSub);

    vtkSmartPointer<vtkRenderer> rendererAll = vtkSmartPointer<vtkRenderer>::New();
    rendererAll->AddActor(actorAll);
    rendererAll->SetBackground(backgroundColorAll.GetData());
    rendererAll->ResetCamera();
    rendererAll->GetActiveCamera()->Azimuth(30);
    rendererAll->GetActiveCamera()->Elevation(30);
    rendererAll->ResetCameraClippingRange();

    vtkSmartPointer<vtkRenderer> rendererMain = vtkSmartPointer<vtkRenderer>::New();
    rendererMain->AddActor(actorMain);
    rendererMain->SetBackground(backgroundColorAll.GetData());
    rendererMain->ResetCamera();
    rendererMain->GetActiveCamera()->Azimuth(50);
    rendererMain->GetActiveCamera()->Elevation(50);
    rendererMain->ResetCameraClippingRange();

    vtkSmartPointer<vtkRenderer> rendererSub = vtkSmartPointer<vtkRenderer>::New();
    rendererSub->AddActor(actorSub);
    rendererSub->SetBackground(backgroundColorAll.GetData());
    rendererSub->ResetCamera();
    rendererSub->GetActiveCamera()->Azimuth(10);
    rendererSub->GetActiveCamera()->Elevation(10);
    rendererSub->ResetCameraClippingRange();

    //vtkSmartPointer<vtkSetRenderWindow> setRenderWindowAll = vtkSmartPointer<vtkSetRenderWindow>
    vtkSmartPointer<vtkRenderWindow> renderWindowAll = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindowAll->AddRenderer(rendererAll);
  //  renderWindowAll->SetWindowName("readAll");
    vtkSmartPointer<vtkRenderWindow> renderWindowMain = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindowMain->AddRenderer(rendererMain);
  //  renderWindowMain->SetWindowName("readMain");
    vtkSmartPointer<vtkRenderWindow> renderWindowSub = vtkSmartPointer<vtkRenderWindow>::New();
   renderWindowSub->AddRenderer(rendererSub);
  //  renderWindowSub->SetWindowName("readSub");
 //   vtkSmartPointer<vtkRenderWindowInteractor> interactorAll = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //interactorAll->SetRenderWindow(renderWindowAll);
    //renderWindowAll->SetSize(300, 300);
//   interactorAll->Start();
    //vtkSmartPointer<vtkRenderWindowInteractor> interactorMain = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //interactorMain->SetRenderWindow(renderWindowMain);
    //renderWindowMain->SetSize(250, 250);
    
    //interactorMain->Start();
    //vtkSmartPointer<vtkRenderWindowInteractor> interactorSub = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //interactorSub->SetRenderWindow(renderWindowSub);
    //renderWindowSub->SetSize(200, 200);
    
    //interactorSub->Start();
    renderWindowAll->Render();
    renderWindowMain->Render();
    renderWindowSub->Render();

    ui->openGLWidget_All->setRenderWindow(renderWindowAll);
    ui->openGLWidget_Main->interactor()->SetRenderWindow(renderWindowMain);
    ui->openGLWidget_Sub->interactor()->SetRenderWindow(renderWindowSub);

//    ui->openGLWidget_All->interactor()->ProcessEvents();
    ui->openGLWidget_Main->interactor()->ProcessEvents();
    ui->openGLWidget_Sub->interactor()->ProcessEvents();

//    ui->openGLWidget_All->interactor()->Start();
    ui->openGLWidget_Main->interactor()->Start();
    ui->openGLWidget_Sub->interactor()->Start();
}   

MainWindow::~MainWindow()
{
    delete ui;
}
