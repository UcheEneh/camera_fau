
#ifndef CROP_H
#define CROP_H

#include <QObject>

// #include <QQuickPaintedItem>
// #include <QQuickItem>
// #include <QPainter>
#include <QImage>


// #include <QtWidgets/QGraphicsScene>

// class QGraphicsPixmapItem;  //show image
// class QGraphicsRectItem;    //choose crop region

class crop : public QObject //, public QQuickPaintedItem // QGraphicsScene
{
    Q_OBJECT

    // Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
public:
    explicit crop(QObject *parent = nullptr);  // (QQuickItem *parent = nullptr);

    Q_INVOKABLE void applyCrop(const QString &filePath, const int &imageWidth, const int &imageHeight, const int &cropWidth, const int &cropHeight, const int &cropX, const int &cropY);
    // Q_INVOKABLE void getImage(QVariant var);//, const int &imageWidth, const int &imageHeight, const int &cropWidth, const int &cropHeight, const int &cropX, const int &cropY);
    // void applyCrop();

    /*
    Q_INVOKABLE void setImage(const QImage &image);
    void paint(QPainter *painter);
    QImage image() const;
    */

signals:
    void clippedImage(const QString &new_file);   //A signal that transmits the cut out image area to the application window to show it in the label

    // void imageChanged();

public slots:
    // void updateImage(const QImage &image);

    // void imageChanged();

private:
    // QGraphicsPixmapItem* m_currentImageItem {nullptr};
    // QGraphicsRectItem* m_selection          {nullptr};
    // QPixmap img_pm;

    QImage cropImg;
    QImage origImage;
};

#endif // CROP_H

