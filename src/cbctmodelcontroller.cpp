#include "cbctmodelcontroller.h"
#include "QMap"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOBJReader.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkProperty.h"
#include "vtkNamedColors.h"
#include "vtkRenderer.h"
#include "vtkTransform.h"
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkCylinderSource.h"
#include "vtkRendererCollection.h"
#include <vtkQuaternion.h>
#include <vtkMatrix4x4.h>
#include <vtkSphereSource.h>
#include <QThread>


#include "ui_mainwindow.h"
#include <qdebug.h>

#define USE_DISPLAY_GLOBALAXIS



class CBCTModelController::VTKInternal {
public:
    Ui::MainWindow* m_mainwindowUi = nullptr;
    CBCTModelController* m_modelController= nullptr;
    CBCTRawImageViewer* m_rawImageViewer = nullptr;
    // Member Variable Concealment.
private:
    QMap<QString, vtkSmartPointer<vtkPolyData>> m_dataMap;

    std::vector<vtkSmartPointer<vtkPolyData>> m_vecdata;
    std::vector<vtkSmartPointer<vtkMapper>> m_mapper_All;
    std::vector<vtkSmartPointer<vtkMapper>> m_mapper_Main;
    std::vector<vtkSmartPointer<vtkMapper>> m_mapper_Sub;

    std::vector<vtkSmartPointer<vtkActor>> m_actor_All;
    std::vector<vtkSmartPointer<vtkActor>> m_actor_Main;
    std::vector<vtkSmartPointer<vtkActor>> m_actor_Sub;

    std::vector< vtkSmartPointer<vtkGenericOpenGLRenderWindow>> m_renderwindow;
    int m_curPositionX = 0;
    int m_curPositionY = 0; // current Y Position Value
    int m_curPositionZ = 0; // cuttent Z Position Value

    double m_curAngle = 0;
    double m_PanoCenter[3];


public:
    VTKInternal(CBCTModelController* modelController)
        : m_modelController(modelController)
    {

    }

    // CBCTModelController Reset method.
    bool internal_Initialize() {
        ///* Load Source */
        QStringList FilePath;
        FilePath << "lowerbody.obj";
        FilePath << "upperbody.obj";
        FilePath << "panomodule.obj";
        FilePath << "cephmodule.obj";

        QStringList MtlFilePath;
        MtlFilePath << "lowerbody.mtl";
        MtlFilePath << "upperbody.mtl";
        MtlFilePath << "panomodule.mtl";
        MtlFilePath << "cephmodule.mtl";

        /*
        file:  "lowerbody.obj" Center : 294.314 767.177 - 473.419
        file : "upperbody.obj" Center : -66.1935 1499.9 - 24.4806
        file : "panomodule.obj" Center : 644.717 1888.39 328.713
        file : "cephmodule.obj" Center : -552.515 1682.58 - 205.5
        */

        _load_objfile(FilePath, MtlFilePath, m_vecdata);

        ///* Create Mapper & Actor */
        _create_mapper(m_vecdata, m_mapper_All);
        _create_mapper(m_vecdata, m_mapper_Main);
        _create_mapper(m_vecdata, m_mapper_Sub);

        _create_actor(m_mapper_All, "All", m_actor_All);
        _create_actor(m_mapper_Main, "Main", m_actor_Main);
        _create_actor(m_mapper_Sub, "Sub", m_actor_Sub);

        m_renderwindow.push_back(_create_render(m_actor_All));
        m_renderwindow.push_back(_create_render(m_actor_Main));
        m_renderwindow.push_back(_create_render(m_actor_Sub));

        _register_window();

        _update_render();


        auto cen = m_actor_Main[2]->GetCenter();

        m_PanoCenter[0] = cen[0];
        m_PanoCenter[1] = cen[1];
        m_PanoCenter[2] = cen[2];
        return true;
    }

    /* 상승 버튼 클릭시 Y 좌표 +, 높이 제한 있음 */
    void internal_Ascending_Function() {
        if (m_curPositionY >= 0)
        {
            m_curPositionY = 0;
            qDebug() << QString::fromLocal8Bit("Y Axis Maximum value");
            return;
        }
        else {
            m_curPositionY += 10;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, 0);

            // All
            m_actor_All[1]->SetUserTransform(transform);
            m_actor_All[2]->SetUserTransform(transform);
            m_actor_All[3]->SetUserTransform(transform);

            // Main
            m_actor_Main[1]->SetUserTransform(transform);
            m_actor_Main[2]->SetUserTransform(transform);
            m_actor_Main[3]->SetUserTransform(transform);

            // Sub
            m_actor_Sub[1]->SetUserTransform(transform);
            m_actor_Sub[2]->SetUserTransform(transform);
            m_actor_Sub[3]->SetUserTransform(transform);

            transform->Update();

            _update_render();
        }

    }

    /* 하강 버튼 클릭시 Y 좌표 -, 높이 제한 있음 */
    void internal_Descending_Function() {
        if (m_curPositionY < -580)
        {
            m_curPositionY = -580;
            qDebug() << "Y Axis Minimum value";
            return;
        }
        else {
            m_curPositionY -= 10;
            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, 0);

            // All
            m_actor_All[1]->SetUserTransform(transform);
            m_actor_All[2]->SetUserTransform(transform);
            m_actor_All[3]->SetUserTransform(transform);

            // Main
            m_actor_Main[1]->SetUserTransform(transform);
            m_actor_Main[2]->SetUserTransform(transform);
            m_actor_Main[3]->SetUserTransform(transform);

            // Sub
            m_actor_Sub[1]->SetUserTransform(transform);
            m_actor_Sub[2]->SetUserTransform(transform);
            m_actor_Sub[3]->SetUserTransform(transform);

            transform->Update();

            _update_render();
            qDebug() << m_curPositionY;
        }
    }

    /* Main 버튼 클릭시 PanoModule 회전 및 원위치 */
    void internal_Panorama_Module_Function() {

        for (int i = 0; i <= 360; i++)
        {
            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

            transform->PostMultiply();
            transform->Translate(-m_PanoCenter[0], -m_PanoCenter[1], -m_PanoCenter[2]);
            transform->RotateY(i);
            transform->Translate(m_PanoCenter);
            transform->Translate(0, m_curPositionY, 0);
            m_actor_Main[2]->SetUserTransform(transform);
            transform->Update();
            m_renderwindow[1]->Render();
            m_mainwindowUi->openGLWidget_Main->update();
            qDebug() << i;
            _update_render();
        }
    }

    /* Sub 버튼 클릭시 CephModule 앞뒤 움직임 및 원위치 */
    void internal_Cephalo_Module_Function()
    {
        // max 260;
        // min 270;
        for (int i = 0; i < 52; i++)
        {
            m_curPositionZ = m_curPositionZ + 5;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, m_curPositionZ);

            // Sub
            m_actor_Sub[3]->SetUserTransform(transform);

            transform->Update();
            qDebug() << "position Z : " << m_curPositionZ;
            _update_render();
        }
        for (int i = 0; i < 130; i++)
        {
            m_curPositionZ = m_curPositionZ - 2;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, m_curPositionZ);

            // Sub


            m_actor_Sub[3]->SetUserTransform(transform);

            transform->Update();
            qDebug() << "position Z : " << m_curPositionZ;
            _update_render();
        }
        for (int i = 0; i < 130; i++)
        {
            m_curPositionZ = m_curPositionZ - 2;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, m_curPositionZ);

            // Sub


            m_actor_Sub[3]->SetUserTransform(transform);

            transform->Update();
            qDebug() << "position Z : " << m_curPositionZ;
            _update_render();
        }
        for (int i = 0; i < 52; i++)
        {
            m_curPositionZ = m_curPositionZ + 5;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, m_curPositionZ);

            // Sub

            m_actor_Sub[3]->SetUserTransform(transform);

            transform->Update();
            qDebug() << "position Z : " << m_curPositionZ;
            _update_render();
        }
    }

    void internal_Reset_Function()
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        m_curPositionX = 0;
        m_curPositionY = 0;
        m_curPositionZ = 0;
        transform->Translate(m_curPositionX, m_curPositionY, m_curPositionZ);


        // All
        m_actor_All[1]->SetUserTransform(transform);
        m_actor_All[2]->SetUserTransform(transform);
        m_actor_All[3]->SetUserTransform(transform);

        // Main
        m_actor_Main[1]->SetUserTransform(transform);
        m_actor_Main[2]->SetUserTransform(transform);
        m_actor_Main[3]->SetUserTransform(transform);

        // Sub
        m_actor_Sub[1]->SetUserTransform(transform);
        m_actor_Sub[2]->SetUserTransform(transform);
        m_actor_Sub[3]->SetUserTransform(transform);

        transform->Update();

        _update_render();

    }

    void internal_Ready_Function()
    {

    }

    void internal_Start_Function()
    {
        if (m_mainwindowUi->PanoCheckBox->isChecked())
        {
            if (m_mainwindowUi->CephCheckBox->isChecked())
            {

m_panoErrorMessage:ERROR_LOG_POLICY_CONFLICT;
            }
            else
            {
                qDebug() << __FUNCTION__;
                m_rawImageViewer->startPanoTimer();
                internal_Panorama_Module_Function();
            }

            //CBCTRawImageViewer m_rawImageViewer;
            //QPixmap panoPix = m_rawImageViewer.PanoImageViewer();
            //ui->PanoLabel->setPixmap(panoPix);
        }

        if (m_mainwindowUi->CephCheckBox->isChecked())
        {
            if (m_mainwindowUi->PanoCheckBox->isChecked())
            {

m_cephErrorMessage:ERROR_LOG_POLICY_CONFLICT;
            }
            else
            {
                qDebug() << __FUNCTION__;
                m_rawImageViewer->stopCephTimer();
                internal_Cephalo_Module_Function();
            }
        }
    }
    void internal_Stop_Function()
    {

    }


    // Internal Methods must be used in the VTKInternal.
private:
    void _load_objfile(QStringList paths, QStringList mtls, std::vector<vtkSmartPointer<vtkPolyData>>& objs)
    {
        for (int i = 0; i < paths.size(); i++)
        {
            vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
            reader->SetFileName(paths[i].toStdString().c_str());
            //reader->SetFileNameMTL(mtls[i].toStdString().c_str());
            reader->Update();

            objs.push_back(reader->GetOutput());
        }

#if USE_VTKOBJIMPORT
        for (int i = 0; i < paths.size(); i++)
        {
            vtkSmartPointer<vtkOBJImporter> reader = vtkSmartPointer<vtkOBJImporter>::New();
            reader->SetFileName(paths[i].toStdString().c_str());
            reader->SetFileNameMTL(mtls[i].toStdString().c_str());
            reader->Update();

            vtkNew<vtkNamedColors> colors;

            vtkNew<vtkRenderer> renderer;
            vtkNew<vtkRenderWindow> renWin;
            vtkNew<vtkRenderWindowInteractor> iren;

            renderer->SetBackground2(colors->GetColor3d("Silver").GetData());
            renderer->SetBackground(colors->GetColor3d("Gold").GetData());
            renderer->GradientBackgroundOn();
            renWin->AddRenderer(renderer);
            renderer->UseHiddenLineRemovalOn();
            renWin->AddRenderer(renderer);
            renWin->SetSize(640, 480);
            renWin->SetWindowName("OBJImporter");

            iren->SetRenderWindow(renWin);
            reader->SetRenderWindow(renWin);
            reader->Update();

            auto actors = renderer->GetActors();
            actors->InitTraversal();
            std::cout << "There are " << actors->GetNumberOfItems() << " actors"
                      << std::endl;

            for (vtkIdType a = 0; a < actors->GetNumberOfItems(); ++a)
            {
                std::cout << reader->GetOutputDescription(a) << std::endl;

                vtkActor* actor = actors->GetNextActor();

                // OBJImporter turns texture interpolation off
                if (actor->GetTexture())
                {
                    std::cout << "Has texture\n";
                    //actor->GetTexture()->InterpolateOn();
                }

                vtkPolyData* pd =
                        dynamic_cast<vtkPolyData*>(actor->GetMapper()->GetInput());

                vtkPolyDataMapper* mapper =
                        dynamic_cast<vtkPolyDataMapper*>(actor->GetMapper());
                mapper->SetInputData(pd);
                objs.push_back(pd);
            }



        }
#endif
    }

    void _create_mapper(const std::vector<vtkSmartPointer<vtkPolyData>>& objs, std::vector<vtkSmartPointer<vtkMapper>>& mapper)
    {
        for (auto data : objs)
        {
            vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
            map->SetInputData(data);
            map->Update();
            mapper.push_back(map);
        }
    }

    void _create_actor(const std::vector<vtkSmartPointer<vtkMapper>>& mappers, const QString& Color, std::vector<vtkSmartPointer<vtkActor>>& actors)
    {
        for (auto map : mappers)
        {
            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(map);
            actors.push_back(actor);
        }
    }

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> _create_render(const std::vector<vtkSmartPointer<vtkActor>>& Actors)
    {
        vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
        vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();

        for (auto actor : Actors)
            Renderer->AddActor(actor);

#ifdef USE_DISPLAY_GLOBALAXIS
        vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
        axes->SetTotalLength(700, 700, 700);
        auto cen = axes->GetCenter();
        qDebug() << "Axis Center : " << cen[0] << cen[1] << cen[2];
        Renderer->AddActor(axes);
#endif

        Renderer->SetBackground(colors->GetColor3d("Black").GetData());
        Renderer->ResetCamera();
        /*Renderer->LightFollowCameraOn();
        Renderer->ResetCameraClippingRange();*/
        Renderer->ResetCamera();
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        renderWindow->AddRenderer(Renderer);
        return renderWindow;
    }

    void _register_window()
    {
        m_mainwindowUi->openGLWidget_All->setRenderWindow(m_renderwindow[GeometryViewType::All]);
        m_mainwindowUi->openGLWidget_Main->setRenderWindow(m_renderwindow[GeometryViewType::Main]);
        m_mainwindowUi->openGLWidget_Sub->setRenderWindow(m_renderwindow[GeometryViewType::Sub]);

        m_mainwindowUi->openGLWidget_All->interactor()->ProcessEvents();
        m_mainwindowUi->openGLWidget_Main->interactor()->ProcessEvents();
        m_mainwindowUi->openGLWidget_Sub->interactor()->ProcessEvents();
    }

    void _update_render()
    {
        for (auto win : m_renderwindow)
            win->Render();
        QApplication::processEvents();
    }

};

CBCTModelController::CBCTModelController(Ui::MainWindow* mainwindowUi)
    : Internal(new VTKInternal(this))
{
    Internal->m_mainwindowUi = mainwindowUi;
}

CBCTModelController::~CBCTModelController()
{
}

bool CBCTModelController::initialize()
{
    return Internal->internal_Initialize();
}
void CBCTModelController::test()
{
}



void CBCTModelController::ascending_Function()
{
    Internal->internal_Ascending_Function();
}

void CBCTModelController::descending_Function()
{
    Internal->internal_Descending_Function();
}

void CBCTModelController::panorama_Module_Function()
{
    qDebug() << "Main Push Btn!!";
    Internal->internal_Panorama_Module_Function();
}

void CBCTModelController::cephalo_Module_Function()
{
    //Internal->myThread.start();
    qDebug() << "Sub Push Btn!!";
    Internal->internal_Cephalo_Module_Function();
}


/* Internal method 를 외부 클래스에서 호출하기 위해 담아주는 함수들 */

void CBCTModelController::reset_VTK_Function()
{
    Internal->internal_Reset_Function();
}

void CBCTModelController::ready_VTK_Fucntion()
{
    Internal->internal_Ready_Function();
}

/* 촬영 SW 에서 시그널 받았을때, modality 조건 포함 VTK motions 동작 */
void CBCTModelController::start_VTK_Function()
{
    Internal->internal_Start_Function();
}

void CBCTModelController::stop_VTK_Function()
{
    Internal->internal_Stop_Function();
}

void  CBCTModelController::pano_VTK_Function()
{
    Internal->internal_Panorama_Module_Function();
}
void  CBCTModelController::ceph_VTK_Function()
{
    Internal->internal_Cephalo_Module_Function();
}
