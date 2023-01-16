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

    void showNextImage();

private:
    QDirIterator *dirIterator;
    QTimer *timer;



};

#endif // CBCTRAWIMAGEVIEWER_H
