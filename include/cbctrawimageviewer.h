#ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>
#include <qdebug.h>
#include "ui_mainwindow.h"
#include "cbctmodelcontroller.h"

class QDir;
class QDirIterator;
class QTimer;
class QElapsedTimer;

class CBCTRawImageViewer : public QObject
{
    Q_OBJECT
public:
    explicit CBCTRawImageViewer();
    ~CBCTRawImageViewer();
    Ui::MainWindow* m_parentUI = nullptr;

    void resetPanoTimer();
    void resetCephTimer();
    void startPanoTimer();
    void stopPanoTimer();

    void startCephTimer();
    void stopCephTimer();

private:
    CBCTModelController* m_modelController;

    bool isRunningPano = true;
    bool isRunningCeph = true;

signals:
    void signals_panoImage(QImage*);
    void signals_cephImage(QImage*);

};

#endif // CBCTRAWIMAGEVIEWER_H
