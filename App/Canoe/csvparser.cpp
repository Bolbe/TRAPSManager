#include "csvparser.h"
#include <QFile>
#include <QDebug>
#include <QUrl>

CSVParser::CSVParser(QObject *parent) : QObject(parent) {

}

bool CSVParser::parseFileSection(const QString &fileName, const QString &sectionName, char separator)
{
    if (fileName.isEmpty()) {
        qCritical() << "No file selected. Abort.";
        emit fatalError("Fichier introuvable", "Nom de fichier vide.");
        return false;
    }

    QString path = QUrl(fileName).toLocalFile();
    qDebug() << "Parsing CSV data from file: " << path;
    if (!QFile::exists(path)) {
        qCritical() << "File does not exist: " << path;
        emit fatalError("Fichier introuvable", QString("Le fichier %1 n'existe pas.").arg(path));
        return false;
    }
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qCritical() << "Cannot open file " << path;
        emit fatalError("Erreur lecture fichier", QString("Impossible d'ouvrir le fichier\n%0").arg(path));
        return false;
    }

    QTextStream stream(&file);

    // find section
    QString string;
    while (true) {
        string = stream.readLine().trimmed();
        if (string.isNull() || stream.atEnd()) {
            qCritical() << "Cannot find section [" << sectionName << "] in file " << path;
            emit fatalError("Erreur lecture fichier", QString("Impossible de trouver la section [%1] dans le fichier\n%2").arg(sectionName).arg(path));
            return false;
        }
        if (string==QString("[%1]").arg(sectionName)) break;
        if (string=="[csv]") {
            QString section;
            section = stream.readLine().trimmed();
            if (!string.isNull() && !stream.atEnd() && section==sectionName) break;
        }
    }
    emit sectionFound(sectionName);
    int rowCount = 0;
    while (true) {
        string = stream.readLine().trimmed();
        if (string.isNull() || stream.atEnd()) break;
        if (string.isEmpty()) continue;
        if (string.at(0)=='[') break;

        //qDebug() << string;
        emit dataRow(rowCount++, string.split(separator));
    }
    return true;

}
