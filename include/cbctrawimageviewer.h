#ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>

class CBCTRawImageViewer
{
    Q_OBJECT
public:
    CBCTRawImageViewer();



private:

    void PanoImageViewer();
    void CephImageViewer();
};

#endif // CBCTRAWIMAGEVIEWER_H
