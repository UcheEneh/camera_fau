#include "ClipScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsPolygonItem>

#include "SizeGripItem.h"


//These are useful for resizing the RectItem or EllipseItem whichever
namespace
{
    class RectResizer : public SizeGripItem::Resizer
    {
        public:
            virtual void operator()(QGraphicsItem* item, const QRectF& rect)
            {
                QGraphicsRectItem* rectItem = dynamic_cast<QGraphicsRectItem*>(item);
                if (rectItem)
                    rectItem->setRect(rect);
            }
    };

    class EllipseResizer : public SizeGripItem::Resizer
    {
        public:
            virtual void operator()(QGraphicsItem* item, const QRectF& rect)
            {
                QGraphicsEllipseItem* ellipseItem =
                    dynamic_cast<QGraphicsEllipseItem*>(item);
                if (ellipseItem)
                    ellipseItem->setRect(rect);
            }
    };
}


//USED TO SET UP THE CROP SETTINGS

ClipScene::ClipScene(QObject* parent) : QGraphicsScene(parent)
{

}



void ClipScene::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(m_previousPosition);
}


QPointF ClipScene::previousPosition() const
{
    return m_previousPosition;
}

void ClipScene::setImage(const QString& filePath, int width, int height)
{
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }
    //m_currentImageItem = new QGraphicsPixmapItem(QPixmap(filePath));

    QPixmap loaded_img(filePath);
    img_pm = loaded_img;

    //Convet from QPixmap to QImage and resize (FOR RESIZING DRAWING POINT IN EWIDGET?)
    image = img_pm.toImage();

    //FOR RESIZING IMAGE BUT 400 is hardcoded
    m_currentImageItem = new QGraphicsPixmapItem();
    m_currentImageItem->setPixmap(img_pm.scaled(QSize(width, height),// 300, 300),    //try to make this not hardcoded
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //Add it to the graphic scene
    addItem(m_currentImageItem);

    //Add graphics item if graphics scene has image
    m_selection = new QGraphicsRectItem();
    m_selection->setBrush(QBrush(QColor(158,182,255,96)));
    m_selection->setPen(QPen(QColor(158,182,255,200),1));
    //m_selection->setRect(0, 0, 70, 70);
    m_selection->setRect(this->width()/2 - 35, this->height()/2 - 35, 280, 280);

    //IF THIS IS SET, WHEN I CROP, IT DOESN'T SELECT NEW REGION
    //m_selection->setFlag(QGraphicsItem::ItemIsMovable);


    // Add it to the graphic scene
    addItem(m_selection);

    //setPreviousPosition(m_selection->rect());
    //MAYBE CREATE SETPREVIOUSRECT MEMBER FN

    //Make selection region scalable
    SizeGripItem* rectSizeGripItem = new SizeGripItem(new RectResizer, m_selection);
    //SizeGripItem* ellipseSizeGripItem = new SizeGripItem(new EllipseResizer, ellipseItem);


}

QImage ClipScene::returnImage()
{
    return image;
}


void ClipScene::applyCrop()
{
    QRect selectionRect = m_selection->boundingRect().toRect();
    clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
    delete m_selection;
}


/*
void ClipScene::setImage2(const QPixmap img)
{
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }

    //FOR RESIZING IMAGE BUT 400 is hardcoded
    QGraphicsPixmapItem *currentImageItem = new QGraphicsPixmapItem();
    currentImageItem->setPixmap(img.scaled(QSize(300, 300),    //try to make this not hardcoded
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //Add it to the graphic scene
    addItem(currentImageItem);
}
*/

//OTHER METHOD OF SELECTING POINTS
/*
void ClipScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_leftMouseButtonPressed = true;
        if (m_currentImageItem)     //only perform the ff if an image has been loaded to QGraphics...
        {
            pressCount++;

            //probably no longer needed since m_previousPosition wouldn't be needed anymore
            //setPreviousPosition(event->scenePos()); //set current press position as previous pos.

            if(pointsVector.count() < 4)    //if < 5, add press point to polygon
            {
                //set mouse press position as a point in the vector
                auto x(event->scenePos().x());
                auto y(event->scenePos().y());
                QPointF point_xy(x, y);
                pointsVector.append(point_xy);

                //WITH OPENCV
                //cv::Point2f point_xy_cv(x, y);
                //pointsVector_cv.append(point_xy_cv);
                //-----------

                //Use ellipse fn to draw the point clicked onto the image
                QRectF ellipse_rect(x-5, y-5, 10, 10);
                ellipse_item[pointsVector.count() - 1] = new QGraphicsEllipseItem();
                ellipse_item[pointsVector.count() - 1]->setBrush(QBrush(QColor(158,182,255,96)));
                ellipse_item[pointsVector.count() - 1]->setPen(QPen(QColor(158,182,255,200),1));
                ellipse_item[pointsVector.count() - 1]->setRect(ellipse_rect);
                //Add it to the graphic scene
                addItem(ellipse_item[pointsVector.count() - 1]);
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void ClipScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_leftMouseButtonPressed = false;

        //if(pointsVector.count() == 4)
        if(pressCount == 4)
        {
            pressCount = 0;

            //Create a selection region (square, polygon...)    //create after all 4 positions chosen
            //m_selection = new QGraphicsRectItem();
            m_selection = new QGraphicsPolygonItem();
            m_selection->setBrush(QBrush(QColor(158,182,255,96))); //(r,g,b,a), a = transparency
            m_selection->setPen(QPen(QColor(158,182,255,200),1));  //same colour as brush, pen width = 1

            //Add it to the graphic scene
            addItem(m_selection);

            poly = pointsVector;    //QPolygonF poly(QVector<QPointF>({}));
            m_selection->setPolygon(poly);

            //CHANGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //IF OTHER IRREGULAR POLYGONS ARE USED THEN THIS WOULD PROBABLY NO LONGER BE boundingRect
            QRect selectionRect = m_selection->boundingRect().toRect();
            //m_selection->boundingRegion(); ???

            //copy and show the region of the box selected on the main image
            clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
            delete m_selection;

            //draw polygon on image to show cropped region.
            //EDIT THIS SO IT DOESN'T SHOW EVERY SINGLE BOX //Maybe take this out of the if loop????????
            addPolygon(poly);
            polygonAdded = true;

            /
            //------------------ METHOD OPENCV ------------------------------------//
            cv::Mat source_image = cv::imread("/home/uche/Pictures/IMG_00000001.jpg");

            //And you have four points you want to use as the corners:
            cv::Point corners[1][4];
            corners[0][0] = pointsVector_cv[0];    //cv::Point2f(30, 30);    //point 1
            corners[0][1] = pointsVector_cv[1];    //cv::Point2f(100, 100);    //point 2
            corners[0][2] = pointsVector_cv[2];    //cv::Point2f(200, 50);    //point 3
            corners[0][3] = pointsVector_cv[3];    //cv::Point2f(50, 25);     //point 4
            const cv::Point* corner_list[1] = {corners[0]};

            //You can use the function cv::fillPoly to draw this shape on a mask:
            int num_points = 4;
            int num_polygons = 1;
            int line_type = 8;
            cv::Mat mask(source_image.rows, source_image.cols, CV_8UC3, cv::Scalar(0,0,0));
            cv::fillPoly(mask, corner_list, &num_points, num_polygons, cv::Scalar(255, 255, 255), line_type);

            //Then simply compute the bitwise_and of the image and mask:
            cv::Mat result;
            cv::bitwise_and(source_image, mask, result);    //result now has the cropped image in it

            cv::namedWindow("cropped image", cv::WINDOW_FREERATIO);
            cv::imshow("cropped image", result);
            cv::waitKey(0);
            /

        }
        else
        {
            //Error message maybe?
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
}
*/


/*
void ClipScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        // With the left mouse button pressed, remember the position
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());

        // /
        // Create a selection square
        //m_selection = new QGraphicsPolygonItem();
        m_selection = new QGraphicsRectItem();
        m_selection->setBrush(QBrush(QColor(158,182,255,96)));
        m_selection->setPen(QPen(QColor(158,182,255,200),1));
        // Add it to the graphic scene
        addItem(m_selection);
        // /

        //IF REGION OF MOUSE PRESS IS WITHIN THE CURRENT M_SELECTION


    }

    QGraphicsScene::mousePressEvent(event);
}

void ClipScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_leftMouseButtonPressed)
    {
        // Form the selection area when moving with the mouse while pressing the LMB
        auto dx = event->scenePos().x() - m_previousPosition.x();
        auto dy = event->scenePos().y() - m_previousPosition.y();
        auto resultDelta = qMax(qAbs(dx), qAbs(dy));
        m_selection->setRect((dx > 0) ? m_previousPosition.x() : m_previousPosition.x() - resultDelta,
                           (dy > 0) ? m_previousPosition.y() : m_previousPosition.y() - resultDelta,
                           qAbs(resultDelta),
                           qAbs(resultDelta));
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void ClipScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() & Qt::LeftButton)
    {
        m_leftMouseButtonPressed = false;

        // When releasing the LMB, we form the cut off area
        QRect selectionRect = m_selection->boundingRect().toRect();
        clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
        delete m_selection;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
*/

/*
void ClipScene::undoPoints()
{
    pointsVector.clear();
    poly.clear();
    removeItem(m_selection);    //error, after 8 mouse presses without pressing delete points, then the polygon doesn't delete
    removeItem(ellipse_item[0]);
    removeItem(ellipse_item[1]);
    removeItem(ellipse_item[2]);
    removeItem(ellipse_item[3]);
    pressCount = 0;

    //pointsVector_cv.clear();

}
*/
