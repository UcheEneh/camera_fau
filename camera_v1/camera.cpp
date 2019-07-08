/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "camera.h"
#include "ui_camera.h"
#include "imagesettings.h"
//#include "edit_info.h"
#include "cropping_dialog.h"

#include <QMediaService>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMessageBox>
#include <QPalette>
#include <QtWidgets>

#include <QScreen>
#include <QCameraInfo>

Q_DECLARE_METATYPE(QCameraInfo)

Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera),
    camera(nullptr),
    imageCapture(nullptr),
    isCapturingImage(false),
    applicationExiting(false)
{
    ui->setupUi(this);
    //ui->lastImagePreviewLabel->show();
    ui->label_Image_name->hide();

    // Setting for finding available camera devices:
    QActionGroup *videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraInfo.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraInfo));
        if (cameraInfo == QCameraInfo::defaultCamera())
            videoDeviceAction->setChecked(true);
        ui->menuDevices->addAction(videoDeviceAction);
    }
    connect(videoDevicesGroup, SIGNAL(triggered(QAction*)), SLOT(updateCameraDevice(QAction*)));
    //connect(ui->captureWidget, SIGNAL(currentChanged(int)), SLOT(updateCaptureMode()));
    setCamera(QCameraInfo::defaultCamera());
}

Camera::~Camera()
{
    delete imageCapture;
    delete camera;
}

//this sets up various connections between the user interface and the Camera class functionality using
//signals and slots. This is more or less what happens during: onPushButtonClicked... etc
void Camera::setCamera(const QCameraInfo &cameraInfo)
{
    //clear everything for clean coding
    delete imageCapture;
    delete camera;

    //create actions for setting up camera, image capture and video record
    //-------------------------------------------------------------------------------------------//
    camera = new QCamera(cameraInfo);

    connect(camera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(updateCameraState(QCamera::State)));
    connect(camera, SIGNAL(error(QCamera::Error)), this, SLOT(displayCameraError()));
    connect(ui->exposureCompensation, SIGNAL(valueChanged(int)), SLOT(setExposureCompensation(int)));

    QSize size = qApp->screens()[0]->size();
    ui->stackedWidget->setMinimumSize(size.width(), size.height() / 2);
    ui->viewfinder->setMinimumSize(size.width(), size.height() / 2);

    camera->setViewfinder(ui->viewfinder);  //main camera display
    updateCameraState(camera->state());
    updateLockStatus(camera->lockStatus(), QCamera::UserRequest);

    imageCapture = new QCameraImageCapture(camera);
    //necessary for getting image id and filename
    connect(imageCapture, SIGNAL(readyForCaptureChanged(bool)), this, SLOT(readyForCapture(bool)));
    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(processCapturedImage(int,QImage)));
    connect(imageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(imageSaved(int,QString)));
    connect(imageCapture, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this,
            SLOT(displayCaptureError(int,QCameraImageCapture::Error,QString)));
    connect(camera, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)),
            this, SLOT(updateLockStatus(QCamera::LockStatus,QCamera::LockChangeReason)));
    //-------------------------------------------------------------------------------------------//

    //Enable still image widget
    ui->captureWidget->setEnabled(camera->isCaptureModeSupported(QCamera::CaptureStillImage));
    //start camera
    //updateCaptureMode();
    // ui->viewfinder->show();
    camera->start();
}

//Activated when events: press focus key or camera buttons is performed
void Camera::keyPressEvent(QKeyEvent * event)
{
    if (event->isAutoRepeat())
        return;
    switch (event->key()) {
    case Qt::Key_CameraFocus:   //this simply displays viewfinder and lock the cam settings
        displayViewfinder();
        camera->searchAndLock();
        event->accept();
        break;
    case Qt::Key_Camera:    //either calls takeImage if capture mode is image mode (i.e CaptureStillImage )
                            //or calls record/stop (depending on current state) if video mode
        if (camera->captureMode() == QCamera::CaptureStillImage) {
            takeImage();    //take image
        }
        event->accept();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}

//to unlock the focus
void Camera::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    switch (event->key()) {
    case Qt::Key_CameraFocus:   //focus
        camera->unlock();
        break;
    default:    //for other keys beside the focus button
        QMainWindow::keyReleaseEvent(event);
    }
}

//rescale image and show on lastimagepreviewlabel for some secs
void Camera::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);
    //Image (img) taken from camera, scaled to fit screen viewfinder (which is same size as lastimagepreviewlabel)
    QImage scaledImage = img.scaled(ui->viewfinder->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);
    //image is set on lastimagepreviewlabel
    ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));

    // Display captured image from lastimagepreviewlabel on viewfinder for 7 secs
    displayCapturedImage();
    QTimer::singleShot(5000, this, SLOT(displayViewfinder()));
    //ui->label_Image_requestid->setText(QString::number(requestId));
}

void Camera::configureCaptureSettings() //on_actionSettings_triggered
{
    switch (camera->captureMode()) {
    case QCamera::CaptureStillImage:
        configureImageSettings();
        break;
    default:
        break;
    }
}

//this takes the changes made in imagesettings dialog and sets it as the camera new settings
void Camera::configureImageSettings()
{
    ImageSettings settingsDialog(imageCapture);
    settingsDialog.setImageSettings(imageSettings);
    if (settingsDialog.exec()) {
        imageSettings = settingsDialog.imageSettings();
        imageCapture->setEncodingSettings(imageSettings);
    }
}

void Camera::toggleLock()   //on_lockButton_clicked (focus)
{
    switch (camera->lockStatus()) {
    case QCamera::Searching:
    case QCamera::Locked:
        camera->unlock();
        break;
    case QCamera::Unlocked:
        camera->searchAndLock();
    }
}

//Focus status/colour... when button is pressed
void Camera::updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason reason)
{
    QColor indicationColor = Qt::black;
    switch (status) {
    case QCamera::Searching:
        indicationColor = Qt::yellow;
        ui->statusbar->showMessage(tr("Focusing..."));
        ui->lockButton->setText(tr("Focusing..."));
        break;
    case QCamera::Locked:
        indicationColor = Qt::darkGreen;
        ui->lockButton->setText(tr("Unlock"));
        ui->statusbar->showMessage(tr("Focused"), 2000);
        break;
    case QCamera::Unlocked:
        indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
        ui->lockButton->setText(tr("Focus"));
        if (reason == QCamera::LockFailed)
            ui->statusbar->showMessage(tr("Focus Failed"), 2000);
    }
    QPalette palette = ui->lockButton->palette();
    palette.setColor(QPalette::ButtonText, indicationColor);
    ui->lockButton->setPalette(palette);
}

void Camera::takeImage()    //on_takeImageButton_clicked
{
    isCapturingImage = true;
    imageCapture->capture();
}

//display image capturing error
void Camera::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    isCapturingImage = false;
}

void Camera::startCamera()  //on_actionStartCamera_triggered
{
    camera->start();
}

void Camera::stopCamera()   //on_actionStopCamera_triggered
{
    camera->stop();
}

/*
//this changes the camera mode if Image or Video tab is clicked (i.e if tab index is 0 or 1) on captureWidget
void Camera::updateCaptureMode()
{
    QCamera::CaptureModes captureMode = QCamera::CaptureStillImage;
    if (camera->isCaptureModeSupported(captureMode))
        camera->setCaptureMode(captureMode);
}
*/
//changes the state of the camera
//e.g settings can't be changed while camera is stopped
void Camera::updateCameraState(QCamera::State state)
{
    switch (state) {
    case QCamera::ActiveState:

        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        ui->captureWidget->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        /*
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        ui->captureWidget->setEnabled(true);
        ui->actionSettings->setEnabled(true);
        */
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        ui->captureWidget->setEnabled(false);
        ui->actionSettings->setEnabled(false);
    }
}

void Camera::setExposureCompensation(int index) //on_exposureCompensation_valueChanged
{
    camera->exposure()->setExposureCompensation(index*0.5);
}

//display camera selection error
void Camera::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera error"), camera->errorString());
}

void Camera::updateCameraDevice(QAction *action)
{
    setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

//using viewfinder label
void Camera::displayViewfinder()
{
    ui->stackedWidget->setCurrentIndex(0);
    // ui->stackedWidget->setCurrentWidget(ui->viewfinder);
    // ui->lastImagePreviewLabel->hide();
    // ui->viewfinder->show();
}

//using lastimagepreviewlabel
void Camera::displayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex(1);
    // ui->stackedWidget->setCurrentWidget(ui->lastImagePreviewLabel);

}

void Camera::readyForCapture(bool ready)
{
    ui->takeImageButton->setEnabled(ready);
}

void Camera::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    //Q_UNUSED(fileName);

    mFilename = fileName;       //PATH FOR IMAGES CAPTURED
    ui->label_Image_name->setText(mFilename);

    isCapturingImage = false;
    if (applicationExiting)
        close();
}

void Camera::closeEvent(QCloseEvent *event)
{
    if (isCapturingImage) {
        setEnabled(false);
        applicationExiting = true;
        event->ignore();
    } else {
        event->accept();
    }
}

/*
void Camera::on_pushButton_EditInfo_clicked()
{
    QString fname;
    if (ui->label_Image_name->text() == "Image path")
        fname = "";
    else
        fname = ui->label_Image_name->text();

    edit_info editInfo;
    editInfo.setData(fname);   //transfer data to edit_info and also plot image on graphics view
    editInfo.loadTable();      //Load table immediately
    editInfo.setModal(true);    //opens new window
    editInfo.setWindowTitle("Edit Info");
    editInfo.exec();
}
*/

void Camera::on_pushButton_Crop_clicked()
{
    QString fname;
    if (ui->label_Image_name->text() == "Image path")
        fname = "";
    else
        fname = ui->label_Image_name->text();

    cropping_Dialog crop;
    crop.setData(fname);   //transfer data to edit_info and also plot image on graphics view
    crop.setModal(true);    //opens new window
    crop.setWindowTitle("Crop");
    crop.exec();
}
