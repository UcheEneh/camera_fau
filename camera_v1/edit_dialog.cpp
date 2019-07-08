#include "edit_dialog.h"
#include "ui_edit_dialog.h"

#include <QFile>
#include <QDebug>
#include <QTimer>

#include <QScreen>
#include <QtNetwork>

Edit_Dialog::Edit_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit_Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);     //better for android
    //this->showFullScreen();

    connect(ui->pushButton_Back, &QPushButton::clicked, this, &Edit_Dialog::close);
    connect(ui->pushButton_Save, &QPushButton::clicked, this, &Edit_Dialog::insertItem);
    ui->dateEdit->setMinimumSize(200, 50);
}

Edit_Dialog::~Edit_Dialog()
{
    delete ui;
}

void Edit_Dialog::setData(const QPixmap *pic, QString filepath) {
    QPixmap pict = *pic;
    //pict.scaled(ui->label_Image->width(), ui->label_Image->height(),
                //Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //pict.scaledToWidth(ui->label_6->width(), Qt::SmoothTransformation)
    //ui->label_Image->setPixmap(pict);

    QSize size = qApp->screens()[0]->size();
    ui->label_Image->setMinimumSize(size.width()-50, (size.height()/2)-50);

    int w = ui->label_Image->width();
    int h = ui->label_Image->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label_Image->setPixmap(pict.scaled(w,h,Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
    path = filepath;
}

//----------------------------- INSERT INTO DB / SERVER -------------------------------------
void Edit_Dialog::insertItem()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"name\""));
    namePart.setBody(QByteArray(ui->lineEdit_Name->text().toLatin1()));

    QHttpPart categoryPart;
    categoryPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"category\""));
    categoryPart.setBody(QByteArray(ui->lineEdit_Category->text().toLatin1()));

    QHttpPart datePart;
    datePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"date\""));
    datePart.setBody(QByteArray(ui->dateEdit->text().toLatin1()));

    QHttpPart imagePart;
    //____________________!!!!!!!!!!!!!!!!!!!!!____________________________________
    // USED THE NAME AND CATEGORY TO LABEL IMAGE. MAYBE SOMETHING MORE UNIQUE?
    QString fname = ui->lineEdit_Name->text() + "_"  + ui->lineEdit_Category->text() +
                    "_" + ui->dateEdit->text();
    QString n = "form-data; name=\"fileToUpload\"; filename=\"" + fname + ".jpg\"";

    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, (QVariant(n)));

    //QString path(ui->lineEdit_Path->text());
    QFile *file = new QFile(path);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(namePart);
    multiPart->append(categoryPart);
    multiPart->append(datePart);
    multiPart->append(imagePart);

    const QUrl url = QUrl("http://fau5.atwebpages.com/welcome.php");
    const QNetworkRequest request(url);

    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);
    QNetworkReply *reply4 = manager->post(request, multiPart);
    multiPart->setParent(reply4);

    connect(reply4, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(manager,SIGNAL(finished(QNetworkReply*)), this,SLOT(onfinish(QNetworkReply*)));
    connect(manager,SIGNAL(finished(QNetworkReply*)), this,SLOT(replyFinished(QNetworkReply*)));

    //------------------------------------------------------------------------

}

void Edit_Dialog::onfinish(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString str(data);

    if(str.isEmpty())
        qDebug() << "Error: Empty string";
    else{
        qDebug() << str;
        //QMessageBox::information(this,"page",str,"ok");
        webString = str;    //webString.append(str);  //using append just makes everything keep showing on textEdit

        ui->label_Status->setText("Uploaded into Database");
        //ui->textEdit->clear();  //always clean when new webpage is opened
        //ui->textEdit->append(webString);
    }
}

void Edit_Dialog::replyFinished(QNetworkReply *reply){
    reply->deleteLater();
    reply = nullptr;
    qDebug() << "Data uploaded";
}
