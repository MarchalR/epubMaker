#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    int findFiles();
    bool createEpubDir(QString sourceDir, QString destinationDir, bool overWriteDirectory);
    void createPages(int nbPages);
    void createOpf(int nbPages);
    void createToc(int nbPages, QString name);
    void folderZip(QString sourceDir, QString destinationDir);
    QString selectFolder();
};

#endif // MAINWINDOW_H
