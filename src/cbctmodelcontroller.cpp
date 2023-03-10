#include "cbctmodelcontroller.h"
#include "QMap"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkOBJReader.h"
#include "vtkPLYReader.h"
#include "vtkOBJImporter.h"

#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkProperty.h"
#include "vtkNamedColors.h"
#include "vtkRenderer.h"
#include "vtkTransform.h"
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkLightActor.h"
#include "vtkLight.h"

#include "vtkRendererCollection.h"
#include <vtkQuaternion.h>
#include <vtkMatrix4x4.h>
#include <vtkSphereSource.h>
#include <QThread>
#include <QTime>
#include <QElapsedTimer>
#include <qevent.h>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include <QDebug>

#define USE_DISPLAY_GLOBALAXIS

/* 시작 후 10도까지는 가속, 180도까지 정상속도, 나머지 감속구간 후 멈춤, 총 200도 가량, 16초 */
/* ceph 촬영 시 끝단에서 시작, 한쪽으로 이동 후 복귀, Pano Module 이 45도 정도 기울어진 상태에서 세팔로 촬영이 시작되어야 한다 (시네빔)*/
/* 모션 시작 시 특정 좌표로 카메라 설정 */
/* 실제로는 모션과 네트워크는 분리되어 있음 */

enum dataType
{
    Lower = 0,
    Upper,
    Pano,
    Ceph,
    Xray,
};
enum viewType
{
    All = 0,
    Main,
    Sub,
};

class CBCTModelController::Internal {
public:
    Ui::MainWindow* m_parentUI = nullptr;
    CBCTModelController* m_Owner = nullptr;

    int m_MaxPanoImage = 0;
    int m_MaxCephImage = 0;

    double m_curPositionX = 0;
    int m_curPositionY = 0; // current Y Position Value
    double m_curPositionZ = 0; // 현재 Ceph Z Position 값

    double m_curPanoAngle = 0; // 현재 Pano Angle 값
    double m_curXRayAngle = 0; // 현재 Xray Angle 값

    double* m_PanoCenter;
    double* m_XrayCenter;

    bool isRunning_Pano = false;
    bool isRunning_Ceph = false;

    /* 환자 로딩 정보 확인 */
    bool isLoadPano = false;
    bool isLoadCeph = false;
    // GeometryDataType, vtkMapper
    struct MapperItem
    {
        QMap<QString, vtkSmartPointer<vtkPolyDataMapper>> m_mapper;
    };
    // GeometryDataType, vtkActor
    struct ActorItem
    {
        QMap<QString, vtkSmartPointer<vtkActor>> m_actor;
    };
    // Member Variable Concealment.
    vtkSmartPointer<vtkPolyData> m_patientPano;
    vtkSmartPointer<vtkPolyData> m_patientCeph;
private:

    // Pano Patient
    vtkSmartPointer<vtkPolyDataMapper> m_patientPanomapperAll;
    vtkSmartPointer<vtkPolyDataMapper> m_patientPanomapperMain;
    vtkSmartPointer<vtkActor> m_patientPanoactorAll;
    vtkSmartPointer<vtkActor> m_patientPanoactorMain;

    // Ceph Patient

    vtkSmartPointer<vtkPolyDataMapper> m_patientCephmapperAll;
    vtkSmartPointer<vtkPolyDataMapper> m_patientCephmapperSub;
    vtkSmartPointer<vtkActor> m_patientCephactorAll;
    vtkSmartPointer<vtkActor> m_patientCephactorSub;


    // GeometryDataType, vtkpolydata
    QMap <QString, vtkSmartPointer<vtkPolyData>> m_dataMap;
    // GeometryViewType , MapperItem
    QMap <QString, MapperItem> m_mapperMap;
    // GeometryViewType , ActorItem
    QMap <QString, ActorItem> m_actorMap;
    // GeometryViewType , vtkRenderer
    QMap <QString, vtkSmartPointer<vtkRenderer>> m_renderMap;
    // GeometryViewType , vtkGenericOpenGLRenderWindow
    QMap <QString, vtkSmartPointer<vtkGenericOpenGLRenderWindow>> m_windowMap;

public:
    Internal(CBCTModelController* owner)
        : m_Owner(owner)
    {

    }

    // CBCTModelController Reset method.
    bool _initialize() {

        ///* Load Source */
        _load_plyfile(GeometryDataType::toString(Lower), m_dataMap);
        _load_plyfile(GeometryDataType::toString(Upper), m_dataMap);
        _load_plyfile(GeometryDataType::toString(Pano), m_dataMap);
        _load_plyfile(GeometryDataType::toString(Ceph), m_dataMap);
        _load_plyfile(GeometryDataType::toString(Xray), m_dataMap);
        // qDebug() << "걸린시간 = " << dbgTime.elapsed();
        qDebug(" ply load ");

        ///* Create Mapper & Actor */
        _create_Mapper(GeometryViewType::toString(All), m_dataMap, m_mapperMap);
        _create_Mapper(GeometryViewType::toString(Main), m_dataMap, m_mapperMap);
        _create_Mapper(GeometryViewType::toString(Sub), m_dataMap, m_mapperMap);

        _create_actor(GeometryViewType::toString(All), m_mapperMap, m_actorMap);
        _create_actor(GeometryViewType::toString(Main), m_mapperMap, m_actorMap);
        _create_actor(GeometryViewType::toString(Sub), m_mapperMap, m_actorMap);

        ///* Create renderer & Window */
        _create_render(GeometryViewType::toString(All), m_actorMap, m_renderMap);
        _create_render(GeometryViewType::toString(Main), m_actorMap, m_renderMap);
        _create_render(GeometryViewType::toString(Sub), m_actorMap, m_renderMap);

        _create_window(GeometryViewType::toString(All), m_renderMap, m_windowMap);
        _create_window(GeometryViewType::toString(Main), m_renderMap, m_windowMap);
        _create_window(GeometryViewType::toString(Sub), m_renderMap, m_windowMap);

        // registart window
        _register_window(GeometryViewType::toString(All), m_windowMap);
        _register_window(GeometryViewType::toString(Main), m_windowMap);
        _register_window(GeometryViewType::toString(Sub), m_windowMap);

        _update_render();


        m_PanoCenter = m_actorMap.value(GeometryViewType::toString(Main)).m_actor.value(GeometryDataType::toString(Pano))->GetCenter();
        m_XrayCenter = m_actorMap.value(GeometryViewType::toString(Main)).m_actor.value(GeometryDataType::toString(Xray))->GetCenter();
        m_MaxPanoImage = m_parentUI->PanoProgressBar->maximum();
        m_MaxCephImage = m_parentUI->CephProgressBar->maximum();

        return true;
    }

    void _stop()
    {
        isRunning_Pano = false;
        isRunning_Ceph = false;
    }

    void _on_AscendingPushButton_pressed() {

        _on_reset_Panorama_module();
        // 아래 함수에는 y축 초기화가 포함

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
            _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(All))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(All))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(All))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(All))->SetUserTransform(transform);


            // Main
            _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(Main))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Main))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Main))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Main))->SetUserTransform(transform);

            // Sub
            _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(Sub))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Sub))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Sub))->SetUserTransform(transform);

            transform->Update();

            _update_render();
        }
    }

    void _on_DescendingPushButton_pressed() {

        _on_reset_Panorama_module();
        // 아래 함수에는 y축 리셋이 포함되어 있음

        if (isRunning_Pano || isRunning_Ceph)
            return;
        isRunning_Pano = true;
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
            _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(All))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(All))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(All))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(All))->SetUserTransform(transform);

            // Main
            _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(Main))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Main))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Main))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Main))->SetUserTransform(transform);

            // Sub
            _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(Sub))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Sub))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub))->SetUserTransform(transform);
            _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Sub))->SetUserTransform(transform);

            transform->Update();

            _update_render();
            qDebug() << m_curPositionY;
        }
        isRunning_Pano = false;
    }

    /* 파노라마 환자는 Sub RenderWindow 에 출력할 필요가 없으므로 데이터 세이브 */
    bool _Load_PanoPatient(const QString& path) {
        if(isLoadPano == true)
        {
            return false;
        }
        vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();

        // reader->SetFileName(path.toStdString().c_str());
        reader->SetFileName("./resources/Patient1.ply");
        reader->Update();

        m_patientPano = reader->GetOutput();

        m_patientPanomapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_patientPanomapperAll->SetInputData(m_patientPano);
        m_patientPanomapperAll->Update();
        m_patientPanomapperMain = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_patientPanomapperMain->SetInputData(m_patientPano);
        m_patientPanomapperMain->Update();

        m_patientPanoactorAll = vtkSmartPointer<vtkActor>::New();
        m_patientPanoactorAll->SetMapper(m_patientPanomapperAll);
        _get_render(GeometryViewType::toString(GeometryViewType::All))->AddActor(m_patientPanoactorAll);
        m_patientPanoactorMain = vtkSmartPointer<vtkActor>::New();
        m_patientPanoactorMain->SetMapper(m_patientPanomapperMain);
        _get_render(GeometryViewType::toString(GeometryViewType::Main))->AddActor(m_patientPanoactorMain);

        _update_render();
        isLoadPano = true;
        return true;
    }


    /* 세팔로 환자는 Main RenderWindow 에 출력할 필요가 없으므로 데이터 세이브 */
    bool _Load_CephPatient(const QString& path) {
        if(isLoadCeph == true)
        {
            return false;
        }
        vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
        //   reader->SetFileName(path.toStdString().c_str());
        reader->SetFileName("./resources/Patient2.ply");

        reader->Update();

        m_patientCeph = reader->GetOutput();

        m_patientCephmapperAll = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_patientCephmapperAll->SetInputData(m_patientCeph);
        m_patientCephmapperAll->Update();
        m_patientCephmapperSub = vtkSmartPointer<vtkPolyDataMapper>::New();
        m_patientCephmapperSub->SetInputData(m_patientCeph);
        m_patientCephmapperSub->Update();

        m_patientCephactorAll = vtkSmartPointer<vtkActor>::New();
        m_patientCephactorAll->SetMapper(m_patientCephmapperAll);
        _get_render(GeometryViewType::toString(GeometryViewType::All))->AddActor(m_patientCephactorAll);
        m_patientCephactorSub = vtkSmartPointer<vtkActor>::New();
        m_patientCephactorSub->SetMapper(m_patientCephmapperSub);
        _get_render(GeometryViewType::toString(GeometryViewType::Sub))->AddActor(m_patientCephactorSub);

        _update_render();
        isLoadCeph = true;
        return true;
    }

    /* All, Main Window 에 출력된 환자 오브젝트 제거 */
    bool _Remove_PanoPatient() {
        if(isLoadPano != true)
             return false;
            //   if (m_patientPanoactorAll == nullptr && m_patientPanoactorMain == nullptr)

        qDebug() <<"null check panop";
        _get_render(GeometryViewType::toString(GeometryViewType::All))->RemoveActor(m_patientPanoactorAll);
        _get_render(GeometryViewType::toString(GeometryViewType::Main))->RemoveActor(m_patientPanoactorMain);

        _update_render();

        qDebug() << "removing pano";
        // Remove 시 모두 초기화

        isLoadPano = false;
        m_patientPano = nullptr;
        m_patientPanomapperAll = nullptr;
        m_patientPanoactorAll = nullptr;
        m_patientPanomapperMain = nullptr;
        m_patientPanoactorMain = nullptr;

        qDebug() << m_patientPano << m_patientPanoactorAll << m_patientPanoactorMain << m_patientPanomapperAll << m_patientPanomapperMain;
        return true;
    }

    /* All, Sub Window 에 출력된 환자 오브젝트 제거 */
    bool _Remove_CephPatient() {
        if(isLoadCeph != true)
            return false;
        qDebug() <<"null check cephp";
        _get_render(GeometryViewType::toString(GeometryViewType::All))->RemoveActor(m_patientCephactorAll);
        _get_render(GeometryViewType::toString(GeometryViewType::Sub))->RemoveActor(m_patientCephactorSub);

        _update_render();
        qDebug() << "removing ceph";
        // Remove 시 모두 초기화
        isLoadCeph = false;
        m_patientCeph = nullptr;
        m_patientCephactorAll = nullptr;
        m_patientCephactorSub = nullptr;
        m_patientCephmapperAll = nullptr;
        m_patientCephmapperSub = nullptr;

        qDebug() << m_patientCeph << m_patientCephactorAll << m_patientCephactorSub << m_patientCephmapperAll << m_patientCephmapperSub;

        return true;
    }

    void _on_XRayModule_Ready() {
        isRunning_Ceph = true;
        m_parentUI->CaptureReadyPushButton->setEnabled(false);
        m_parentUI->CaptureStartPushButton->setEnabled(false);
        for (int i = 0; i <= 45; i++)
        {
            if (!isRunning_Ceph)
                break;
            m_curXRayAngle--;
            _Rotate_Xray();
        }
        m_parentUI->CaptureReadyPushButton->setEnabled(true);
        m_parentUI->CaptureStartPushButton->setEnabled(true);
        isRunning_Ceph = false;
    }

    void _on_MainPushButton_clicked() {
        // 회전중인 경우 종료
        if (isRunning_Pano || isRunning_Ceph)
        {
            return;
        }
        isRunning_Pano = true;

        // 현재 Pano Angle을 +10도 까지 회전
        for (int i = 0; i <= 10; i++)
        {
            if (!isRunning_Pano)
            {
                break;
            }
            else {
                m_curPanoAngle++;
                _Rotate_Pano();
            }
            Sleep(10);
        }

        // 현재 Pano Angle을 반대로 200도 까지 회전
        for (int i = 0; i <= 200; i++)
        {
            if (!isRunning_Pano)
                break;
            else {
                if (0 <= i && i < 100)
                {
                    m_curPositionX = m_curPositionX - 1;
                }
                else if (100 <= i && i < 200)
                {
                    m_curPositionX = m_curPositionX + 1;
                }
                else if (i == 200)
                {
                    m_curPositionX = 0;
                }
                m_curPanoAngle--;
                _Rotate_Pano();
            }
            Sleep(20);
        }

        // 다시 + 방향으로 10도 회전
        for (int i = 0; i <= 10; i++)
        {
            if (!isRunning_Pano)
                break;
            else {
                m_curPanoAngle++;
                _Rotate_Pano();
            }
            Sleep(10);
        }
        isRunning_Pano = false;
    }

    void _on_SubPushButton_clicked()
    {
        if (isRunning_Pano || isRunning_Ceph)
            return;
        isRunning_Ceph = true;

        /*  양수는 반시계방향으로, 음수는 시계방향으로 이동 */
        for (int i = 0; i <= 45; i++)
        {
            if (!isRunning_Ceph)
                break;
            m_curXRayAngle--;
            _Rotate_Xray();
        }

        // Ceph Module AniMation
        _Animation_Ceph();

        for (int i = 0; i <= 45; i++)
        {
            if (!isRunning_Ceph)
                break;
            /* -45 만큼 이동한 상태를 원상복구함 */
            m_curXRayAngle++;
            _Rotate_Xray();
        }
        isRunning_Ceph = false;
    }


    void _on_reset_Panorama_module()
    {
        if (isRunning_Pano || isRunning_Ceph)
        {
            return;
        }
        isRunning_Pano = true;

        if (m_curPanoAngle == -180)
        {
            for (int i = 0; i <= 180; i++)
            {
                if (!isRunning_Pano)
                    break;
                m_curPanoAngle = -180 + i;
                _Rotate_Pano();

            }
            m_curPanoAngle = 0;
        }
        isRunning_Pano = false;
    }


    void _Animation_Ceph() {

        for (int i = 0; i < 520; i++)
        {
            if (!isRunning_Ceph)
                break;
            m_curPositionZ--;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, m_curPositionZ);

            // All
            m_actorMap.value(GeometryViewType::toString(All)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
            auto actorAll = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(All));
            actorAll->SetUserTransform(transform);

            // Sub
            m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
            auto actorSub = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub));
            actorSub->SetUserTransform(transform);


            transform->Update();
            qDebug() << "position Z : " << m_curPositionZ;
            _update_render();
        }

        for (int i = 0; i < 260; i++)
        {
            if (!isRunning_Ceph)
                break;
            m_curPositionZ = m_curPositionZ + 2;

            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
            transform->Translate(0, m_curPositionY, m_curPositionZ);

            // All
            m_actorMap.value(GeometryViewType::toString(All)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
            auto actorAll = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(All));
            actorAll->SetUserTransform(transform);

            // Sub
            m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
            auto actorSub = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub));
            actorSub->SetUserTransform(transform);


            transform->Update();
            qDebug() << "position Z : " << m_curPositionZ;
            _update_render();
        }
    }

    void _Rotate_Pano()
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        transform->Translate(-m_PanoCenter[0], -m_PanoCenter[1], -m_PanoCenter[2]);
        transform->RotateY(m_curPanoAngle);
        transform->Translate(m_PanoCenter);
        transform->Translate(m_curPositionX, m_curPositionY, 0);

        m_actorMap.value(GeometryViewType::toString(Main)).m_actor.value(GeometryDataType::toString(Pano))->SetUserTransform(transform);
        auto actorPanoMain = _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Main));
        actorPanoMain->SetUserTransform(transform);
        transform->Update();
        m_actorMap.value(GeometryViewType::toString(Main)).m_actor.value(GeometryDataType::toString(Xray))->SetUserTransform(transform);
        auto actorXrayMain = _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Main));
        actorXrayMain->SetUserTransform(transform);
        transform->Update();

        m_actorMap.value(GeometryViewType::toString(All)).m_actor.value(GeometryDataType::toString(Pano))->SetUserTransform(transform);
        auto actorPanoAll = _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(All));
        actorPanoAll->SetUserTransform(transform);
        transform->Update();
        m_actorMap.value(GeometryViewType::toString(All)).m_actor.value(GeometryDataType::toString(Xray))->SetUserTransform(transform);
        auto actorXrayAll = _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(All));
        actorXrayAll->SetUserTransform(transform);
        transform->Update();

        m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Pano))->SetUserTransform(transform);
        auto actorPanoSub = _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Sub));
        actorPanoSub->SetUserTransform(transform);
        transform->Update();
        m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Xray))->SetUserTransform(transform);
        auto actorXraySub = _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Sub));
        actorXraySub->SetUserTransform(transform);
        transform->Update();

        qDebug() << m_curPanoAngle;

        _update_render();
    }

    void _Rotate_Xray()
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        transform->Translate(-m_XrayCenter[0], -m_XrayCenter[1], -m_XrayCenter[2]);
        transform->RotateY(m_curXRayAngle);
        transform->Translate(m_XrayCenter);

        //악궁궤적 추적을 위한 x 좌표 변환 수치 적용
        transform->Translate(m_curPositionX, m_curPositionY, 0);

        m_actorMap.value(GeometryViewType::toString(Main)).m_actor.value(GeometryDataType::toString(Xray))->SetUserTransform(transform);
        auto actorXrayMain = _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Main));
        actorXrayMain->SetUserTransform(transform);
        transform->Update();

        m_actorMap.value(GeometryViewType::toString(All)).m_actor.value(GeometryDataType::toString(Xray))->SetUserTransform(transform);
        auto actorXrayAll = _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(All));
        actorXrayAll->SetUserTransform(transform);
        transform->Update();

        m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Xray))->SetUserTransform(transform);
        auto actorXraySub = _get_actor(GeometryDataType::toString(Xray), GeometryViewType::toString(Sub));
        actorXraySub->SetUserTransform(transform);
        transform->Update();

        qDebug() << m_curXRayAngle;

        _update_render();
    }

    void _Translate_Ceph()
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(0, m_curPositionY, m_curPositionZ);

        // Sub
        m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
        auto actorSub = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub));
        actorSub->SetUserTransform(transform);
        transform->Update();

        //All
        m_actorMap.value(GeometryViewType::toString(All)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
        auto actorAll = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(All));
        actorAll->SetUserTransform(transform);
        transform->Update();

        qDebug() << "position Z : " << m_curPositionZ;
        _update_render();
    }

    /* 파노라마 모듈 초기화 */
    void _on_PanoModel_Reset() {
        if (isRunning_Pano || isRunning_Ceph)
        {
            return;
        }
        isRunning_Pano = true;

        /* 파노라마 모듈 회전 각도 초기화 */
        if (m_curPanoAngle != 0)
        {
            if (m_curPanoAngle >= 0)
            {
                for (int i = m_curPanoAngle; i >= 0; i--)
                {
                    if (!isRunning_Pano)
                        break;
                    m_curPanoAngle = i;
                    _Rotate_Pano();
                }
            }
            else {
                for (int i = m_curPanoAngle; i <= 0; i++)
                {
                    if (!isRunning_Pano)
                        break;
                    m_curPanoAngle = i;
                    _Rotate_Pano();
                }
            }
        }

        /* 파노라마 모듈 악궁 X 축 초기화 */
        if(m_curPositionX !=0)
        {
            if(m_curPositionX > 0)
            {
                for(int i = m_curPositionX; i >= 0; i--)
                {
                    if(!isRunning_Pano)
                        break;
                    m_curPositionX = i;
                    _Rotate_Pano();
                }
            }
            if(m_curPositionX < 0 )
                for(int i = m_curPositionX; i <= 0; i++)
                {
                    if(!isRunning_Pano)
                        break;
                    m_curPositionX = i;
                    _Rotate_Pano();
                }
        }
        isRunning_Pano = false;
    }

    /* 세팔로 모듈 초기화 */
    void _on_CephModel_Reset() {
        if (isRunning_Pano || isRunning_Ceph)
        {
            return;
        }
        isRunning_Ceph = true;

        // 먼저 XRay 부터 원위치
        if (m_curXRayAngle != 0)
        {
            if (m_curXRayAngle >= 0)
            {
                for (int i = m_curXRayAngle; i >= 0; i--)
                {
                    if (!isRunning_Ceph)
                        break;
                    m_curXRayAngle = i;
                    _Rotate_Xray();
                }
            }
            else {
                for (int i = m_curXRayAngle; i <= 0; i++)
                {
                    if (!isRunning_Ceph)
                        break;
                    m_curXRayAngle = i;
                    _Rotate_Xray();
                }
            }
        }
        // Ceph Z Position이 0이 아니라면 원위치
        if (m_curPositionZ != 0)
        {
            if (m_curPositionZ >= 0)
            {
                for (int i = m_curPositionZ; i >= 0; i--)
                {
                    if (!isRunning_Ceph)
                        break;
                    m_curPositionZ = i;
                    _Translate_Ceph();
                }
            }
            else {
                for (int i = m_curPositionZ; i <= 0; i++)
                {
                    if (!isRunning_Ceph)
                        break;
                    m_curPositionZ = i;
                    _Translate_Ceph();
                }
            }
        }
        isRunning_Ceph = false;
    }
    /* 높이 조절 초기화 */
    void _on_Elevation_reset()
    {
        if(m_curPositionY < 0)
        {
            for(int i = m_curPositionY; i <= 0; i++)
            {
                m_curPositionY = i;
                _on_AscendingPushButton_pressed();
            }
        }
    }

    // Internal Methods must be used in the Internal.
private:
    /* 다중 ply 파일 호출 */
    void _load_plyfile(const QString& type, QMap<QString, vtkSmartPointer<vtkPolyData>>& plys)
    {
        vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
        switch (GeometryDataType::toEnum(type))
        {
        case GeometryDataType::Lower:
        {
            reader->SetFileName("./resources/T2 Modeling/LowerBody.ply");
        }break;
        case GeometryDataType::Upper:
        {
            reader->SetFileName("./resources/T2 Modeling/UpperBody.ply");
        }break;
        case GeometryDataType::Panorama:
        {
            reader->SetFileName("./resources/T2 Modeling/PanoModule.ply");
        }break;
        case GeometryDataType::Cephalo:
        {
            reader->SetFileName("./resources/T2 Modeling/CephModule.ply");
        }break;
        case GeometryDataType::Xray:
        {
            reader->SetFileName("./resources/T2 Modeling/XrayModule.ply");
        }break;
        }

        reader->Update();
        plys.insert(type, reader->GetOutput());
    }

    /* 파이프라인 매퍼를 생성 */
    void _create_Mapper(const QString& viewType, QMap<QString, vtkSmartPointer<vtkPolyData>>& plys, QMap<QString, MapperItem>& mapperMap)
    {

        switch (GeometryViewType::toEnum(viewType))
        {
        case GeometryViewType::All:
        {
            MapperItem item;
            for (auto it = plys.keyBegin(); it != plys.keyEnd(); it++)
            {
                vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
                map->SetInputData(plys.value(*it));
                map->Update();
                item.m_mapper.insert(*it, map);
            }
            mapperMap.insert(viewType, item);
        }break;
        case GeometryViewType::Main:
        {
            MapperItem item;
            for (auto it = plys.keyBegin(); it != plys.keyEnd(); it++)
            {
                vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
                map->SetInputData(plys.value(*it));
                map->Update();
                item.m_mapper.insert(*it, map);
            }
            mapperMap.insert(viewType, item);
        }break;
        case GeometryViewType::Sub:
        {
            MapperItem item;
            for (auto it = plys.keyBegin(); it != plys.keyEnd(); it++)
            {
                vtkSmartPointer<vtkPolyDataMapper> map = vtkSmartPointer<vtkPolyDataMapper>::New();
                map->SetInputData(plys.value(*it));
                map->Update();
                item.m_mapper.insert(*it, map);
            }
            mapperMap.insert(viewType, item);
        }break;
        default:
            break;
        }
    }

    /* 파이프라인 액터를 생성 */ 
    void _create_actor(const QString& viewType, QMap<QString, MapperItem>& mapperMap, QMap<QString, ActorItem>& actorMap)
    {

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        switch (GeometryViewType::toEnum(viewType))
        {
        case GeometryViewType::All:
        {
            if (mapperMap.contains(viewType))
            {
                auto map = mapperMap.value(viewType).m_mapper;
                ActorItem item;
                for (auto it = map.keyBegin(); it != map.keyEnd(); it++)
                {
                    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

                    actor->SetMapper(map.value(*it));
                    item.m_actor.insert(*it, actor);
                }

                actorMap.insert(viewType, item);
                //qDebug() << "actor Size : " << item.m_actor.size();
                //qDebug() << "actor Size : " << actorMap.value(viewType).m_actor.size();

            }
        }break;
        case GeometryViewType::Main:
        {
            if (mapperMap.contains(viewType))
            {
                auto map = mapperMap.value(viewType).m_mapper;
                ActorItem item;
                for (auto it = map.keyBegin(); it != map.keyEnd(); it++)
                {

                    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                    actor->SetMapper(map.value(*it));
                    item.m_actor.insert(*it, actor);
                }
                actorMap.insert(viewType, item);
            }
        }break;
        case GeometryViewType::Sub:
        {
            if (mapperMap.contains(viewType))
            {
                auto map = mapperMap.value(viewType).m_mapper;
                ActorItem item;
                for (auto it = map.keyBegin(); it != map.keyEnd(); it++)
                {
                    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

                    actor->SetMapper(map.value(*it));
                    item.m_actor.insert(*it, actor);
                }
                actorMap.insert(viewType, item);
            }
        }break;
        default:
            break;
        }
    }

    /* 파이프라인 렌더를 생성 */
    void _create_render(const QString& viewType, QMap<QString, ActorItem>& actorMap, QMap<QString, vtkSmartPointer<vtkRenderer>>& renderMap)
    {
        vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
        vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();

#ifdef USE_DISPLAY_GLOBALAXIS
        vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
        vtkSmartPointer<vtkTransform> axesTransform = vtkSmartPointer<vtkTransform>::New();
        vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
        vtkSmartPointer<vtkCamera> cam = Renderer->GetActiveCamera();
        axes->SetTotalLength(700, 700, 700);
        axes->GetCenter();
        axes->SetUserTransform(axesTransform);
        axesTransform->Translate(-1000, 0, 0);
        Renderer->AddActor(axes);

#endif
        switch (GeometryViewType::toEnum(viewType))
        {
        case GeometryViewType::All:
        {
            if (actorMap.contains(viewType))
            {
                auto actor = actorMap.value(viewType).m_actor;
                for (auto it = actor.keyBegin(); it != actor.keyEnd(); it++)
                {
                    Renderer->AddActor(actor.value(*it));
                }
                Renderer->SetBackground(colors->GetColor3d("Gray").GetData());
                Renderer->SetBackground2(colors->GetColor3d("Black").GetData());
                Renderer->GradientBackgroundOn();
                Renderer->ResetCamera();
                Renderer->LightFollowCameraOn();
                cam->SetViewUp(0,5000,0);
                cam->Azimuth(340);
                cam->Elevation(20);
                cam->Zoom(1.6);
                cam->GetFocalPoint();

                Renderer->ResetCameraClippingRange();
                renderMap.insert(viewType, Renderer);
            }
        }break;
        case GeometryViewType::Main:
        {
            if (actorMap.contains(viewType))
            {
                auto actor = actorMap.value(viewType).m_actor;
                for (auto it = actor.keyBegin(); it != actor.keyEnd(); it++)
                {
                    Renderer->AddActor(actor.value(*it));
                }
                Renderer->SetBackground(colors->GetColor3d("Black").GetData());
                Renderer->ResetCamera();
                Renderer->LightFollowCameraOn();
                cam->SetViewUp(0,5000,0);
                cam->Azimuth(280);
                cam->Elevation(20);
                cam->Zoom(4.7);
                cam->GetFocalPoint();
                cam->SetFocalPoint(2000,2000,1500);

                Renderer->ResetCameraClippingRange();
                renderMap.insert(viewType, Renderer);
            }
        }break;
        case GeometryViewType::Sub:
        {
            if (actorMap.contains(viewType))
            {
                auto actor = actorMap.value(viewType).m_actor;
                for (auto it = actor.keyBegin(); it != actor.keyEnd(); it++)
                {
                    Renderer->AddActor(actor.value(*it));
                }

                Renderer->SetBackground(colors->GetColor3d("Black").GetData());
                Renderer->ResetCamera();
                Renderer->LightFollowCameraOn();
                cam->SetViewUp(0,5000,0);

                cam->Azimuth(10);
                cam->Elevation(15);
                cam->Zoom(4.7);
                cam->GetFocalPoint();
                cam->SetFocalPoint(1000,1500,500);

                Renderer->ResetCameraClippingRange();
                renderMap.insert(viewType, Renderer);
            }
        }break;
        default:
            break;
        }
    }

    void _create_window(const QString& viewType, QMap<QString, vtkSmartPointer<vtkRenderer>>& renderMap, QMap<QString, vtkSmartPointer<vtkGenericOpenGLRenderWindow>>& windowMap)
    {
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

        switch (GeometryViewType::toEnum(viewType))
        {
        case GeometryViewType::All:
        {
            if (renderMap.contains(viewType))
            {
                auto render = renderMap.value(viewType);
                renderWindow->AddRenderer(render);
                windowMap.insert(viewType, renderWindow);
            }
        }break;
        case GeometryViewType::Main:
        {
            if (renderMap.contains(viewType))
            {
                auto render = renderMap.value(viewType);
                renderWindow->AddRenderer(render);
                windowMap.insert(viewType, renderWindow);
            }
        }break;
        case GeometryViewType::Sub:
        {
            if (renderMap.contains(viewType))
            {
                auto render = renderMap.value(viewType);
                renderWindow->AddRenderer(render);
                windowMap.insert(viewType, renderWindow);
            }
        }break;
        default:
            break;
        }
    }

    void _register_window(const QString& viewType, QMap<QString, vtkSmartPointer<vtkGenericOpenGLRenderWindow>>& windowMap)
    {
        switch (GeometryViewType::toEnum(viewType))
        {
        case GeometryViewType::All:
        {
            m_parentUI->openGLWidget_All->setRenderWindow(windowMap.value(viewType));
            m_parentUI->openGLWidget_All->interactor()->ProcessEvents();

        }break;
        case GeometryViewType::Main:
        {
            m_parentUI->openGLWidget_Main->setRenderWindow(windowMap.value(viewType));
            m_parentUI->openGLWidget_Main->interactor()->ProcessEvents();

        }break;
        case GeometryViewType::Sub:
        {
            m_parentUI->openGLWidget_Sub->setRenderWindow(windowMap.value(viewType));
            m_parentUI->openGLWidget_Sub->interactor()->ProcessEvents();
        }break;
        default:
            break;
        }
    }

    void _update_render()
    {
        for (auto& win : m_windowMap)
        {
            win->Render();
        }
        QApplication::processEvents();
    }

    vtkSmartPointer<vtkPolyData> _get_data(const QString& dataType)
    {
        return m_dataMap.value(dataType);
    }
    vtkSmartPointer<vtkMapper> _get_mapper(const QString& dataType, const QString& viewType)
    {
        return m_mapperMap.value(viewType).m_mapper.value(dataType);
    }
    vtkSmartPointer<vtkActor> _get_actor(const QString& dataType, const QString& viewType)
    {
        return m_actorMap.value(viewType).m_actor.value(dataType);
    }
    vtkSmartPointer<vtkRenderer> _get_render(const QString& viewType)
    {
        return m_renderMap.value(viewType);
    }
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> _get_window(const QString& viewType) {
        return m_windowMap.value(viewType);
    }
};

CBCTModelController::CBCTModelController(Ui::MainWindow* parentUI)
    : PData(new Internal(this))
{
    PData->m_parentUI = parentUI;
}

CBCTModelController::~CBCTModelController()
{
}

bool CBCTModelController::initialize()
{
    return PData->_initialize();
}

/* 파일 전송(Progressbar Panovalue)과 일치하는 PanoModule 모션 */
void CBCTModelController::on_Rotate_PanoObject(const int& val)
{
    auto angle = ceil(val / 4.16);
    qDebug() << "image Count : " << val << "angle : " << angle;

    if (val == 1750)
    {
        angle = 420;
    }

    if (0 < angle && angle <= 10)
    {
        if (PData->isRunning_Pano)
            return;
        PData->isRunning_Pano = true;
        PData->m_curPanoAngle = angle;
        PData->_Rotate_Pano();
        PData->isRunning_Pano = false;
    }

    if (10 < angle && angle <= 410)
    {
        if (PData->isRunning_Pano)
            return;
        if (10 < angle && angle <= 210)
        {
            PData->m_curPositionX = PData->m_curPositionX -0.15;
        }
        if (210 < angle && angle <= 410)
        {
            PData->m_curPositionX = PData->m_curPositionX +0.15;
        }
        PData->isRunning_Pano = true;
        PData->m_curPanoAngle = 10 - ((angle - 10) / 2);
        PData->_Rotate_Pano();
        PData->isRunning_Pano = false;
    }

    else if (410 < angle && angle <= 420)
    {
        if (PData->isRunning_Pano)
            return;
        PData->isRunning_Pano = true;
        PData->m_curPanoAngle = -190 + (angle -410);
        PData->_Rotate_Pano();
        PData->isRunning_Pano = false;
    }
}

/* 파일 전송(Progressbar Cephvalue)과 일치하는 CephModule 모션 */
void CBCTModelController::on_Translate_CephObject(const int& val)
{
    int count = 1.248 * val;

    qDebug() << "image : " << val << "count : " << count;

    if (0 < count && count <= 1040)
    {
        if (PData->isRunning_Ceph)
            return;
        PData->isRunning_Ceph = true;
        PData->m_curPositionZ = PData->m_curPositionZ -0.64;
        PData->_Translate_Ceph();
        PData->isRunning_Ceph = false;
    }
    else if (1040 < count && count <= 1560)
    {
        if (PData->isRunning_Ceph)
            return;
        PData->isRunning_Ceph = true;
        PData->m_curPositionZ = PData->m_curPositionZ + 1.28;
        PData->_Translate_Ceph();
        PData->isRunning_Ceph = false;
    }

}
void CBCTModelController::on_XRayModule_Ready()
{
    PData->_on_XRayModule_Ready();
}

void CBCTModelController::on_AscendingPushButton_pressed()
{
    PData->_on_AscendingPushButton_pressed();
}

void CBCTModelController::on_DescendingPushButton_pressed()
{
    PData->_on_DescendingPushButton_pressed();
}

bool CBCTModelController::Load_PanoPatient(const QString& path) {
    return PData->_Load_PanoPatient(path);
}

bool CBCTModelController::Load_CephPatient(const QString& path) {
    return PData->_Load_CephPatient(path);
}

void CBCTModelController::on_PanoModel_Reset() {
    PData->_on_PanoModel_Reset();
    /* 높이 조절 초기화 */
    PData->_on_Elevation_reset();
}
void CBCTModelController::on_CephModel_Reset() {
    PData->_on_CephModel_Reset();
    /* 높이 조절 초기화 */
    PData->_on_Elevation_reset();
}


bool CBCTModelController::Remove_PanoPatient() {
    return PData->_Remove_PanoPatient();

}

bool CBCTModelController::Remove_CephPatient() {
    return PData->_Remove_CephPatient();
}



void CBCTModelController::stop() {

    PData->_stop();
}

void CBCTModelController::on_CaptureVTK_Reset()
{
    PData->m_parentUI->CaptureResetPushButton->setEnabled(false);

    PData->_on_PanoModel_Reset();
    PData->_on_CephModel_Reset();
    PData->_on_Elevation_reset();

    PData->m_parentUI->CaptureResetPushButton->setEnabled(true);
    PData->m_parentUI->CaptureReadyPushButton->setEnabled(true);

}

/* 장비 컨트롤러 : 리셋 */
void CBCTModelController::on_ResetPushButton_clicked()
{
    PData->m_parentUI->ResetPushButton->setEnabled(false);

    PData->_on_PanoModel_Reset();
    PData->_on_CephModel_Reset();
    PData->_on_Elevation_reset();

    PData->m_parentUI->AscendingPushButton->setEnabled(true);
    PData->m_parentUI->DescendingPushButton->setEnabled(true);
    PData->m_parentUI->MainPushButton->setEnabled(true);
    PData->m_parentUI->SubPushButton->setEnabled(true);
}

/* 장비 컨트롤러 : 파노라마 */
void CBCTModelController::on_MainPushButton_clicked()
{
    // Main Push Button 비활성화
    /*리셋으로 초기화 하기 전까지 메인 버튼 비활성화 */
    PData->m_parentUI->MainPushButton->setEnabled(false);
    PData->m_parentUI->SubPushButton->setEnabled(false);
    PData->m_parentUI->StopPushButton->setEnabled(true);
    PData->m_parentUI->AscendingPushButton->setEnabled(false);
    PData->m_parentUI->DescendingPushButton->setEnabled(false);
    qDebug() << "Main Push Btn!!";
    PData->_on_MainPushButton_clicked();

}

/* 장비 컨트롤러 : 세팔로 */
void CBCTModelController::on_SubPushButton_clicked()
{
    PData->m_parentUI->MainPushButton->setEnabled(false);
    PData->m_parentUI->SubPushButton->setEnabled(false);
    PData->m_parentUI->StopPushButton->setEnabled(true);
    PData->m_parentUI->AscendingPushButton->setEnabled(false);
    PData->m_parentUI->DescendingPushButton->setEnabled(false);
    if (PData->m_curPanoAngle != 0)
    {
        qDebug() << "Pano Module Reset required";
        return;
    }
    else {
        qDebug() << "Sub Push Btn!!";
        PData->_on_SubPushButton_clicked();
    }
}

/* 장비 컨트롤러 : 정지 */
void CBCTModelController::on_StopPushButton_clicked()
{
    PData->_stop();
    PData->m_parentUI->ResetPushButton->setEnabled(true);
    PData->m_parentUI->StopPushButton->setEnabled(false);
    PData->m_parentUI->AscendingPushButton->setEnabled(false);
    PData->m_parentUI->DescendingPushButton->setEnabled(false);
    PData->m_parentUI->MainPushButton->setEnabled(false);
    PData->m_parentUI->SubPushButton->setEnabled(false);
}

/* Stop 기능 작동 시 환자가 퇴실했을 때만 리셋 버튼 활성화 */
void CBCTModelController::remove_Patient_Exception()
{
    if(PData->m_patientPano == nullptr && PData->m_patientCeph == nullptr)
    {
        PData->m_parentUI->CaptureResetPushButton->setEnabled(true);
    }
    else{
        PData->m_parentUI->CaptureResetPushButton->setEnabled(false);
    }
}
