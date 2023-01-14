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

private slots:
    void on_MainPushButton_clicked();

    void on_SubPushButton_clicked();

    void on_CaptureStartPushButton_clicked();

    void PanoImageViewer();

    void CephImageViewer();
    //    void on_AscendingPushButton_pressed();

    //    void on_AscendingPushButton_released();

    //    void on_DescendingPushButton_pressed();

    //    void on_DescendingPushButton_released();

private:
    void loadObj();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
