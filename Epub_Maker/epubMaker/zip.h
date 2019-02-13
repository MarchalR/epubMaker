#ifndef ZIP_H
#define ZIP_H

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

class zip
{
public:
    zip();

    static bool archive(const QString & filePath, const QDir & dir, const QString & comment = QString(""));
    static void recurseAddDir(QDir d, QStringList & list);

private:

};



#endif // ZIP_H
