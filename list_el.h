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
    Q_PROPERTY(QString aud READ getAud NOTIFY audChanged)
    Q_PROPERTY(QString prep READ getPrep NOTIFY prepChanged)
    Q_PROPERTY(QString para READ getPara NOTIFY paraChanged)

    QString _info, _time, _aud, _prep, _day;
    int _weekType;
public:
    bool operator==(ListEl & obj)
    {
        return (_info == obj.getInfo() &&
                _time == obj.getTime() &&
                _day == obj.getDay() &&
                _weekType == obj.getWeekType());
    }

    ListEl(QString info, QString time, QString day, QString prep, QString aud, int weekType) :
        _info(info),
        _time(time),
        _aud(aud),
        _prep(prep),
        _day(day),
        _weekType(weekType)
    {
    }

    QString getTime() const { return _time; }

    QString getDay() const
    {
        /*
        switch(_day)
        {
        case 1: return "Понедельник";
        case 2: return "Вторник";
        case 3: return "Среда";
        case 4: return "Четверг";
        case 5: return "Пятница";
        case 6: return "Суббота";
        default: return "";
        }*/
        return _day;
    }
    QString getPara() const
    {
        if(_time.left(4) == "8-30")
            return "1 пара";
        QString t = _time.left(5);
        if(t == "10-15")
            return "2 пара";
        if(t == "12-00")
            return "3 пара";
        if(t == "14-15")
            return "4 пара";
        if(t == "16-00")
            return "5 пара";
        if(t == "17-45")
            return "6 пара";
        return "7 пара";
    }
    QString getInfo() const { return _info; }
    QString getAud() const { return _aud; }
    QString getPrep() const { return _prep; }
    int getWeekType() const {return _weekType; }

    void setAud(QString aud) { _aud = aud; }
    void setPrep(QString prep) { _prep = prep; }

signals:
    void infoChanged();
    void dayChanged();
    void timeChanged();
    void audChanged();
    void prepChanged();
    void paraChanged();
};

#endif // LIST_EL_H
