#ifndef TODB_H
#define TODB_H

#include <QObject>

#include <QFileInfo>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

class ToDB : public QObject
{
    Q_OBJECT

    // Q_PROPERTY(QString info READ info WRITE setInfo NOTIFY replyFinished)
    // Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY imageChanged)

public:
    explicit ToDB(QObject *parent = nullptr);

    Q_INVOKABLE void sendToDB(const QString &filePath, const QString &name, const QString &category, const QString &date_str, const QString &comment);

signals:
    void updateFinished(const QString &updateMsg);
    void networkError(const QString &errorMsg);


public slots:
    // void onfinish(QNetworkReply *);
    void slotError(QNetworkReply::NetworkError e) {
        qDebug() << "slotError" << e ;
        QString errStr("Network Error");
        emit networkError(errStr);
    }
    void replyFinished(QNetworkReply *);

private:
    // void dataSent(QString output);
};

#endif // TODB_H
