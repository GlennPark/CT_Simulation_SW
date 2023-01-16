    #ifndef CBCTRAWIMAGEVIEWER_H
#define CBCTRAWIMAGEVIEWER_H

#include <QObject>

class CBCTRawImageViewer : public QObject
{
    Q_OBJECT
public:
    CBCTRawImageViewer();



    void PanoImageViewer();
    void CephImageViewer();
};

#endif // CBCTRAWIMAGEVIEWER_H
