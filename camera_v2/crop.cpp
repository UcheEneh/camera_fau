#include "crop.h"

// #include <QtWidgets/QGraphicsPixmapItem>
// #include <QtWidgets/QGraphicsRectItem>

#include <QBuffer>
#include <QVariant>
#include <QImage>
#include <QPixmap>
#include <QtDebug>

crop::crop(/*QQuickItem *parent*/QObject *parent) : /*QQuickPaintedItem(parent)*/ QObject(parent) // QGraphicsScene(parent)
{

}

// getImage returns a QImage which can be passed to C++ for further processing.
void crop::applyCrop(const QString &filePath, const int &imageWidth, const int &imageHeight, const int &cropWidth, const int &cropHeight, const int &cropX, const int &cropY)
{
    QPixmap img_pm(filePath);

    // Aspect ratio and smooth commented out to fit image from QML
    img_pm = img_pm.scaled(QSize(imageWidth, imageHeight)/*,
                           Qt::KeepAspectRatio, Qt::SmoothTransformation*/);
    QRect boundingBox(cropX, cropY, cropWidth, cropHeight);
    QPixmap cropImg = img_pm.copy(boundingBox);

    // check if file exists and if yes, delete then replace (or just replace depending on how this works)

    QString new_file = filePath;
    QString str2 = ".";
    std::size_t found = new_file.indexOf(str2);
    if (found != std::string::npos)
        new_file.replace(found, found+3, ""); //str2.length()+3, ""); //delete the .png and replace below
    new_file = new_file + "_cropped.jpg";

    cropImg.save(new_file);
    qDebug() << "Saved to: " << new_file;
    emit clippedImage(new_file);

    /*
    if (m_currentImageItem)
    {
        this->removeItem(m_currentImageItem);
    }
    //m_currentImageItem = new QGraphicsPixmapItem(QPixmap(filePath));

    QPixmap loaded_img(filePath);
    img_pm = loaded_img;

    //Convet from QPixmap to QImage and resize (FOR RESIZING DRAWING POINT IN EWIDGET?)
    image = img_pm.toImage();

    // CHANGE THIS TO THE IMAGE GT FROM QML
    m_currentImageItem = new QGraphicsPixmapItem();
    m_currentImageItem->setPixmap(img_pm.scaled(QSize(imageWidth, imageHeight),    //try to make this not hardcoded
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //Add it to the graphic scene
    addItem(m_currentImageItem);

    //Add graphics item if graphics scene has image
    m_selection = new QGraphicsRectItem();
    //m_selection->setBrush(QBrush(QColor(158,182,255,96)));
    //m_selection->setPen(QPen(QColor(158,182,255,200),1));
    //m_selection->setRect(0, 0, 70, 70);
    m_selection->setRect(cropWidth, cropHeight, cropX, cropY);

    // Add it to the graphic scene
    addItem(m_selection);

    QRect selectionRect = m_selection->boundingRect().toRect();
    clippedImage(m_currentImageItem->pixmap().copy(selectionRect));
    delete m_selection;
    */
}
