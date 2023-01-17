#ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>

class QDir;
class QDirIterator;
class QTimer;

class CBCTRawImageViewer : public QObject
{
    Q_OBJECT
public:
    explicit CBCTRawImageViewer();
    ~CBCTRawImageViewer();

    QPixmap PanoImageViewer();
    QPixmap CephImageViewer();

    void startPanoTimer();
    void stopPanoTimer();

private:
    QDirIterator *panoImageIterator;
    QDirIterator *cephImageIterator;
    QTimer *panoImageTimer;
    QTimer *cephImageTimer;

private slots:
    void timeoutPanoTimer();

signals:
    void signals_panoImage(QImage*);


};

#endif // CBCTRAWIMAGEVIEWER_H
