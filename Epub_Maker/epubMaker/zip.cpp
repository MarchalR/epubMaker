#include "zip.h"

zip::zip()
{

}


bool zip::archive(const QString & filePath, const QDir & dir, const QString & comment2) {

    QuaZip zip(filePath);
    zip.setFileNameCodec("IBM866");

    if (!zip.open(QuaZip::mdCreate)) {
    //myMessageOutput(true, QtDebugMsg, QString("testCreate(): zip.open(): %1").arg(zip.getZipError()));
        return false;
    }

    if (!dir.exists()) {
    //myMessageOutput(true, QtDebugMsg, QString("dir.exists(%1)=FALSE").arg(dir.absolutePath()));
        return false;
    }

    QFile inFile;

    QStringList sl;
        recurseAddDir(dir, sl);

    QFileInfoList files;
    foreach (QString fn, sl) files << QFileInfo(fn);

    QuaZipFile outFile(&zip);

    char c;
    foreach(QFileInfo fileInfo, files) {

        if (!fileInfo.isFile())
        continue;

        QString fileNameWithRelativePath = fileInfo.filePath().remove(0, dir.absolutePath().length() + 1);

        inFile.setFileName(fileInfo.filePath());

        if (!inFile.open(QIODevice::ReadOnly)) {
        //myMessageOutput(true, QtDebugMsg, QString("testCreate(): inFile.open(): %1").arg(inFile.errorString().toLocal8Bit().constData()));
        return false;
        }

        if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath()))) {
        //myMessageOutput(true, QtDebugMsg, QString("testCreate(): outFile.open(): %1").arg(outFile.getZipError()));
        return false;
        }

        while (inFile.getChar(&c) && outFile.putChar(c));

        if (outFile.getZipError() != UNZ_OK) {
     //myMessageOutput(true, QtDebugMsg, QString("testCreate(): outFile.putChar(): %1").arg(outFile.getZipError()));
        return false;
        }

        outFile.close();

        if (outFile.getZipError() != UNZ_OK) {
        //myMessageOutput(true, QtDebugMsg, QString("testCreate(): outFile.close(): %1").arg(outFile.getZipError()));
        return false;
        }

        inFile.close();
    }

    if (!comment2.isEmpty())
        zip.setComment(comment2);

    zip.close();

    if (zip.getZipError() != 0) {
    //myMessageOutput(true, QtDebugMsg, QString("testCreate(): zip.close(): %1").arg(zip.getZipError()));
        return false;
    }

    return true;

}

void zip::recurseAddDir(QDir d, QStringList & list) {

    QStringList qsl = d.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    foreach (QString file, qsl) {

     QFileInfo finfo(QString("%1/%2").arg(d.path()).arg(file));

     if (finfo.isSymLink())
      return;

     if (finfo.isDir()) {

      QString dirname = finfo.fileName();
      QDir sd(finfo.filePath());

      recurseAddDir(sd, list);

     } else
      list << QDir::toNativeSeparators(finfo.filePath());
    }
}
