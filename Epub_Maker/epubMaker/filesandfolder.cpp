#include "filesandfolder.h"

filesAndFolder::filesAndFolder()
{

}

QHash<QString, QString> filesAndFolder::getMetas(QString pathToEdit){
    QFile inputFile(pathToEdit + "/OEBPS/content.opf");
    qDebug() << inputFile.exists();
        inputFile.open(QIODevice::ReadWrite);
        QTextStream in(&inputFile);

        QHash<QString, QString> listMetas;

         QXmlStreamReader xmlReader(&inputFile);

        //Parse the XML until we reach end of it
        while(!xmlReader.atEnd() && !xmlReader.hasError()) {
                // Read next element
                QXmlStreamReader::TokenType token = xmlReader.readNext();
                //If token is just StartDocument - go to next
                if(token == QXmlStreamReader::StartDocument) {
                        continue;
                }
                //If token is StartElement - read it
                if(token == QXmlStreamReader::StartElement) {

                        if(xmlReader.name() == "title") {
                            listMetas["title"] = xmlReader.readElementText();
                                //continue;
                        }

                        if(xmlReader.name() == "language") {
                            listMetas["language"] = xmlReader.readElementText();

                        }

                        if(xmlReader.name() == "creator") {
                            listMetas["creator"] = xmlReader.readElementText();
                        }

                        if(xmlReader.name() == "publisher") {
                            listMetas["publisher"] = xmlReader.readElementText();
                        }
                        if(xmlReader.name() == "rights") {
                            listMetas["rights"] = xmlReader.readElementText();
                        }

                        if(xmlReader.name() == "identifier") {
                            listMetas["identifier"] = xmlReader.readElementText();
                        }
                        if(xmlReader.name() == "description") {
                            listMetas["description"] = xmlReader.readElementText();
                        }
                        if(xmlReader.name() == "rendition:layout") {
                            listMetas["layout"] = xmlReader.readElementText();
                        }
                        if(xmlReader.name() == "orientation") {
                            listMetas["orientation"] = xmlReader.readElementText();
                        }

                        if(xmlReader.name() == "spread") {
                            listMetas["spread"] = xmlReader.readElementText();
                        }
                }
        }

        if(xmlReader.hasError()) {
            qDebug() << "error";
        }

        xmlReader.clear();
        inputFile.close();
        return listMetas;
}


void filesAndFolder::changeOpf(int nbPages, QString destinationDir, QHash<QString, QString> metas){

    QString header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> <package xmlns=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" version=\"3.0\" unique-identifier=\"bookid\" prefix=\"rendition: http://www.idpf.org/vocab/rendition/# ibooks: http://vocabulary.itunes.apple.com/rdf/ibooks/vocabulary-extensions-1.0/\"> <metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\"><meta name=\"generator\" content=\"Remi Marchal\"/><meta name=\"cover\" content=\"couv.png\"/><dc:title>"+ metas["title"] +"</dc:title><dc:creator>"+ metas["creator"] +"</dc:creator><dc:subject>jeunesse</dc:subject><dc:publisher>"+ metas["publisher"] +"</dc:publisher><dc:date>2016-06-30</dc:date><dc:rights>"+ metas["rights"] +"</dc:rights><dc:language>"+ metas["language"] +"</dc:language><meta property=\"dcterms:modified\">2016-08-09T08:59:46Z</meta><dc:identifier id=\"bookid\">"+ metas["identifier"] +"</dc:identifier><!--fixed-layout options--><meta property=\"rendition:layout\">pre-paginated</meta><meta property=\"rendition:orientation\">landscape</meta><meta property=\"rendition:spread\">none</meta></metadata><manifest><item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\" properties=\"nav\"/><item id=\"holiJS\" href=\"js/script.js\" media-type=\"text/javascript\"/><item id=\"holiCSS.css\" href=\"css/style.css\" media-type=\"text/css\"/>";
    QString footer = "</manifest><spine>";
    QString ender = "</spine></package>";

    QString path = destinationDir + "/OEBPS/content.opf";

    QFile file( path );
    file.remove();

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



int filesAndFolder::findFiles(QString sourceDir){
    QString path = sourceDir;
    QDir dir( path );
    dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    int total_files = static_cast<int>(dir.count());

    return total_files;
}

bool filesAndFolder::createEpubDir(QString sourceDir, QString destinationDir,bool overWriteDirectory){

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

void filesAndFolder::createPages(int nbPages, QString destinationDir){

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

void filesAndFolder::createOpf(int nbPages, QString destinationDir){

    QString header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> <package xmlns=\"http://www.idpf.org/2007/opf\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" version=\"3.0\" unique-identifier=\"bookid\" prefix=\"rendition: http://www.idpf.org/vocab/rendition/# ibooks: http://vocabulary.itunes.apple.com/rdf/ibooks/vocabulary-extensions-1.0/\"> <metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\"><meta name=\"generator\" content=\"Remi Marchal\"/><meta name=\"cover\" content=\"couv.png\"/><dc:title>Let's go on holiday</dc:title><dc:creator>Lesley Ormal-Grenon, Marion Puech</dc:creator><dc:subject>jeunesse</dc:subject><dc:publisher>AlDrac Jeunesse</dc:publisher><dc:date>2016-06-30</dc:date><dc:rights>© AlDrac Éditions, 2016</dc:rights><dc:language>fr</dc:language><meta property=\"dcterms:modified\">2016-08-09T08:59:46Z</meta><dc:identifier id=\"bookid\">holiday</dc:identifier><!--fixed-layout options--><meta property=\"rendition:layout\">pre-paginated</meta><meta property=\"rendition:orientation\">landscape</meta><meta property=\"rendition:spread\">none</meta></metadata><manifest><item id=\"toc\" href=\"toc.xhtml\" media-type=\"application/xhtml+xml\" properties=\"nav\"/><item id=\"holiJS\" href=\"js/script.js\" media-type=\"text/javascript\"/><item id=\"holiCSS.css\" href=\"css/style.css\" media-type=\"text/css\"/>";
    QString footer = "</manifest><spine>";
    QString ender = "</spine></package>";

    QString path = destinationDir + "/OEBPS/content.opf";

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


void filesAndFolder::createToc(int nbPages, QString name, QString destinationDir){
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
