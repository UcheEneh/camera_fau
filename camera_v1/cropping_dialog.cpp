#include "cropping_dialog.h"
#include "ui_cropping_dialog.h"

#include <QtWidgets>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QScreen>


#include <QCameraInfo>

//#include <QMouseEvent>
#include "edit_dialog.h"

cropping_Dialog::cropping_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cropping_Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);     //better for android
    //this->showFullScreen();

    // use screen size for the image region
    size = qApp->screens()[0]->size();
    ui->m_graphicsView->setMinimumSize(size.width(), size.height() / 2);

    m_clipScene = new ClipScene(this);
    ui->m_graphicsView->setScene(m_clipScene);  //set Graphics View as a clipping region

    connect(ui->pushButton_Back, &QPushButton::clicked, this, &cropping_Dialog::close);
    // connect(ui->pushButton_Add, &QPushButton::clicked, this, &cropping_Dialog::onAddFile);
    // connect(ui->pushButton_Save, &QPushButton::clicked, this, &cropping_Dialog::onSaveImage);
    connect(m_clipScene, &ClipScene::clippedImage, this, &cropping_Dialog::onClippedImage);
    // connect(ui->pushButton_Revert, &QPushButton::clicked, this, &cropping_Dialog::undoCrop);
    connect(ui->pushButton_Crop, &QPushButton::clicked, this, &cropping_Dialog::crop);

    //everytime a box is selected and the mouse is released, clippedImage signal is
    //activated which copies the image in the selected box/region onto the clipLabel here
}

cropping_Dialog::~cropping_Dialog()
{
    delete ui;
}

//---------------------------- SET DATA FROM CAMERA ----------------------------------------------//
void cropping_Dialog::setData(QString &filename) {
    ui->lineEdit_ImageName->setText(filename);
    global_fname = filename;
    //automatically open original image in graphics view (that has been changed to a clipping region)
    if (ui->lineEdit_ImageName->text().isEmpty())
        return;
    else{
        m_clipScene->setImage(ui->lineEdit_ImageName->text(), size.width(), size.height() / 2);
        file = ui->lineEdit_ImageName->text();
    }
}

//-------------------------------- CROPPING / CLIP SCENE ----------------------------------------//
void cropping_Dialog::onClippedImage(const QPixmap& pixmap)
{
    //ui->label_Image->setPixmap(pixmap);
    /*
    // get label dimensions
    int w = ui->label_Image->width();
    int h = ui->label_Image->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label_Image->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    */

    imageLabel.setPixmap(pixmap);
    pmap = pixmap;
}

void cropping_Dialog::crop()  //Just basic replotting, nothing hardcore here
{
    m_clipScene->applyCrop();
    cropPerformed = true;

    //############################
    //EDIT FILE PATH TO INCLUDE _cropped
    QString new_file = file;
    QString str2 = ".";
    std::size_t found = new_file.indexOf(str2);
    if (found != std::string::npos)
        new_file.replace(found, found+3, ""); //str2.length()+3, ""); //delete the .png and replace below
    new_file = new_file + "_cropped.jpg";
    //############################

    qDebug() << "new file path: " << new_file;

    //GET IMAGE AND SAVE WITH NEW NAME PATH
    // QString imagePath = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                     // new_file,
                                                     // tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    //SAVE IMAGE
    QImage *imageObject = new QImage();
    *imageObject = pmap.toImage();

    if (pmap.isNull()){
        QMessageBox::warning(nullptr, QString("Warning"),
                             QString("No image selected"));
    }
    else if(imageObject->save(new_file)){
        Edit_Dialog edit;
        edit.setData(&pmap, new_file); //, new_file);
        edit.setModal(true);
        edit.setWindowTitle("Edit Info");
        edit.exec();
    }

    //clear graphicsview and reload original image
    m_clipScene->setImage(file, size.width(), size.height() / 2);
    //and change back imagepath in lineEdit_ImageName
    ui->lineEdit_ImageName->setText(file);
    cropPerformed = false;
}

void cropping_Dialog::undoCrop()  //Just basic replotting, nothing hardcore here
{

    if(cropPerformed){   //after save is clicked, cropPerformed = true
        //m_clipScene->undoPoints();  //maybe leave this here as well

        //Warning if undo clicked
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Undo"),
                                     tr("Revert back to original image? \n"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes);
        if (reply == QMessageBox::Yes){
            //clear graphicsview and reload original image
            m_clipScene->setImage(file, size.width(), size.height() / 2);

            //and change back imagepath in lineEdit_ImageName
            ui->lineEdit_ImageName->setText(file);
            // ui->label_Image->clear();
        }
    }
    else{
        QMessageBox::warning(this, tr("Undo"),
                             tr("No changes have been made to an image"),
                             QMessageBox::Close);
    }
    cropPerformed = false;
}

/*
void cropping_Dialog::deletePoints()  //Just basic replotting, nothing hardcore here
{
    if(m_clipScene->polygonAdded)   //specific for polygon added
        m_clipScene->undoPoints();
}
*/

/*
void cropping_Dialog::onAddFile()
{
    //Crop another image
    QString imagePath = QFileDialog::getOpenFileName(this, "Open Image File", QString(), tr("Image (*.png *.jpg)"));
    m_clipScene->setImage(imagePath, size.width(), size.height() / 2);   //opens the image in the graphics view that has been changed to a clipping region

    image = m_clipScene->returnImage();  //get image from clipscene and use for QPaint (REMOVED)!!!!
    file = imagePath;   //set file as image path to be used later for naming the cropped image
}
*/

/*
void cropping_Dialog::onSaveImage(){

    //############################
    //EDIT FILE PATH TO INCLUDE _cropped
    new_file = file;
    QString str2 = ".";
    std::size_t found = new_file.indexOf(str2);
    if (found != std::string::npos)
        new_file.replace(found, found+3, ""); //str2.length()+3, ""); //delete the .png and replace below
    new_file = new_file + "_cropped.jpg"; //QDir::currentPath() + new_file + "_cropped.png";
    //############################

    //GET IMAGE AND SAVE WITH NEW NAME PATH
    const QPixmap *pic(ui->label_Image->pixmap());
    QString imagePath = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                     new_file,
                                                     tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));
    //SAVE IMAGE
    QImage *imageObject = new QImage();
    *imageObject = pic->toImage();

    if(imageObject->save(imagePath)){    //if image is saved:
        cropPerformed = true;
        ///
        //clear graphicsview and load cropped image onto it
        m_clipScene->setImage(imagePath);
        //and change imagepath in lineEdit_ImageName
        ui->lineEdit_ImageName->setText(imagePath);
        ///
        if (ui->label_Image->isEnabled()){
            const QPixmap *pic(ui->label_Image->pixmap());
            Edit_Dialog edit;
            edit.setData(pic, new_file);
            edit.setModal(true);
            edit.setWindowTitle("Edit Info");
            edit.exec();
        }
        else{
            QMessageBox::warning(nullptr, QString("Warning"),
                                 QString("Cropping not performed. No image selected"));
        }
    }
}
*/
