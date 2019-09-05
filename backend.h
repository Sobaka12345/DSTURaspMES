#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextCodec>
#include <QTextDecoder>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDate>

#include "pair.h"
#include "list_el.h"
#include "prep_el.h"

class Backend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QObject*> upDataList READ getUpData NOTIFY upDataChanged)
    Q_PROPERTY(QList<QObject*> downDataList READ getDownData NOTIFY downDataChanged)
    Q_PROPERTY(QList<QObject*> prepList READ getPreps NOTIFY prepListChanged)
    Q_PROPERTY(bool currentWeek READ getCurWeek NOTIFY curWeekChanged)
    Q_PROPERTY(bool firstLaunch READ isFirstLaunch NOTIFY launchChanged)
    QList<QObject*> upDataList;
    QList<QObject*> downDataList;
    QList<QObject*> prepList;
    bool currentWeek, firstLaunch;

    static void readWeekFile(QString file, QList<QObject*> & dest);

    QString getTime(QString & row);
    QString getDayInfo(QString & row, int & flag);
    int checkIdentical(ListEl*, ListEl*);
    void addToList(QList<QObject*> &, QMap<QString, QVariant>&);

    QString raspObj, prepObj, prepId;
    int raspMode;

public:
    explicit Backend(QObject *parent = nullptr);
    QList<QObject*> getUpData() { return upDataList; }
    QList<QObject*> getDownData() { return downDataList; }
    QList<QObject*> getPreps() { return prepList; }
    bool getCurWeek() const { return currentWeek; }
    bool isFirstLaunch() const { return firstLaunch; }
    ~Backend();

signals:
    void upDataChanged();
    void downDataChanged();
    void prepListChanged();
    void curWeekChanged();
    void launchChanged();


public slots:
    void searchPreps(QString txt);
    void loadId(QString txt, int mode);
    void setIndex(QString id);
    void loadSchedule(QNetworkReply * reply);
    void update();
private slots:
    void replyFinished(QNetworkReply * reply);
    void prepsReceived(QNetworkReply * reply);
};

#endif // BACKEND_H
