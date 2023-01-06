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
// #include <QVTKWidget.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    actorAll ->SetMapper(mapperAll);
    vtkSmartPointer<vtkActor> actorMain = vtkSmartPointer<vtkActor>::New();
    actorMain->SetMapper(mapperMain);
    vtkSmartPointer<vtkActor> actorSub = vtkSmartPointer<vtkActor>::New();
    actorSub->SetMapper(mapperSub);


    vtkSmartPointer<vtkRenderer> rendererAll = vtkSmartPointer<vtkRenderer>::New();
    rendererAll->AddActor(actorAll);
    vtkSmartPointer<vtkRenderer> rendererMain = vtkSmartPointer<vtkRenderer>::New();
    rendererMain->AddActor(actorMain);
    vtkSmartPointer<vtkRenderer> rendererSub = vtkSmartPointer<vtkRenderer>::New();
    rendererSub->AddActor(actorSub);
    
    vtkSmartPointer<vtkRenderWindow> renderWindowAll = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindowAll->AddRenderer(rendererAll);
    vtkSmartPointer<vtkRenderWindow> renderWindowMain = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindowMain->AddRenderer(rendererMain);
    vtkSmartPointer<vtkRenderWindow> renderWindowSub = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindowSub->AddRenderer(rendererSub);

    vtkSmartPointer<vtkRenderWindowInteractor> interactorAll = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactorAll->Start();
    vtkSmartPointer<vtkRenderWindowInteractor> interactorMain = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactorMain->Start();
    vtkSmartPointer<vtkRenderWindowInteractor> interactorSub = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactorSub->Start();


}

MainWindow::~MainWindow()
{
    delete ui;
}
