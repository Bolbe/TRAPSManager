#include "bibloader.h"
#include "database/database.h"
#include <QThread>
#include <QDebug>
#include <QtConcurrent>

BibLoader::BibLoader(Database *database) :
    _db(database)
{
    Q_ASSERT(_db);

}

void BibLoader::load(const QString &fileName,
                     const QString &sectionName,
                     bool reset)
{

    qDebug() << "Calling load on thread " << QThread::currentThread();
    _fileName = fileName;
    _sectionName = sectionName;
    _reset = reset;

    QtConcurrent::run([this]() {
        qDebug() << "Executing process on thread " << QThread::currentThread();
        process();
    });


}
