#ifndef BIBLOADER_H
#define BIBLOADER_H

#include <QObject>

class Database;

class BibLoader : public QObject
{
    Q_OBJECT
public:
    explicit BibLoader(Database* database);

    void load(const QString &fileName, const QString &sectionName, bool reset);

signals:

    void error(QString title, QString message);
    void toast(QString text, int delay);
    void loaded();

protected slots:

    virtual void process() = 0;

protected:
    Database* _db;
    QString _fileName;
    QString _sectionName;
    bool _reset;


};

#endif // BIBLOADER_H
