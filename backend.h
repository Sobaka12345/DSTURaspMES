#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextCodec>
#include <QTextDecoder>

#include "pair.h"
#include "list_el.h"

class Backend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> upDataList READ getUpData NOTIFY upDataChanged)
    QList<QObject*> upDataList;

    Q_PROPERTY(QList<QObject*> downDataList READ getDownData NOTIFY downDataChanged)
    QList<QObject*> downDataList;

    QString getTime(QString & row);
    QString getDayInfo(QString & row, bool & flag);

public:
    explicit Backend(QObject *parent = nullptr);
    QList<QObject*> getUpData() { return upDataList; }
    QList<QObject*> getDownData() { return downDataList; }
    ~Backend();

signals:
    void upDataChanged();
    void downDataChanged();


public slots:
    void loadSchedule(QString gr_id);

private slots:
    void replyFinished(QNetworkReply * reply);
};

#endif // BACKEND_H
