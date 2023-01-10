#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow.h>
#include <QObject.h>

//class vtkObject;
//class vtkObjReader;
//class vtkActor;
//class vtkRenderer;
//class vtkRendererWindow;
//class vtkPolyDataMapper;
//class vtkRenderWindowInterActor;
//class vtkSmartPointer;
//class QVTKOpenGLStereoWidget;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loadObj();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
