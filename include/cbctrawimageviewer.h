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

    QPixmap ShowNextPanoImage();
    QPixmap ShowNextCephImage();

private:
    QDirIterator *panoImageIterator;
    QDirIterator *cephImageIterator;
    QTimer *panoImageTimer;
    QTimer *cephImageTimer;



};

#endif // CBCTRAWIMAGEVIEWER_H
