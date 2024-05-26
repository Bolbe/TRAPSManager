#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QObject>
#include <QStringList>

class CSVParser : public QObject
{
    Q_OBJECT
public:
    explicit CSVParser(QObject *parent = nullptr);

    bool parseFileSection(const QString& fileName, const QString& sectionName, char separator = ',');


signals:

    void fatalError(const QString& title, const QString& message);
    void dataRow(int rowIndex, const QStringList& stringList);
    void sectionFound(QString sectionName);


};

#endif // CSVPARSER_H
