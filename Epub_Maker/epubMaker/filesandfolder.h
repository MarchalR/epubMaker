#ifndef FILESANDFOLDER_H
#define FILESANDFOLDER_H

#include <QDir>

#include <QFile>
#include <QTextStream>
#include <QTextCursor>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QDirIterator>
#include <QThread>
#include "quazip.h"
#include "quazipfile.h"
#include "quazipfileinfo.h"
#include <QObject>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>
#include <QXmlReader>
#include <QXmlContentHandler>

class filesAndFolder
{
public:
    filesAndFolder();
    static int findFiles(QString sourceDir);
    static bool createEpubDir(QString sourceDir, QString destinationDir, bool overWriteDirectory);
    static void createPages(int nbPages, QString destinationDir);
    static void createOpf(int nbPages, QString destinationDir);
    static void createToc(int nbPages, QString name, QString destinationDir);
    static void editMetas();
};

#endif // FILESANDFOLDER_H
