#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include "zip.h"
#include "filesandfolder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString sourceDir;
    QString destinationDir;
    QString gabaritDir;
    QString massDir;

private slots:

    void on_goButton_clicked();

    void on_selectImageFolder_clicked();

    void on_selectDestinationFolder_clicked();

    void on_selectGabarit_clicked();

    void on_selectMassFolder_clicked();

    void on_lunchMassConvert_clicked();

    void on_selectGabaritMass_clicked();

    void on_testButton_clicked();



private:
    Ui::MainWindow *ui;    
    QString selectFolder();
};

#endif // MAINWINDOW_H
