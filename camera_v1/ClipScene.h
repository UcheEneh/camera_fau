#pragma once


#include <QtWidgets/QGraphicsScene>

//for opencv
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>

class QGraphicsSceneMouseEvent; //move mouse
class QGraphicsPixmapItem;  //show image
class QGraphicsRectItem;    //choose crop region
class QGraphicsPolygonItem; //choose crop region using polygon

class ClipScene : public QGraphicsScene //QGrpshicsScene is a subclass of QtWidgets
{
    Q_OBJECT
    //Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    ClipScene(QObject* parent);
    QPointF previousPosition() const;   //QPointF: defines a point in the plane using floating point precision.
    void setImage(const QString& filePath, int width, int height);  // Method for replacing an image in QGraphicsScene
    //void setImage2(const QPixmap img);
    QImage image;   //used for ewidget to add point
    QImage returnImage();

    //void undoPoints();
    bool polygonAdded;

    void applyCrop();

signals:
    void previousPositionChanged(const QPointF previousPosition);   //signal to update rectangle if mouse position moves
    void clippedImage(const QPixmap& pixmap);   //A signal that transmits the cut out image area to the application window to show it in the label

public slots:
    void setPreviousPosition(const QPointF previousPosition);

protected:
    /*
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    */

private:
    QGraphicsPixmapItem* m_currentImageItem {nullptr};
    QPointF m_previousPosition;
    bool m_leftMouseButtonPressed           {false};

    QGraphicsRectItem* m_selection          {nullptr};
    //QGraphicsPolygonItem* m_selection       {nullptr};
    //QGraphicsEllipseItem *ellipse_item[4]       {nullptr};


    QPixmap img_pm;

    QPolygonF poly;
    QVector<QPointF> pointsVector;      //vector used to eventually make polygon

    //WITH OPENCV
    //QVector<cv::Point2f> pointsVector_cv;

    int pressCount = 0;
};
