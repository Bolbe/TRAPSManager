#ifndef NICKNAME_H
#define NICKNAME_H

#include <QStringList>

class NickName
{
public:

    static QStringList name256;

    static QString name(double number);
    static QString randomName();
    static void dump(const QString& filename);
    static void dumpRandom(int count, const QString& filename);



private:

    static QStringList _forbidden;
    static QStringList _syllableListShort;
    static QStringList _syllableListLong;

    static QString _name(int number);

};

#endif // NICKNAME_H
