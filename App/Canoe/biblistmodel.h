#ifndef BIBLISTMODEL_H
#define BIBLISTMODEL_H

#include <QObject>
#include <spreadsheet/spreadsheet.h>
#include <Canoe/bib.h>
#include <QJsonArray>

class TRAPSDB;
class PenaltyViewModel;

class BibListModel : public SpreadSheet
{

    Q_OBJECT
    Q_PROPERTY(bool chronoVisible READ chronoVisible CONSTANT)
    Q_PROPERTY(bool teamMode READ teamMode CONSTANT)
    Q_PROPERTY(int scheduling READ scheduling CONSTANT)

public:
    explicit BibListModel(TRAPSDB* database, PenaltyViewModel* penaltyViewModel);
    ~BibListModel();

    int bibCount() const;
    QPair<Bib*, int> bibAtRowIndex(int rowIndex) const;
    Bib* bibWithNumber(int number) const;
    int bibIndex(int bibnumber) const;

    bool chronoVisible() const;
    bool teamMode() const;
    int scheduling() const { return _scheduling; }

signals:
    void error(QString title, QString message);
    void toast(QString text, int delay);
    void bibCountChanged(int);
    // used to send data to third parties
    void penaltyUpdate(QPair<int, int>, int gateId, int value); // bib, gate, value
    void chronoReceived(int bib, int value); // bib, value


public slots:
    void requestAction(int rowIndex, int columnIndex);
    void setChronoVisible(bool visible);
    void setTeamMode(bool modeOn);

    void processIncomingPenaltyList(QList<Penalty> penaltyList);
    void processIncomingPenalty(int senderId, int bibnumber, QHash<int, int> penaltyList);
    void processIncomingStartTime(int bibnumber, qint64 startTime);
    void processIncomingFinishTime(int bibnumber, qint64 finishTime);
    void processIncomingLapTime(int bibnumber, int lap, qint64 time);

    void selectPenalty(int bibIndex, int gateIndex);
    void setScheduling(int criteria);

    QJsonArray jsonArray() const;

    // used by tcp server
    QList<int> bibNumberList() const;

    void loadCSV(const QString& filename, bool reset=true);
    void clearPenalties();
    void clearChronos();
    void reload();


protected:
    QString text(int rowIndex, int columnIndex) const;
    QString bgColor(int rowIndex, int columnIndex) const;
    QString textColor(int rowIndex, int columnIndex) const;
    QStringList contextMenuList(int rowIndex, int columnIndex) const;
    void contextMenuAction(int rowIndex, int columnIndex, int menuIndex);
    int rowCount() const;

private:

    QList<Bib*> _bibList;

    TRAPSDB* _db;
    PenaltyViewModel* _penaltyView;
    int _scheduling;
    int _penaltyDepth;

    void orderBibList();

    void setLock(bool lock, QSet<int> rowIndexSet);
    void setLock(bool lock, int rowIndex);

    void forwardBib(QSet<int> rowIndexSet);
    void forwardBib(int rowIndex);

};

#endif // BIBLISTMODEL_H
