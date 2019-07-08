#ifndef EDIT_DIALOG_H
#define EDIT_DIALOG_H

#include <QDialog>

#include <QFileInfo>
#include <QMessageBox>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "ClipScene.h"

namespace Ui {
class Edit_Dialog;
}

class Edit_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Edit_Dialog(QWidget *parent = nullptr);
    ~Edit_Dialog();

    void setData(const QPixmap *pic, QString path);

private slots:
    void onfinish(QNetworkReply *);
    void insertItem(); //QString name, QString category, QString date, QByteArray inByteArray);
    void replyFinished(QNetworkReply *);
    void slotError(QNetworkReply::NetworkError e) {
            qDebug() << "slotError" << e ;
    }

private:
    Ui::Edit_Dialog *ui;

    //QNetworkAccessManager* qnam;
    //QNetworkReply *reply2;
    //QUrl url;
    QString path;
    QString webString;
};

#endif // EDIT_DIALOG_H
