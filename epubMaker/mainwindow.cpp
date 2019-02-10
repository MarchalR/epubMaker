#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QTextCursor>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <zip_file.hpp>
#include <QDirIterator>

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
    //QString gabaritDir = "/Users/AlDrac/gabarit";
    destinationDir = destinationDir + "/" + name;
    int nbPages = findFiles();

    createEpubDir(gabaritDir, destinationDir, true); //Create Epub Folder (EpubGen) and copy gabarit elements
    createPages(nbPages); //Create a page for each pictures in sourceDir folder and put them in destinationDir/OEPBS folder

    QString destinationDirImg = destinationDir + "/OEBPS/img";
    createEpubDir(sourceDir, destinationDirImg, true); //Copy imgs from testEpub to EpubGen/OEBPS/img

    createOpf(nbPages);
    createToc(nbPages, name);

}


void MainWindow::on_testButton_clicked()
{
    sourceDir = "/Users/AlDrac/massDir/testEpub1_epub";
    destinationDir = "/Users/AlDrac/massDir";

    folderZip(sourceDir, destinationDir);
}



void MainWindow::folderZip(QString sourceDir, QString destinationDir){

      std::string utf8_destDir = destinationDir.toUtf8().constData();
      std::string utf8_sourceDir = sourceDir.toUtf8().constData();

      miniz_cpp::zip_file file;


      std::string toCopy;


      //file.write(utf8_sourceDir);
      //file.save(std::string("/Users/AlDrac/massDir/test.zip"));


      QDirIterator it(sourceDir, QDirIterator::Subdirectories);

      QStringList fileList;
         do
         {
             fileList << it.next();
         } while (it.hasNext());



      QStringList result;

      /* FONCTIONNE MAIS DOIT SUPPR DEBUT PATH
      foreach (const QString &str, fileList) {
             if (!(str.contains("/."))){
                 result += str;
                toCopy = str.toUtf8().constData();
                qDebug() << str;
                file.write(toCopy);
             }

         }
      */


      // SUPPR DEBUT PATH MAIS NE FONCTIONNE PAS
      foreach (const QString &str, fileList) {
             if (!(str.contains("/."))){
                 //result += str;
                 QString string = str;
                 string.replace("/Users/AlDrac/massDir/testEpub1_epub","");

                 std::string stringToCons (string.toUtf8());
                 static const std::string& stringCons = stringToCons ;


                toCopy = stringCons;
                qDebug() << string;
                file.write(toCopy);
             }

         }




      file.save(std::string("/Users/AlDrac/massDir/test.zip"));

      //qDebug() << result;

}



void MainWindow::on_lunchMassConvert_clicked()
{
    QDir directory(massDir);
    QStringList allfiles = directory.entryList(QDir::AllEntries);
    for (int i = 0; i < 2; i++){
        qDebug() << "suppr" << allfiles.value(0);
        allfiles.removeAt(0);
    }

    for (int i = 0; i < allfiles.length(); i++){
        sourceDir = massDir + "/" + allfiles.value(i);
        destinationDir = massDir + "/" + allfiles.value(i) + "_epub";

        int nbPages = findFiles();

        createEpubDir(gabaritDir, destinationDir, true); //Create Epub Folder (EpubGen) and copy gabarit elements
        createPages(nbPages); //Create a page for each pictures in sourceDir folder and put them in destinationDir/OEPBS folder

        QString destinationDirImg = destinationDir + "/OEBPS/img";
        createEpubDir(sourceDir, destinationDirImg, true); //Copy imgs from testEpub to EpubGen/OEBPS/img

        createOpf(nbPages);
        createToc(nbPages, allfiles.value(i));

    }

    qDebug() << "file list" << allfiles.length();

}


QString MainWindow::selectFolder(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug() << "path" << dir;
    if ( dir.isNull() == false )
    {
        return dir;
    }
    return nullptr;
}

int MainWindow::findFiles(){
    QString path = sourceDir;
    QDir dir( path );
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    int total_files = static_cast<int>(dir.count());
    qDebug() << "Total Files" << total_files;

    return total_files;
}

bool MainWindow::createEpubDir(QString sourceDir, QString destinationDir,bool overWriteDirectory){

    QDir originDirectory(sourceDir);

        if (! originDirectory.exists())
        {
            return false;
        }

        QDir destinationDirectory(destinationDir);

        if(destinationDirectory.exists() && !overWriteDirectory)
        {
            return false;
        }
        else if(destinationDirectory.exists() && overWriteDirectory)
        {
            destinationDirectory.removeRecursively();
        }

        originDirectory.mkpath(destinationDir);

        foreach (QString directoryName, originDirectory.entryList(QDir::Dirs | \
                                                                  QDir::NoDotAndDotDot))
        {
            QString destinationPath = destinationDir + "/" + directoryName;
            originDirectory.mkpath(destinationPath);
            createEpubDir(sourceDir + "/" + directoryName, destinationPath, overWriteDirectory);
        }

        foreach (QString fileName, originDirectory.entryList(QDir::Files))
        {
            QFile::copy(sourceDir + "/" + fileName, destinationDir + "/" + fileName);
        }

        /*! Possible race-condition mitigation? */
        QDir finalDestination(destinationDir);
        finalDestination.refresh();

        if(finalDestination.exists())
        {
            return true;
        }

        return false;
}

void MainWindow::createPages(int nbPages){

    for (int i=0; i < nbPages; i++){
        QString htmlTxt = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?> <!DOCTYPE html> <html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\"> <head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /> <meta name=\"viewport\" content=\"width=2048, height=1536\" /> <title>Page" + QString::number( i ) + "</title> <link rel=\"stylesheet\" href=\"css/style.css\" /> </head> <body> <div id=\"bloc1\" class=\"container\"> <img class=\"page\" src=\"img/page" + QString::number( i ) + ".png\" /> </div> <script type=\"text/javascript\" charset=\"utf-8\" src=\"js/script.js\"></script> </body> </html> ";
        QString html = htmlTxt.toHtmlEscaped();
        QString filename= destinationDir +"/OEBPS/page" + QString::number( i ) + ".xhtml";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << htmlTxt << endl;
        }
    }
}

void MainWindow::createOpf(int nbPages){

    QString header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> <package xmlns=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" version=\"3.0\" unique-identifier=\"bookid\" prefix=\"rendition: http://www.idpf.org/vocab/rendition/# ibooks: http://vocabulary.itunes.apple.com/rdf/ibooks/vocabulary-extensions-1.0/\"> <metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\"><meta name=\"generator\" content=\"Remi Marchal\"/><meta name=\"cover\" content=\"couv.png\"/><dc:title>Let's go on holiday</dc:title><dc:creator>Lesley Ormal-Grenon, Marion Puech</dc:creator><dc:subject>jeunesse</dc:subject><dc:publisher>AlDrac Jeunesse</dc:publisher><dc:date>2016-06-30</dc:date><dc:rights>© AlDrac Éditions, 2016</dc:rights><dc:language>fr</dc:language><meta property=\"dcterms:modified\">2016-08-09T08:59:46Z</meta><dc:identifier id=\"bookid\">holiday</dc:identifier><!--fixed-layout options--><meta property=\"rendition:layout\">pre-paginated</meta><meta property=\"rendition:orientation\">landscape</meta><meta property=\"rendition:spread\">none</meta></metadata><manifest><item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\" properties=\"nav\"/><item id=\"holiJS\" href=\"js/script.js\" media-type=\"text/javascript\"/><item id=\"holiCSS.css\" href=\"css/style.css\" media-type=\"text/css\"/>";
    QString footer = "</manifest><spine>";
    QString ender = "</spine></package>";

    QString path = destinationDir + "/OEBPS/content.opf";
    qDebug() << "Path opf" << path;

    QFile file( path );

    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << header << endl;
        for (int i=0; i < nbPages; i++){
            stream << "<item id=\"img" + QString::number( i ) + "\" href=\"img/page" + QString::number( i ) + ".png\" media-type=\"image/png\"/>" << endl;
            stream << "<item id=\"page" + QString::number( i ) + "\" href=\"page" + QString::number( i ) + ".xhtml\" media-type=\"application/xhtml+xml\"  properties=\"scripted\"/>" << endl;
        }
        stream << footer << endl;

        for (int i=0; i < nbPages; i++){
            stream << "<itemref idref=\"page" + QString::number( i ) + "\"/>" << endl;
        }

        stream << ender << endl;
    }
}


void MainWindow::createToc(int nbPages, QString name){
    QString header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><html xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:epub=\"http://www.idpf.org/2007/ops\"><head><title>" + name + "</title></head><body><nav epub:type=\"page-list\"><h2>PageList</h2><ol>";
    QString mid = "</ol></nav><nav epub:type=\"toc\" id=\"toc\"><h2>PageList</h2><ol>";
    QString footer = "</ol></nav></body></html>";

    QString path = destinationDir + "/OEBPS/toc.xhtml";

    QFile file( path );

    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << header << endl;
        for (int i=0; i < nbPages; i++){
            stream << "<li><a href=\"page" + QString::number( i ) + ".xhtml\">Page" + QString::number( i ) + "</a></li>" << endl;
        }
        stream << mid << endl;

        for (int i=0; i < nbPages; i++){
            stream << "<li><a href=\"page" + QString::number( i ) + ".xhtml\">Page" + QString::number( i ) + "</a></li>" << endl;
        }

        stream << footer << endl;
    }
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


/* Get all elements in folder, keep for reference
QDir directory("/Users/AlDrac/EpubGen/OEBPS");
QStringList allfiles = directory.entryList(QDir::AllEntries);
qDebug() << "file list" << allfiles;
*/



void MainWindow::on_selectMassFolder_clicked()
{
    massDir = selectFolder();
    ui->selectMassFolder->setText(massDir);
}






