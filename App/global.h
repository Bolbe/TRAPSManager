#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

#define MATERIAL_SHADE_900 "#0d47a1"
#define MATERIAL_SHADE_800 "#1565c0"
#define MATERIAL_SHADE_700 "#1976d2"
#define MATERIAL_SHADE_600 "#1e88e5"
#define MATERIAL_SHADE_500 "#2196f3"
#define MATERIAL_SHADE_400 "#42a5f5"
#define MATERIAL_SHADE_300 "#64b5f6"
#define MATERIAL_SHADE_200 "#90caf9"
#define MATERIAL_SHADE_100 "#bbdefb"
#define MATERIAL_SHADE_50  "#e3f2fd"

class Global
{

public:

    static void init();

    static QString appDataDir;
    static QString httpConfigFile;
    static QString docroot;


private:
    Global() {}
    static void installFile(const QString& qrcSourceFilename, const QString& destinationFilename, bool forceInstall = false);
    static QString createDir(const QString& dirPath);

};

#endif // GLOBAL_H
