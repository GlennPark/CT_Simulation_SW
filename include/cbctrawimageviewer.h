﻿#ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>
#include <qdebug.h>
#include "ui_mainwindow.h"

class QDir;
class QDirIterator;
class QTimer;

class CBCTRawImageViewer : public QObject
{
    Q_OBJECT
public:
    explicit CBCTRawImageViewer();
    ~CBCTRawImageViewer();
    Ui::MainWindow* m_mainwindowUi = nullptr;

    QPixmap PanoImageViewer();
    QPixmap CephImageViewer();

    void startPanoTimer();
    void stopPanoTimer();

    void startCephTimer();
    void stopCephTimer();

private:
    QDirIterator *panoImageIterator;
    QDirIterator *cephImageIterator;
    QTimer *panoImageTimer;
    QTimer *cephImageTimer;

private slots:
    void timeoutPanoTimer();
    void timeoutCephTimer();

signals:
    void signals_panoImage(QImage*);
    void signals_cephImage(QImage*);

};

#endif // CBCTRAWIMAGEVIEWER_H
