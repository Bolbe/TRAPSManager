#include "nickname.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QFile>
#include <limits>

QStringList NickName::_forbidden = QStringList()   << "SEX" << "ZIVA" << "SIDA" << "VOMI"
                                                    << "SAZI" << "NAZI" << "SALO" << "SALI" << "PEDOF" << "SANOFI"
                                                    << "PUT";

QStringList NickName::_syllableListShort = QStringList() << "BA" << "BI" << "BO" << "BU" << "DA" << "DI" << "DO"
                                                     << "DU" << "FA" << "FI" << "FO" << "FU" << "GA" << "GO" << "GU"
                                                     << "JA" << "JI" << "JO" << "JU" << "KA" << "KI" << "KO"
                                                     << "LA" << "LI" << "LO" << "LU" << "MA" << "MI" << "MO"
                                                     << "MU" << "NA" << "NI" << "NO" << "NU" << "PA" << "PI"
                                                     << "PO" << "PU" << "RA" << "RI" << "RO" << "RU" << "SA"
                                                     << "SI" << "SO" << "SU" << "TA" << "TI" << "TO" << "TU" << "VA"
                                                     << "VI" << "VO" << "VU" << "XA" << "XI" << "XO" << "XU"
                                                     << "ZA" << "ZI" << "ZO" << "ZU";

QStringList NickName::_syllableListLong = QStringList() << "BA" << "BE" << "BI" << "BO" << "BU" << "DA" << "DE" << "DI" << "DO" << "DU"
                                                      << "FA" << "FE" << "FI" << "FO" << "FU" << "GA" << "GO" << "GU" << "JA" << "JE"
                                                      << "JI" << "JO" << "JU" << "KA" << "KE" << "KI" << "KO" << "LA" << "LE" << "LI"
                                                      << "LO" << "LU" << "MA" << "ME" << "MI" << "MO" << "MU" << "NA" << "NE" << "NI"
                                                      << "NO" << "NU" << "PA" << "PE" << "PI" << "PO" << "PU" << "RA" << "RE" << "RI"
                                                      << "RO" << "RU" << "SA" << "SE" << "SI" << "SO" << "SU" << "TA" << "TE" << "TI"
                                                      << "TO" << "TU" << "VA" << "VE" << "VI" << "VO" << "VU" << "XA" << "XE" << "XI"
                                                      << "XO" << "XU" << "ZA" << "ZE" << "ZI" << "ZO" << "ZU";

QStringList NickName::name256 = QStringList() << "MENOTU" << "DEPOZO" << "BETADA" << "SAZEKI" << "ZATABI" << "GUTIDI" << "NOTABA" << "DOXAJA" << "FOGUTA" << "ROZIBA" << "RAKIBU" << "NOBOVA" << "MUVALI" << "FEXEZA" << "FETIZU" << "PUJETA" << "TEGOFA" << "ZURAJA" << "ZEJEMU" << "RATABI" << "SITABA" << "MUTABA" << "LAZEGA" << "POMAVU" << "ZUTAGU" << "TODAVA" << "LIXEKA" << "ROTABA" << "VUKATO" << "LINIDO" << "PABAVI" << "NUMETA" << "GUNUTA" << "DUBAFA" << "LIBIMO" << "NOJOTA" << "DEVATA" << "SATABI" << "MURELI" << "LUPOVU" << "VEZORA" << "MEGUVU" << "FIGABA" << "FIKIGA" << "LUNABI" << "ROTABA" << "KAFUJA" << "VEKIFI" << "XEMAJI" << "LEFAXO" << "TOPALO" << "LEVIBU" << "JOMILU" << "JITABA" << "DOFEZU" << "LUMONU" << "ZADAXU" << "LUBOZI" << "KEJOVO" << "GOMITU" << "NEJOTA" << "SIVULU" << "DUMIRU" << "VOTABA" << "RUGOBI" << "RUBAZU" << "MUFUTA" << "TAVABI" << "DUJOVA" << "NUJERA" << "KAPIGO" << "XIBEZA" << "TAXEZU" << "PIJIRA" << "BUPITA" << "NOTIJI" << "DAGOJA" << "ZEMONA" << "VOZUNU" << "KELABA" << "SUBEPI" << "SANOVU" << "MIZABU" << "FUGABA" << "RAJAVU" << "TEVABA" << "PIZITU" << "TIXAZO" << "JUKABA" << "GAXAMU" << "NOTABA" << "SENOKA" << "KALEVU" << "PIJUXA" << "SOTABA" << "RUTIVA" << "FURIGA" << "BAZAJO" << "LAVIKI" << "REVUTA" << "TUVABA" << "DEZAGO" << "BIMUNA" << "PUTABU" << "MOKALU" << "LEXARO" << "SATIGO" << "PILAGU" << "TUPOKA" << "KOREVI" << "PAMUBI" << "NAKALO" << "GOXAFI" << "NELUTA" << "PERABA" << "FAPIJO" << "TEVABA" << "TIDOKO" << "NEDOMA" << "NIVEXA" << "SEZUMU" << "JOKIDU" << "DIVIRO" << "ZAVEDU" << "ZOPOVU" << "XETARU" << "ZOBADA" << "BENUGU" << "MEVUGU" << "SARAKI" << "TOPIKI" << "JAXOGA" << "DAXANI" << "GAPEFA" << "VOXABA" << "DAROJO" << "NUZUXA" << "VAKARO" << "SITABA" << "GATABI" << "JIMOTU" << "LUNAVO" << "TUDIVU" << "TIDUJU" << "TIVABA" << "XIFALU" << "GOTABA" << "PIRABA" << "FUGABA" << "METUFO" << "TAROMU" << "PORABA" << "KATABI" << "ZEVEKO" << "LIBADA" << "NIPABA" << "GATOMI" << "MODEVU" << "SITABA" << "GATABI" << "LOTAPO" << "VOFEPI" << "SABOPO" << "NIBOLA" << "REFEPO" << "SONOJU" << "XEVEBA" << "FOLANO" << "ZUTIJO" << "KAPAGO" << "DEZELU" << "ROZIVU" << "VELIRI" << "KIDEFI" << "LABOGO" << "NUMIVA" << "PUZUDO" << "JUMILU" << "FIGABA" << "LAXIFO" << "FOZEVA" << "JUVUNO" << "BELUZU" << "PITORA" << "SAREDI" << "DELORA" << "SIZAPA" << "FALUVU" << "SADUVO" << "FEGAJA" << "GAMALI" << "SUXAJA" << "SUXUDI" << "GAXIFO" << "XIFOGO" << "TELIPU" << "XABEDA" << "SUTABA" << "LATAPI" << "GUFINO" << "JOBODU" << "ZIXALI" << "BIMURA" << "VUXETA" << "XIZABA" << "XUDOKI" << "BUGOVA" << "GUVIJA" << "ZAMUXA" << "BUFIZO" << "ZATABI" << "VEGUZI" << "XUZUBA" << "JORUKA" << "VAXABI" << "VIDEXI" << "VEZIGO" << "TIFOVA" << "DAMIFA" << "RUNOLU" << "KEZADU" << "GUVELA" << "VIPARA" << "TAZADI" << "NIGUBO" << "DOPOVA" << "PERABA" << "SUBIZI" << "JAKOPU" << "FALERO" << "LIGOMI" << "SABALI" << "KEJABU" << "VOLITA" << "BUKOMA" << "XERAMU" << "TAMEKA" << "GUZOTO" << "FIROTA" << "JAVELU" << "RITABA" << "NAMUBO" << "MINABA" << "XITABA" << "SADIJI" << "TEVABA" << "TOVABA" << "PURABA" << "LOZINU" << "LENAKO" << "SEVOXI" << "ZUTABA" << "SUTABA" << "REBUTA" << "ZOTABA" << "LOBADI";


QString NickName::name(double number) {

    if (number<0) number = -number;
    if (number>1) number = 1;
    int setShort = NickName::_syllableListShort.count();
    int setLong = NickName::_syllableListLong.count();

    int universe = setShort*setLong*setLong;
    qDebug() << "Universe: " << universe;

    int index = (int)(number*universe);
    QString value;

    while (value.isEmpty()) {

        value = NickName::_name(index);
        index++;
        if (index>=universe) index = 0;

    }

    qDebug() << value;

    return value;
}

QString NickName::randomName() {
    double ratio = (double)QRandomGenerator::global()->generate()/std::numeric_limits<quint32>::max();
    return NickName::name(ratio);
}

QString NickName::_name(int number) {

    int setShort = NickName::_syllableListShort.count();
    int setLong = NickName::_syllableListLong.count();

    int digit3 = number/(setShort*setLong)%setLong;
    int digit2 = ((number-digit3*setShort*setLong)/setShort)%setLong;
    int digit1 = number%setShort;

    QString syl3 = NickName::_syllableListLong.value(digit3);
    QString syl2 = NickName::_syllableListLong.value(digit2);
    QString syl1 = NickName::_syllableListShort.value(digit1);

    if (syl3[1]==syl2[1] && syl2[1]==syl1[1]) return QString();
    if (syl1[0]==syl2[0] || syl1[0]==syl3[0] || syl2[0]==syl3[0]) return QString();
    if ((syl2[0]=='S' || syl1[0]=='S')) return QString();

    QString value = QString("%0%1%2").arg(syl3).arg(syl2).arg(syl1);
    if (_forbidden.contains(value)) return QString();
    return value;

}

void NickName::dump(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream outputStream(&file);
    int universe = NickName::_syllableListShort.count()*NickName::_syllableListLong.count()*NickName::_syllableListLong.count();
    for (int i=0; i<universe; i++) {
        QString name = _name(i);
        if (!name.isEmpty()) outputStream << name << "\n";
    }

}

void NickName::dumpRandom(int count, const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream outputStream(&file);
    for (int i=0; i<count; i++) {
        double ratio = (double)QRandomGenerator::global()->generate()/std::numeric_limits<quint32>::max();
        QString name = NickName::name(ratio);
        outputStream << QString("\"%0\" << ").arg(name);
    }
}

