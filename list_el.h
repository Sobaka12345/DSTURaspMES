#ifndef LIST_EL_H
#define LIST_EL_H

#include <QObject>
#include <QString>

class ListEl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString info READ getInfo  NOTIFY infoChanged)
    Q_PROPERTY(QString day READ getDay  NOTIFY dayChanged)
    Q_PROPERTY(QString time READ getTime NOTIFY timeChanged)


    QString _info, _time;
    int _day;
public:
    ListEl(QString info, QString time, int day) :
        _info(info),
        _time(time),
        _day(day)
    {
    }

    QString getTime() { return _time; }
    int getDayNumber() { return _day; }
    QString getDay()
    {
        switch(_day)
        {
        case 0: return "Понедельник";
        case 1: return "Вторник";
        case 2: return "Среда";
        case 3: return "Четверг";
        case 4: return "Пятница";
        case 5: return "Суббота";
        default: return "";
        }
    }
    QString getInfo() { return _info; }

signals:
    void infoChanged();
    void dayChanged();
    void timeChanged();
};

#endif // LIST_EL_H
