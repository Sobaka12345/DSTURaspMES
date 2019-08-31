#include "backend.h"

Backend::Backend(QObject *parent) : QObject(parent)
{
    QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder * decoder = new QTextDecoder(codec);

    QFile upFile("upper.week");
    if(!upFile.open(QIODevice::ReadOnly))
        exit(-69);
    QString upWeek = decoder->toUnicode(upFile.readAll());
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
        index = upWeek.indexOf('~');
        QString day = upWeek.left(index);
        upWeek = upWeek.remove(0, index + 1);
        upDataList.append(new ListEl(info, time, day.toInt()));
    }

    QFile downFile("bottom.week");
    if(!downFile.open(QIODevice::ReadOnly))
        exit(-69);
    QString downWeek = decoder->toUnicode(downFile.readAll());
    downFile.close();
    count = downWeek.count('~');
    for(int i = 0; i < count; i++)
    {
        int index = downWeek.indexOf('|');
        QString time = downWeek.left(index);
        downWeek = downWeek.remove(0, index + 1);
        index = downWeek.indexOf('|');
        QString info = downWeek.left(index);
        downWeek = downWeek.remove(0, index + 1);
        index = downWeek.indexOf('~');
        QString day = downWeek.left(index);
        downWeek = downWeek.remove(0, index + 1);
        upDataList.append(new ListEl(info, time, day.toInt()));
    }

    delete decoder;
}

void Backend::loadSchedule(QString gr_id)
{
    QString url("https://edu.donstu.ru/Rasp/RaspFull.aspx?group=");
    url += gr_id;

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    mgr->get(QNetworkRequest(QUrl(url)));
}

void Backend::replyFinished(QNetworkReply * reply)
{
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
    QTextCodec * codec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder * decoder = new QTextDecoder(codec);

    bool readNextLine = false;
    QVector<QString> data;

    while (!reply->atEnd()) {
           QString str = decoder->toUnicode(reply->readLine());
           if(readNextLine)
           {
               data.push_back(str);
               readNextLine = false;
           }
           if(str.contains("<tr id=\"ctl00_MainContent_RaspTable_DXDataRow"))
               readNextLine = true;
    }

    QVector<Pair*> pairs;
    bool twice = false;
    QString * temp = nullptr;
    for(auto it = data.begin(); it != data.end(); it++)
    {
        int count = it->count("dx-wrap dxgv");
        for(int c = 0; c < count; c++)
        {
            int i, index = it->indexOf("class");
            for(i = index; (*it)[i] != '>' && i != it->indexOf("rowspan",i); i++);
            *it = it->remove(index, i - index);
        }
        bool flag = false;
        if(!twice)
        {
            if(it->mid(it->indexOf('<'), 15).contains("rowspan")) {
                pairs.append(new Pair(getTime(*it)));
                getDayInfo(*it, flag);
                twice = true;
                temp = it;
            } else {
                pairs.append(new Pair(getTime(*it)));
                getDayInfo(*it, flag);
                for (int i = 0; i < 6; i++)
                    pairs.last()->pushDay(getDayInfo(*it, flag), "", i);
            }
        } else {
            getDayInfo(*it, flag);
            for (int i = 0; i < 6; i++)
            {
                QString upper = getDayInfo(*temp, flag);
                if(flag)
                    pairs.last()->pushDay(upper, getDayInfo(*it, flag), i);
                else
                    pairs.last()->pushDay(upper, "", i);
            }
            twice = false;
        }

    }

    QVector<QList<ListEl*>> inter(6);
    for(auto x : pairs)
    {
        auto up = x->getUpWeek();
        for(int i = 0; i < 6; i++)
        {
            if(up[i] != "&nbsp;" && up[i] != "")
                inter[i].append(new ListEl(up[i], x->getTime(), i));
        }
    }
    QFile upFile("upper.week");
    if(!upFile.open(QIODevice::WriteOnly))
        exit(-69);
    QTextStream stream(&upFile);
    for(int i = 0; i < 6; i++)
    {
        for(auto x : inter[i])
        {
            upDataList.append(x);
            stream << x->getTime() << '|';
            stream << x->getInfo() << '|';
            stream << x->getDayNumber() << '~';
        }
        inter[i].clear();
    }
    upFile.close();

    QFile downFile("bottom.week");
    if(!downFile.open(QIODevice::WriteOnly))
        exit(-69);
    stream.setDevice(&downFile);
    for(auto x : pairs)
    {
        auto down = x->getDownWeek();
        for(int i = 0; i < 6; i++)
        {
            if(down[i] != "&nbsp;" && down[i] != "")
                inter[i].append(new ListEl(down[i], x->getTime(), i));
        }
    }
    for(int i = 0; i < 6; i++)
    {
        for(auto x : inter[i])
        {
            downDataList.append(x);
            stream << x->getTime() << '|';
            stream << x->getInfo() << '|';
            stream << x->getDayNumber() << '~';
        }
        inter[i].clear();
    }
    downFile.close();

    emit upDataChanged();

    delete decoder;
    for(auto x: pairs)
        delete x;
}

QString Backend::getTime(QString &row)
{
    int chopIndex = row.indexOf("</td>");
    QString slice = row.mid(0, chopIndex);
    row = row.remove(0, chopIndex + 5);
    slice = slice.remove(0, slice.indexOf(">") + 1);
    return slice;
}

QString Backend::getDayInfo(QString &row, bool &flag)
{
    int chopIndex = row.indexOf("</td>");
    QString slice = row.mid(0, chopIndex);
    row = row.remove(0, chopIndex + 5);
    if(!slice.contains("rowspan"))
        flag = true;
    else
        flag = false;
    slice = slice.remove(0, slice.indexOf(">") + 1);
    return slice;
}

Backend::~Backend()
{
    for(auto x : upDataList)
        delete x;

    for(auto x: downDataList)
        delete x;
}
