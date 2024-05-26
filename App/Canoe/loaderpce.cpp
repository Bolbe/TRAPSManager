#include "loaderpce.h"
#include <QDebug>
#include <QThread>
#include "csvparser.h"
#include "trapsdb.h"
#include "bib.h"

LoaderPCE::LoaderPCE(Database* database) :
    BibLoader(database)
{

}


void LoaderPCE::processLoad(const QString &fileName, bool reset) {
    load(fileName, "resultats", reset);
}

void LoaderPCE::process() {

    qDebug() << "Processing on thread " << QThread::currentThread();

    auto bibs = new QHash<QString,QJsonObject>();
    auto locks = new QHash<QString,QJsonObject>();
    auto times = new QHash<QString,QJsonObject>();
    auto penalties = new QHash<QString,QJsonObject>();

    emit toast("Chargement des dossards...", 3000);

    CSVParser parser;

    // Handling errors
    QObject::connect(
        &parser, &CSVParser::fatalError,
        this, &LoaderPCE::error
    );

    bool reset = _reset;

    QObject::connect(
        &parser, &CSVParser::sectionFound,
        [this, reset](const QString& sectionName) {
            if (reset) {
                _db->clear();
                emit toast("Dossards actuels effacés", 3000);
            }
        }
    );

    // Handling rows
    QObject::connect(
        &parser, &CSVParser::dataRow,
        [bibs, locks, times, penalties](int rowIndex, const QStringList& tab) {
            int entry = rowIndex+1;
            // bib number
            if (tab.length()<11) {
                qWarning() << "No bib number found in this line:" << tab;
                return;
            }
            bool ok;
            int bibnumber = tab[11].toInt(&ok);
            if (!ok || bibnumber<1) {
                qWarning() << "Cannot convert the bib number read in this line: " << tab;
                return;
            }
            Bib bib(bibnumber);
            bib.setCateg(tab[3]);

            //need to know how many teammate: column 12
            int mateCount = tab[12].toInt(&ok);
            if (!ok) mateCount=1;

            if (tab.length()>28 || tab.length()>23) {
                // schedule (index 23 if 1 guy, index 28 if 2 guys)
                if (mateCount==1) bib.setSchedule(tab[23]);
                else bib.setSchedule(tab[28]);
            }
            bib.setEntry(entry);
            bibs->insert(bib.numberStr(), bib.jsonParam());
            locks->insert(bib.numberStr(), bib.jsonLock());
            times->insert(bib.numberStr(), bib.jsonTime());
            penalties->insert(bib.numberStr(), bib.jsonPenalty());

        }
    );

    bool result = parser.parseFileSection(_fileName, "resultats", ';');

    qDebug() << "Inserting maps in db";
    if (result && bibs->count()>0) {
        _db->setValueMap(DB_BIB, *bibs);
        _db->setValueMap(DB_LOCKS, *locks);
        _db->setValueMap(DB_TIMES, *times);
        _db->setValueMap(DB_PENALTIES, *penalties);
        emit toast(QString("Liste de %0 dossards chargée").arg(bibs->count()), 4500);
        emit loaded();
    }
    delete bibs;
    delete locks;
    delete times;
    delete penalties;
    qDebug() << "Done with processing";
}

