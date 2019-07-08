#include "todb.h"

#include <QFile>
#include <QDebug>
#include <QTimer>

#include <QtNetwork>

ToDB::ToDB(QObject *parent) : QObject(parent)
{

}

void ToDB::sendToDB(const QString &filePath, const QString &name, const QString &category, const QString &date_str, const QString &comment){
    QString date = date_str;
    date.replace(QString("."), QString("_"));

    qDebug() << name;
    qDebug() << category;
    qDebug() << date;
    qDebug() << comment;
    qDebug() << filePath;

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart namePart;
    namePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"name\""));
    namePart.setBody(QByteArray(name.toLatin1()));

    QHttpPart categoryPart;
    categoryPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"category\""));
    categoryPart.setBody(QByteArray(category.toLatin1()));

    QHttpPart datePart;
    datePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"date\""));
    datePart.setBody(QByteArray(date.toLatin1()));

    /*
    QHttpPart commentPart;
    commentPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"comment\""));
    commentPart.setBody(QByteArray(comment.toLatin1()));
    */

    QHttpPart imagePart;
    //____________________!!!!!!!!!!!!!!!!!!!!!____________________________________
    // USED THE NAME AND CATEGORY TO LABEL IMAGE. MAYBE SOMETHING MORE UNIQUE?
    QString fname = name + "_"  + category + "_" + date;
    QString n = "form-data; name=\"fileToUpload\"; filename=\"" + fname + ".jpg\"";

    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, (QVariant(n)));

    //QString path(ui->lineEdit_Path->text());
    QFile *file = new QFile(filePath);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(namePart);
    multiPart->append(categoryPart);
    // multiPart->append(commentPart);
    multiPart->append(datePart);
    multiPart->append(imagePart);


    const QUrl url = QUrl("http://fau5.atwebpages.com/welcome.php");
    const QNetworkRequest request(url);

    QNetworkAccessManager *manager;
    manager = new QNetworkAccessManager(this);
    QNetworkReply *reply4 = manager->post(request, multiPart);
    multiPart->setParent(reply4);

    connect(reply4, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(manager,SIGNAL(finished(QNetworkReply*)), this,SLOT(replyFinished(QNetworkReply*)));
    // connect(manager,SIGNAL(finished(QNetworkReply*)), this,SLOT(onfinish(QNetworkReply*)));
    //------------------------------------------------------------------------

}

void ToDB::replyFinished(QNetworkReply *reply){
    QByteArray data = reply->readAll();
    QString str(data);

    if(str.isEmpty())
        emit updateFinished("Error: Empty string"); // qDebug() << "Error: Empty string";
    else{
        emit updateFinished(str);// qDebug() << str; // "Data uploaded";
    }

    reply->deleteLater();
    reply = nullptr;

}

/*
void ToDB::onfinish(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QString str(data);

    if(str.isEmpty())
        qDebug() << "Error: Empty string";
    else{
        qDebug() << str; // "Data uploaded";
    }
}
*/
