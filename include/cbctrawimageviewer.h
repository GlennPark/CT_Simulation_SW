#ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>
#include <qdebug.h>
#include "ui_mainwindow.h"

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
    void readyPanoTimer();
    void readyCephTimer();
    void startPanoTimer();
    void stopPanoTimer();

    void startCephTimer();
    void stopCephTimer();

private:
    bool isRunningPano = false;
    bool isRunningCeph = false;

    QDirIterator* panoImageIterator;
    QDirIterator* cephImageIterator;
    QTimer* panoImageTimer;
    QTimer* cephImageTimer;

signals:
    void signals_panoImage(QImage*);
    void signals_cephImage(QImage*);

};

#endif // CBCTRAWIMAGEVIEWER_H
