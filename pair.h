#ifndef PAIR_H
#define PAIR_H

#include <QString>
#include <QTextStream>
#include <QVector>

class Pair {
public:
    Pair(QString time) :
        _time     (time),
        _upWeek(6),
        _downWeek(6)
    {

    }

    void pushDay(QString upDay, QString downDay, int day)
    {
        _upWeek.insert(day, upDay);
        _downWeek.insert(day, downDay);
    }

    void show(QTextStream & stream) const
    {
        for(int i = 0; i < 6; i++)
        {
            stream << i << "th DAY " << "\n\n    ";
            stream << "UP  " << _upWeek[i] << "\n\n    ";
            stream << "DOWN  " << _downWeek[i] << "\n\n    ";
        }
        stream << "\n\n\n    ";
    }

    QVector<QString> getUpWeek() const
    {
        return _upWeek;
    }

    QVector<QString> getDownWeek() const
    {
        return _downWeek;
    }

    QString getTime() const
    {
        return _time;
    }

private:
    QString          _time;
    QVector<QString> _upWeek;
    QVector<QString> _downWeek;
};


#endif // PAIR_H
