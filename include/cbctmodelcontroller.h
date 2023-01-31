#ifndef CBCTMODELCONTROLLER_H
#define CBCTMODELCONTROLLER_H

#include "cbctrawimageviewer.h"
#include <QObject>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class GeometryDataType {
public:
    enum Type {
        Upper = 0,
        Lower,
        Panorama,
        Cephalo,
    };

    static QString toString(int type) {
        switch (type) {
        case GeometryDataType::Upper:
            return "Upper";
        case GeometryDataType::Lower:
            return "Lower";
        case GeometryDataType::Panorama:
            return "Panorama";
        case GeometryDataType::Cephalo:
            return "Cephalo";
        }

        return "Error";
    }

    static int toEnum(const QString& type) {
        if (type == "Upper") {
            return GeometryDataType::Upper;
        }
        else if (type == "Lower") {
            return GeometryDataType::Lower;
        }
        else if (type == "Panorama") {
            return GeometryDataType::Panorama;
        }
        else if (type == "Cephalo") {
            return GeometryDataType::Cephalo;
        }
        return -1;
    }
};

class GeometryViewType {
public:
    enum Type {
        All = 0,
        Main,
        Sub,
    };

    static QString toString(int type) {
        switch (type) {
        case GeometryViewType::All:
            return "All";
        case GeometryViewType::Main:
            return "Main";
        case GeometryViewType::Sub:
            return "Sub";

        }

        return "Error";
    }

    static int toEnum(const QString& type) {
        if (type == "All") {
            return GeometryViewType::All;
        }
        else if (type == "Main") {
            return GeometryViewType::Main;
        }
        else if (type == "Sub") {
            return GeometryViewType::Sub;
        }
        return -1;
    }
};

class CBCTModelController : public QObject
{
    Q_OBJECT
public:
    CBCTModelController(Ui::MainWindow* mainwindowUi = nullptr);
    ~CBCTModelController();

    /* Internal method 를 외부 클래스에서 호출하기 위해 담아주는 함수들 */
    void reset_VTK_Function();
    void ready_VTK_Fucntion();
    void start_VTK_Function();
    void stop_VTK_Function();

    /* Viewer 와 동시 실행을 위해 생성한 메소드 */
    void pano_VTK_Function();
    void ceph_VTK_Function();
    \
    bool initialize();
    void test();



private slots:


    /* 모션 동작을 구현한 Internal method 를 슬롯 함수로 변환 */

    /* 상승 Btn : Lower Body Obj File 만 빼고 Y Axis 기준 +Up Max를 벗어나면 안된다. */
    void ascending_Function();
    /* 하강 Btn : Lower Body Obj File 만 빼고 Y Axis 기준 -Lo Min을 벗어나면 안된다. */
    void descending_Function();
    /*  MainPush Btn : Pano Obj File이 Y Axis 기준으로 360 회전을 한다. */
    void panorama_Module_Function();
    /* SubPush Btn : Ceph Obj File이 우측 끝에서 좌측 끝으로 이동 후 원 상태로 복귀 한다. */
    void cephalo_Module_Function();

   // void ready_Function();

private:
    CBCTRawImageViewer* m_rawImageViewer;
    class VTKInternal;
    VTKInternal* Internal;
};

#endif // CBCTMODELCONTROLLER_H
