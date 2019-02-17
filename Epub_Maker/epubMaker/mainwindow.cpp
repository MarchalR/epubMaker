#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_goButton_clicked()
{

    QString name = ui->lineEdit->text();
    destinationDir = destinationDir + "/" + name;
    int nbPages = filesAndFolder::findFiles(sourceDir);

    filesAndFolder::createEpubDir(gabaritDir, destinationDir, true); //Create Epub Folder (EpubGen) and copy gabarit elements
    filesAndFolder::createPages(nbPages, destinationDir); //Create a page for each pictures in sourceDir folder and put them in destinationDir/OEPBS folder

    QString destinationDirImg = destinationDir + "/OEBPS/img";
    filesAndFolder::createEpubDir(sourceDir, destinationDirImg, true); //Copy imgs from testEpub to EpubGen/OEBPS/img

    filesAndFolder::createOpf(nbPages, destinationDir);
    filesAndFolder::createToc(nbPages, name, destinationDir);

    QDir dir = sourceDir;
    QString filePath = destinationDir  +".epub";
    QString comment2 = QString("test");

    zip::archive(filePath, dir, comment2);

    qDebug() << destinationDir;
    QDir dirToDel(destinationDir);
    dirToDel.removeRecursively();

}


void MainWindow::on_testButton_clicked()
{
    /*sourceDir = "/Users/AlDrac/massDir/testEpub1_epub";
    destinationDir = "/Users/AlDrac/massDir/test2.epub";

    QDir dir = sourceDir;
    QString filePath = destinationDir;
    QString comment2 = QString("test");
    //folderZip2(sourceDir, destinationDir);
    archive(filePath, dir, comment2);*/
}



void MainWindow::on_lunchMassConvert_clicked()
{
    QDir directory(massDir);
    QStringList allfiles = directory.entryList(QDir::AllEntries);
    for (int i = 0; i < 2; i++){
        allfiles.removeAt(0);
    }

    for (int i = 0; i < allfiles.length(); i++){
        sourceDir = massDir + "/" + allfiles.value(i);
        destinationDir = massDir + "/" + allfiles.value(i) + "_epub";

        int nbPages = filesAndFolder::findFiles(sourceDir);

        filesAndFolder::createEpubDir(gabaritDir, destinationDir, true); //Create Epub Folder (EpubGen) and copy gabarit elements
        filesAndFolder::createPages(nbPages, destinationDir); //Create a page for each pictures in sourceDir folder and put them in destinationDir/OEPBS folder

        QString destinationDirImg = destinationDir + "/OEBPS/img";
        filesAndFolder::createEpubDir(sourceDir, destinationDirImg, true); //Copy imgs from testEpub to EpubGen/OEBPS/img

        filesAndFolder::createOpf(nbPages, destinationDir);
        filesAndFolder::createToc(nbPages, allfiles.value(i), destinationDir);

        QDir dir = destinationDir;
        QString filePath = destinationDir + ".epub";
        QString comment2 = QString("test");

        zip::archive(filePath, dir, comment2);


        qDebug() << destinationDir;
        QDir dirToDel(destinationDir);
        dirToDel.removeRecursively();
    }
}


void MainWindow::on_unzipEpub_clicked()
{
    QFileInfo fi(toUnzip);
    QString nameDir = fi.fileName();

    destinationDir = fi.absolutePath();

    zip::extract(toUnzip, destinationDir);

    qDebug() << "toUnzip" << toUnzip;
     qDebug() << "namedir" << nameDir;
      qDebug() << "destinationDir" << destinationDir;
}

void MainWindow::on_saveMeta_clicked()
{

}



QString MainWindow::selectFolder(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if ( dir.isNull() == false )
    {
        return dir;
    }
    return nullptr;
}


QString MainWindow::selectFile(){
    QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                               "/home",
                                               tr("Epub (*.epub)"));
    if ( file.isNull() == false )
    {
        return file;
    }
    return nullptr;
}

void MainWindow::on_selectImageFolder_clicked()
{
    sourceDir = selectFolder();
    ui->selectImageFolder->setText(sourceDir);
}

void MainWindow::on_selectDestinationFolder_clicked()
{
    destinationDir = selectFolder();
    ui->selectDestinationFolder->setText(destinationDir);
}

void MainWindow::on_selectGabarit_clicked()
{
    gabaritDir = selectFolder();
    ui->selectGabarit->setText(gabaritDir);
}

void MainWindow::on_selectGabaritMass_clicked()
{
    gabaritDir = selectFolder();
    ui->selectGabaritMass->setText(gabaritDir);
}

void MainWindow::on_selectMassFolder_clicked()
{
    massDir = selectFolder();
    ui->selectMassFolder->setText(massDir);
}

void MainWindow::on_selectToUnzip_clicked()
{
    toUnzip = selectFile();
    ui->selectToUnzip->setText(toUnzip);
}

void MainWindow::on_selectToEdit_clicked()
{
    toEdit = selectFile();
    ui->selectToEdit->setText(toEdit);
}

