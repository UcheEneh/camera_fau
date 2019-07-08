#ifndef CROPPING_DIALOG_H
#define CROPPING_DIALOG_H

#include <QDialog>
#include <QLabel>
/*
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
*/

#include <QMouseEvent>

#include "ClipScene.h"

class ClipScene;

namespace Ui {
class cropping_Dialog;
}

class cropping_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit cropping_Dialog(QWidget *parent = nullptr);
    ~cropping_Dialog();

    void setData (QString &filename);
    QString global_fname;
    QSize size;

private slots:
    // void onAddFile();                           // Slot for adding an image to the application
    void onClippedImage(const QPixmap& pixmap); // Slot for accepting a cropped application area
    // void onSaveImage();                         // Slot for saving cropped image (For now, also closes)
    void undoCrop();                            //Slot for reloading original image
    //void deletePoints();                        //Slot to delete polygon points
    void crop();                            //Slot for reloading original image

    // void on_Edit();

private:
    Ui::cropping_Dialog *ui;
    ClipScene* m_clipScene;         // The graphical scene in which the image trimming functionality is implemented

    QImage image;
    QPointF lastPoint;
    QString file;

    bool cropPerformed = false;
    bool leftMouseButtonPressed           {false};

    //WITH OPENCV
    // QVector<cv::Point2f> pointsVector_cv;
    int pressCount_cv = 0;

    QLabel imageLabel;
    QPixmap pmap;
};

#endif // CROPPING_DIALOG_H
