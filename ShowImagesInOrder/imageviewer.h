#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageViewer; }
QT_END_NAMESPACE

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
    void showNextImage();

private:
    Ui::ImageViewer *ui;
    QDirIterator *imageIterator;
    QTimer *imageTimer;
};
#endif // IMAGEVIEWER_H
