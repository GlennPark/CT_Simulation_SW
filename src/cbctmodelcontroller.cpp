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
    int m_curPositionZ = 0; // cuttent Z Position Value

    double m_curAngle = 0;
    double* m_PanoCenter;
    double* m_XrayCenter;

    bool isRunning_Pano = false;
    bool isRunning_Ceph = false;
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
private:

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
        qDebug(" ply load ") ;

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


    void _on_AscendingPushButton_pressed() {

        _on_reset_Panorama_module();

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

    void _on_MainPushButton_clicked() {

        m_parentUI->MainPushButton->setEnabled(false);
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(m_curPositionX, m_curPositionY, 0);



        // Main



        if (isRunning_Pano || isRunning_Ceph)
        {
            return;
        }
        isRunning_Pano = true;

        for (int i = 0; i <= 20; i++)
        {
            if(!isRunning_Pano)
            {
                break;
            }

            _Rotate_Pano(i/2);
            transform->Update();
        }m_curAngle = m_curAngle +10;

        for (int i = 0; i <= 800; i++)
        {
            if (!isRunning_Pano)
                break;
            if(0 <= i && i < 400)
            {
                m_curPositionX = m_curPositionX -0.4;
                transform->Update();
            }
            else if(400 <= i && i < 800)
            {
                m_curPositionX = m_curPositionX +0.4;
                transform->Update();
            }
            else if(i == 800)
            {
                m_curPositionX = 0;
                transform->Update();
            }
            _Rotate_Pano(10 -i/4);
            transform->Update();
        }m_curAngle = m_curAngle - 200;


        for (int i = 0; i <= 20; i++)
        {
            if(!isRunning_Pano)
                break;

            _Rotate_Pano(-190 +i/2);
             transform->Update();
        }m_curAngle = m_curAngle + 10;
        isRunning_Pano = false;

        /*리셋으로 초기화 하기 전까지 메인 버튼 비활성화 */





    }

    void _on_reset_Panorama_module()
    {

        if (isRunning_Pano || isRunning_Ceph)
        {
            return;
        }
        isRunning_Pano = true;

        if(m_curAngle == -180)
        {
            for (int i = 0; i <= 180; i++)
            {
                if(!isRunning_Pano)
                    break;

                _Rotate_Pano(-180 +i);

            }
            m_curAngle = 0;
        }
        isRunning_Pano = false;

    }

    void _on_SubPushButton_clicked()
    {
        if (isRunning_Pano || isRunning_Ceph)
            return;
        isRunning_Pano = true;

        /*  양수는 반시계방향으로, 음수는 시계방향으로 이동 */
        for (int i = 0; i <= 45; i++)
        {
            if (!isRunning_Pano)
                break;

            _Rotate_Xray(-i);
        }
        isRunning_Pano = false;

        if (isRunning_Pano || isRunning_Ceph)
            return;
        isRunning_Ceph = true;

        _Animation_Ceph();

        isRunning_Ceph = false;

        if (isRunning_Pano || isRunning_Ceph)
            return;
        isRunning_Pano = true;

        for (int i = 0; i <= 45; i++)
        {
            if (!isRunning_Pano)
                break;
            /* -45 만큼 이동한 상태를 원상복구함 */
            _Rotate_Xray(-45+i);
        }
        isRunning_Pano = false;
    }


    void _on_CaptureResetPushButton_VTK_clicked()
    {
        //        isRunning_Ceph = false;
        //        isRunning_Pano = false;
        /*파노라마 리셋*/
        _on_reset_Panorama_module();

        /* 하강된 높이 리셋 */

        do
        {
            _on_AscendingPushButton_pressed();
        }
        while(m_curPositionY < 0);


        //        _on_reset_Panorama_module();
        //        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        //        m_curPositionX = 0;
        //        m_curPositionY = 0;
        //        m_curPositionZ = 0;
        //        transform->Translate(m_curPositionX, m_curPositionY, m_curPositionZ);

        //        // All
        //        _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(All))->SetUserTransform(transform);
        //        _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(All))->SetUserTransform(transform);
        //        _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(All))->SetUserTransform(transform);


        //        // Main
        //        _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(Main))->SetUserTransform(transform);
        //        _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Main))->SetUserTransform(transform);
        //        _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Main))->SetUserTransform(transform);

        //        // Sub
        //        _get_actor(GeometryDataType::toString(Upper), GeometryViewType::toString(Sub))->SetUserTransform(transform);
        //        _get_actor(GeometryDataType::toString(Pano), GeometryViewType::toString(Sub))->SetUserTransform(transform);
        //        _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub))->SetUserTransform(transform);

        //        transform->Update();

        //        _update_render();

        m_parentUI->MainPushButton->setEnabled(true);
    }

    void _on_CaptureReadyPushButton_VTK_clicked()
    {

    }

    void _Animation_Pano() {


    }

    void _Animation_Ceph() {
        // max 260;
        // min 270;
        //        for (int i = 0; i < 52; i++)
        //        {
        //            if (!isRunning_Ceph)
        //                break;
        //            m_curPositionZ = m_curPositionZ + 5;

        //            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        //            transform->Translate(0, m_curPositionY, m_curPositionZ);

        //            // Sub
        //            m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
        //            auto actor = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub));
        //            actor->SetUserTransform(transform);

        //            transform->Update();
        //            qDebug() << "position Z : " << m_curPositionZ;
        //            _update_render();
        //        }
        for (int i = 0; i < 520; i++)
        {
            if (!isRunning_Ceph)
                break;
            m_curPositionZ = m_curPositionZ - 1;

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
        //        for (int i = 0; i < 52; i++)
        //        {
        //            if (!isRunning_Ceph)
        //                break;
        //            m_curPositionZ = m_curPositionZ + 5;

        //            vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        //            transform->Translate(0, m_curPositionY, m_curPositionZ);

        //            // Sub
        //            m_actorMap.value(GeometryViewType::toString(Sub)).m_actor.value(GeometryDataType::toString(Ceph))->SetUserTransform(transform);
        //            auto actor = _get_actor(GeometryDataType::toString(Ceph), GeometryViewType::toString(Sub));
        //            actor->SetUserTransform(transform);
        //            transform->Update();

        //            qDebug() << "position Z : " << m_curPositionZ;
        //            _update_render();
        //        }
    }
    void _Rotate_Pano(int angle)
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        transform->Translate(-m_PanoCenter[0], -m_PanoCenter[1], -m_PanoCenter[2]);
        transform->RotateY(angle);
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



        qDebug() << angle;

        _update_render();
    }

    void _Rotate_Xray(int angle)
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        transform->Translate(-m_XrayCenter[0], -m_XrayCenter[1], -m_XrayCenter[2]);
        transform->RotateY(angle);
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

        qDebug() << angle;

        _update_render();
    }

    void _Translate_Ceph(double posZ)
    {
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->Translate(0, m_curPositionY, posZ);

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

        qDebug() << "position Z : " << posZ;
        _update_render();
    }
    // Internal Methods must be used in the Internal.
private:
    void _load_plyfile(const QString& type, QMap<QString, vtkSmartPointer<vtkPolyData>>& plys)
    {
        vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
        switch (GeometryDataType::toEnum(type))
        {
        case GeometryDataType::Lower:
        {
            reader->SetFileName("C:/Qt_VTK_CT/resources/T2 Modeling/LowerBody.ply");

        }break;
        case GeometryDataType::Upper:
        {
            reader->SetFileName("C:/Qt_VTK_CT/resources/T2 Modeling/UpperBody.ply");
        }break;
        case GeometryDataType::Panorama:
        {
            reader->SetFileName("C:/Qt_VTK_CT/resources/T2 Modeling/PanoModule.ply");
        }break;
        case GeometryDataType::Cephalo:
        {
            reader->SetFileName("C:/Qt_VTK_CT/resources/T2 Modeling/CephModule.ply");
        }break;
        case GeometryDataType::Xray:
        {
            reader->SetFileName("C:/Qt_VTK_CT/resources/T2 Modeling/XrayModule.ply");
        }break;
        }

        reader->Update();
        plys.insert(type, reader->GetOutput());


    }

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

    void _create_render(const QString& viewType, QMap<QString, ActorItem>& actorMap, QMap<QString, vtkSmartPointer<vtkRenderer>>& renderMap)
    {
        vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
        vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();

#ifdef USE_DISPLAY_GLOBALAXIS
        vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
        vtkSmartPointer<vtkTransform> axesTransform = vtkSmartPointer<vtkTransform>::New();

        axes->SetTotalLength(700, 700, 700);
        axes->GetCenter();
        axes->SetUserTransform(axesTransform);
        axesTransform->Translate(-1000,0,0);
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
    auto angle = ceil(val / 3.89);
    qDebug() << "image Count : " << val << "angle : " << angle;

    if(val == 1750)
    {
        angle = 450;
    }

    if(0 < angle && angle <= 405)
    {
        if (PData->isRunning_Pano)
            return;
        PData->isRunning_Pano = true;
        PData->_Rotate_Pano(angle);
        PData->isRunning_Pano = false;
    }
    else if( 405 < angle && angle <= 450)
    {
        if (PData->isRunning_Pano)
            return;
        PData->isRunning_Pano = true;
        PData->_Rotate_Pano(450 -angle);
        PData->isRunning_Pano = false;
    }
}

/* 파일 전송(Progressbar Cephvalue)과 일치하는 CephModule 모션 */
void CBCTModelController::on_Translate_CephObject(const int& val)
{

    int count = 0.848 * val;

    qDebug() << "image : " << val << "count : " << count;
    // Z축 +동작
    if (0 < count && count <= 260)
    {
        if (PData->isRunning_Ceph)
            return;
        PData->isRunning_Ceph = true;

        PData->_Translate_Ceph(count);

        PData->isRunning_Ceph = false;
    }
    else if (260 < count && count <= 520)
    {
        if (PData->isRunning_Ceph)
            return;
        PData->isRunning_Ceph = true;

        PData->_Translate_Ceph(520 - count);
        PData->isRunning_Ceph = false;
    }
    else if (520 < count && count <= 790)
    {

        if (PData->isRunning_Ceph)
            return;
        PData->isRunning_Ceph = true;

        PData->_Translate_Ceph(-(count - 520));
        PData->isRunning_Ceph = false;
    }
    else if (790 < count && count <= 1060)
    {

        if (PData->isRunning_Ceph)
            return;
        PData->isRunning_Ceph = true;

        PData->_Translate_Ceph(-(1060 - count));
        PData->isRunning_Ceph = false;
    }

}
void CBCTModelController::on_MainPushButton_clicked()
{
    qDebug() << "Main Push Btn!!";
    PData->_on_MainPushButton_clicked();
}

void CBCTModelController::on_SubPushButton_clicked()
{
    qDebug() << "Sub Push Btn!!";
    PData->_on_SubPushButton_clicked();
}

void CBCTModelController::on_AscendingPushButton_pressed()
{
    PData->_on_AscendingPushButton_pressed();
}

void CBCTModelController::on_DescendingPushButton_pressed()
{
    PData->_on_DescendingPushButton_pressed();
}

void CBCTModelController::on_CaptureResetPushButton_VTK_clicked()
{
    PData->_on_CaptureResetPushButton_VTK_clicked();
}

void CBCTModelController::on_CaptureReadyPushButton_VTK_clicked()
{
    PData->_on_CaptureReadyPushButton_VTK_clicked();
}
