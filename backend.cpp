#include "backend.h"

void Backend::readWeekFile(QString file, QList<QObject *> &dest)
{
    QFile upFile(file);
    if(!upFile.open(QIODevice::ReadWrite))
        exit(-69);
    QString upWeek = upFile.readAll();
    upFile.close();
    int count = upWeek.count('~');
    for(int i = 0; i < count; i++)
    {
        int index = upWeek.indexOf('|');
        QString time = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);
        index = upWeek.indexOf('|');
        QString info = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);
        index = upWeek.indexOf('|');
        QString prep = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);
        index = upWeek.indexOf('|');
        QString aud = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);
        index = upWeek.indexOf('|');
        QString weekType = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);
        index = upWeek.indexOf('~');
        QString day = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);

        dest.append(new ListEl(info,time,day,prep,aud,weekType.toInt()));
    }
}

Backend::Backend(QObject *parent) : QObject(parent), loadFlag(false)
{
    readWeekFile("upper.week", upDataList);
    readWeekFile("bottom.week", downDataList);

    QFile weekFile("week.week");
    if(weekFile.exists())
    {
        firstLaunch = false;
        weekFile.open(QIODevice::ReadWrite);
        QString w = weekFile.readAll();
        int type = w.left(1).toInt();
        currentWeek = (bool)type;
        w = w.remove(0,2);
        int wDay = w.left(1).toInt();
        w = w.remove(0,2);
        w = w.remove(w.indexOf("~"), 1);
        int yDay = w.toInt();
        int curYDay = QDate::currentDate().dayOfYear();
        int maxDay = QDate::isLeapYear(QDate::currentDate().year() - 1) ? 366 : 365;
        if(curYDay < yDay)
            curYDay += maxDay;
        weekFile.seek(0);
        if((8 - wDay) <= curYDay - yDay)
        {
            qDebug() << ((((curYDay - yDay) / 7) % 2 == 0) && (((curYDay - yDay) - 7 *((curYDay - yDay) / 7)) >= (8 - wDay)));
            qDebug() << "\n" << ((((curYDay - yDay) / 7) % 2 != 0) && (((curYDay - yDay) - 7*((curYDay - yDay) / 7)) < (8 - wDay)));
            if(((((curYDay - yDay) / 7) % 2 == 0) && (((curYDay - yDay) - 7 *((curYDay - yDay) / 7)) >= (8 - wDay))) ||
                    ((((curYDay - yDay) / 7) % 2 != 0) && (((curYDay - yDay) - 7*((curYDay - yDay) / 7)) < (8 - wDay))))
                currentWeek = !type;

            QTextStream wfStream(&weekFile);
            wfStream << currentWeek << "|"
                     << QDate::currentDate().dayOfWeek() << "|"
                     << QDate::currentDate().dayOfYear() << "~";
        }
        weekFile.close();
    } else {
       firstLaunch = true;
       currentWeek = false;
    }
    emit curWeekChanged();
    emit launchChanged();
}

void Backend::loadId(QString txt, int mode)
{
    raspObj = txt;
    raspMode = mode;
    QString url("https://edu.donstu.ru/api/Raspgrouplist");

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(loadSchedule(QNetworkReply*)));

    mgr->get(QNetworkRequest(QUrl(url)));

}

void Backend::setIndex(QString id)
{
    prepId = id;
    raspMode = 2;
}

void Backend::loadSchedule(QNetworkReply * reply)
{
    QString url("https://edu.donstu.ru/api/Rasp?");
    if(raspMode == 1)
    {
        QString groups = reply->readAll();
        int index = groups.indexOf(raspObj.toUpper(), Qt::CaseSensitivity::CaseInsensitive);
        if(index == -1)
            return;
        else
        {
            int i;
            for(i = index + raspObj.length(); !groups[i].isDigit(); i++);
            QString gr_id;
            while(groups[i].isDigit())
                gr_id += groups[i++];
            url += "idGroup=" + gr_id;
        }
    }
    else if(raspMode == 2 && prepId != "")
    {
       url += "idPrepodLine=" + prepId;
    }
    else return;


    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    mgr->get(QNetworkRequest(QUrl(url)));
}

void Backend::update()
{
        QFile infFile("info.week");
        if(!infFile.open(QIODevice::ReadOnly))
            return;
        QString inf = infFile.readAll();
        raspMode = inf.left(1).toInt();
        if(raspMode == 1)
        {
            raspObj = inf.remove(0,2);
            QString url("https://edu.donstu.ru/api/Raspgrouplist");

            QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
            connect(mgr, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(loadSchedule(QNetworkReply*)));

            mgr->get(QNetworkRequest(QUrl(url)));
        }
        else if(raspMode == 2)
        {
            prepId = inf.remove(0,2);
            loadSchedule(nullptr);
        }
        else return;
}

int Backend::checkIdentical(ListEl * el1, ListEl* el2)
{
    int out = 0;
        if(*el1 == *el2){
            out = 1;
        }
    if(out == 1 && (el1->getAud() == el2->getAud()))
        out = 2;
    return out;
}

void Backend::addToList(QList<QObject *> &list, QMap<QString, QVariant> & map)
{
    auto insert = new ListEl(map.find("дисциплина")->toString() + "<br>",
                             map.find("начало")->toString() + " - " + map.find("конец")->toString(),
                             map.find("дата")->toString() == "" ? map.find("день_недели")->toString() : map.find("дата")->toString().left(10),
                             raspMode == 1 ? map.find("преподаватель")->toString() + "<br>" : map.find("группа")->toString() + "<br>",
                             map.find("аудитория")->toString(),
                             map.find("типНедели")->toString().toInt()
                             );
    int mode;
    if(!list.empty() && (mode = checkIdentical((ListEl*)list.back(), insert)))
    {
        ListEl * prev = (ListEl*)list.back();
        switch(mode)
        {
        case 1: prev->setAud(prev->getAud() + ", " + insert->getAud());
        case 2:
            prev->setPrep(prev->getPrep().replace("<br>",", ") + insert->getPrep());
            delete insert;
            break;
        }
    } else {
       list.append(insert);
    }
}


void Backend::replyFinished(QNetworkReply * reply)
{
    if(reply->error() != QNetworkReply::NoError)
            return;
    if(!upDataList.empty())
    {
        for(auto x : upDataList)
            delete x;
        upDataList.clear();
    }
    if(!downDataList.empty())
    {
        for(auto x : downDataList)
            delete x;
        downDataList.clear();
    }
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError){
        qDebug() << jsonError.errorString();
        }

    QMap<QString, QVariant> list = doc.toVariant().toMap();
    auto kek = list.find("data")->toMap();
    auto rasp = kek.find("rasp")->toList();

    for(auto x : rasp)
    {
        auto map = x.toMap();
        switch(map.find("типНедели")->toInt())
        {
        case 1:
            addToList(upDataList, map);
            break;
        case 2:
            addToList(downDataList, map);
            break;
        case 0:
            addToList(upDataList, map);
            addToList(downDataList, map);
        }
    }
    emit upDataChanged();
    emit downDataChanged();

    QFile upFile("upper.week");
    QFile downFile("bottom.week");
    if(!upFile.open(QIODevice::WriteOnly) ||
            !downFile.open(QIODevice::WriteOnly))
        exit(-69);
    QTextStream upStream(&upFile);
    QTextStream downStream(&downFile);

    for(auto & x : upDataList)
    {
        auto xp = (ListEl*)x;
        upStream << xp->getTime() << "|";
        upStream << xp->getInfo() << "|";
        upStream << xp->getPrep() << "|";
        upStream << xp->getAud() << "|";
        upStream << xp->getWeekType() << "|";
        upStream << xp->getDay() << "~";
    }
    upFile.close();
    for(auto & x : downDataList)
    {
        auto xp = (ListEl*)x;
        downStream << xp->getTime() << "|";
        downStream << xp->getInfo() << "|";
        downStream << xp->getPrep() << "|";
        downStream << xp->getAud() << "|";
        downStream << xp->getWeekType() << "|";
        downStream << xp->getDay() << "~";
    }
    downFile.close();

    QFile infFile("info.week");
    infFile.open(QIODevice::WriteOnly);
    QTextStream infStream(&infFile);
    infStream << raspMode << "|" << (raspMode == 1 ? raspObj : prepId);
    infFile.close();


    auto info = kek.find("info")->toMap();
    QFile weekFile("week.week");
    weekFile.open(QIODevice::WriteOnly);
    QTextStream weekStream(&weekFile);
    QDate date(QDate::currentDate());
    currentWeek = info.find("curNumNed")->toInt() % 2;
    weekStream << (int)currentWeek << "|"
               << date.dayOfWeek() << "|"
               << date.dayOfYear() << "~";
    weekFile.close();
    firstLaunch = false;
    emit curWeekChanged();
    emit launchChanged();

}

QString Backend::getTime(QString &row)
{
    int chopIndex = row.indexOf("</td>");
    QString slice = row.mid(0, chopIndex);
    row = row.remove(0, chopIndex + 5);
    slice = slice.remove(0, slice.indexOf(">") + 1);
    return slice;
}

QString Backend::getDayInfo(QString &row, int &flag)
{
    int chopIndex = row.indexOf("</td>");
    QString slice = row.mid(0, chopIndex);
    row = row.remove(0, chopIndex + 5);
    if(!slice.contains("rowspan"))
        flag = 1;
    else if(slice.contains("br"))
        flag = 2;
    else
        flag = 0;
    slice = slice.remove(0, slice.indexOf(">") + 1);

    return slice;
}

Backend::~Backend()
{
    for(auto x : upDataList)
        delete x;

    for(auto x: downDataList)
        delete x;

    for(auto x : prepList)
        delete x;
}

void Backend::searchPreps(QString txt)
{
    if(txt == "")
        return;
    loadFlag = true;
    emit load();
    prepObj = txt;
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(prepsReceived(QNetworkReply*)));

    mgr->get(QNetworkRequest(QUrl("https://edu.donstu.ru/api/raspprepodlist")));
}

void Backend::prepsReceived(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
       loadFlag = false;
       emit load();
       return;
    }
    if(!prepList.empty())
    {
        for(auto x : prepList)
            delete x;
        prepList.clear();
    }


    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(),&jsonError);
    if (jsonError.error != QJsonParseError::NoError){
        qDebug() << jsonError.errorString();
        }
    QMap<QString, QVariant> list = doc.toVariant().toMap();
    auto data = list.find("data")->toList();
    for(auto & x: data)
    {
        auto map = x.toMap();
        QString name = map.find("name")->toString();
        if(name.contains(prepObj, Qt::CaseInsensitive))
        {
            prepList.append(new PrepEl(name + "  " + map.find("kaf")->toString(), map.find("id")->toString()));
        }
    }
    loadFlag = false;
    emit load();
    emit prepListChanged();
}
