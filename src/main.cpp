#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication CTApp(argc, argv);

   

    //vtkSmartPointer<QVTKOpenGLStereoWidget> StereoAll = vtkSmartPointer<
    //QVTKOpenGLStereoWidget<vtkRenderWindow>->AddRenderer(rendererAll);
    //
    //widget.show();
    
    MainWindow mainWindow;
    mainWindow.show();
    return CTApp.exec();
}
