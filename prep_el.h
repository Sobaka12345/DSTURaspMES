#ifndef PREP_EL_H
#define PREP_EL_H

#include <QObject>
#include <QString>

class PrepEl : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)

    QString _name, _id;

public:
    PrepEl(QString name, QString id) :
        _name   (name),
        _id     (id)
    {}

    QString getName() const { return _name; }
    QString getId()   const { return _id;   }

signals:
    void nameChanged();
    void idChanged();
};

#endif // PREP_EL_H
