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

void MainWindow::on_excelPathBtn_released()
{

    QString path = QFileDialog::getOpenFileName(this,tr("Wybierz harmonogram"), QDir::currentPath(), tr("Excel (*.xlsx)"));
    if(!path.isEmpty()) {
        qDebug() << "BYYYDE" << endl;
        schedulePath = path;
        ui->excelPathLe->setText(path);
        generatePartList();
    }
}

void MainWindow::on_xmlPathBtn_released()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz plik XML"), QDir::currentPath(), tr("XML (*.xml)"));
    if(!fileName.isEmpty()) ui->xmlPathLe->setText(fileName);
}

void MainWindow::on_searchPathBtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Wybierz ścieżkę wyszukiwania"), "//k1/Produkcja/TECHNOLODZY/BAZA DO TXT/txt", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()) ui->excelPathLe->setText(dir);
}

void MainWindow::on_convertButton_released()
{
    QStringList missingPaths;
    if( ui->searchPathLe->text().isEmpty() || !ui->searchPathLe->text().contains("/"))
        missingPaths << "ścieżka wyszukiwania";
    if( ui->xmlPathLe->text().isEmpty() || !ui->xmlPathLe->text().contains("/"))
        missingPaths << "ścieżka pliku XML";
    if( ui->excelPathLe->text().isEmpty() || !ui->excelPathLe->text().contains("xlsx"))
        missingPaths << "ścieżka harmonogramu";

    if (!missingPaths.isEmpty()) {
        QMessageBox::information(this, tr("Informacja"), QString("Brakujące ścieżki: "+missingPaths.join(",")+"" + "."));
        return;
    }
    else {

//        setEnabled(true);
//        if(finder!=nullptr) delete finder;
//        if(finderThread!=nullptr) delete finderThread;

//        finder = new Finder(0, schedulePath, ui->inputLineEdit->text(), ui->outputLineEdit->text(), isWhite, searchCriterion);
//        finderThread = new QThread;
//        finder->moveToThread(finderThread);

//        QObject::connect( finder, SIGNAL(finished(bool,QString)), this, SLOT(on_processingFinished(bool,QString)));
//        QObject::connect( finder, SIGNAL(itemFound(QString,bool)), this, SLOT(on_itemFound(QString,bool)));
//        QObject::connect( finder, SIGNAL(signalProgress(int,QString) ), this, SLOT( on_setValue(int,QString)));
//        QObject::connect( finder, SIGNAL(showCopartnerDialog()),this,SLOT(on_showCopartnerDialog()),Qt::BlockingQueuedConnection);

//        QObject::connect( finderThread, SIGNAL(started()), finder, SLOT(findFiles())); // start searching
//        QObject::connect( finder, SIGNAL(finished(bool,QString)), finderThread, SLOT(quit()),Qt::DirectConnection);

//        finderThread->start();
    }
}

void MainWindow::createCommandTag(std::unique_ptr<QXmlStreamWriter> &xml)
{
    QStringList TblRef(QStringList() << "PRODUCTS" << "PRODUCT OPERATIONS" << "IMPORTGEO" << "MANUFACTURING");
    QStringList FldRefFirst(QStringList()  << "PrdRef" << "PrdRef" << "Product");
    QStringList FldRefSecond(QStringList() << "MatRef" << "WrkRef" << "GeometryType");
    QStringList FldRefThird(QStringList()  << "Height" << "OprRef" << "GeometryPath");
    QStringList FldRefFourth(QStringList()  << "mnOID" << "PrdRefDst" << "Descrip" << "OrdRef" << "ComName" << "RQ" << "DDate");

    for(int i=0;i<3;++i) {

        xml->writeStartElement("COMMAND");

        xml->writeAttribute("Name","Import");
        xml->writeAttribute("TblRef",TblRef[i]);

        // first line
        xml->writeStartElement("Field");
        xml->writeAttribute("FldRef",FldRefFirst[i]);
        xml->writeAttribute("FldValue","Indeks_detalu"); // drawing number ALWAYS
        xml->writeAttribute("FldType", "20");
        xml->writeEndElement();

        // second line
        xml->writeStartElement("Field");
        xml->writeAttribute("FldRef",FldRefSecond[i]);
        xml->writeAttribute("FldValue",(i==0) ? "S235JR" : (i==1 ? "Eckert" : "DXF")); // i=0 - TYPE , i=1 - MACHINE, i=2 = "DXF"
        xml->writeAttribute("FldType","20");
        xml->writeEndElement();

        // third line
        xml->writeStartElement("Field");
        xml->writeAttribute("FldRef",FldRefThird[i]);
        xml->writeAttribute("FldValue",(i==0) ? "35" : (i==1 ? "2D Cut" : "C:\DXF\100761.dxf")); // i=0 - HEIGHT , i=1 - "2D CUT", i=2 = DXF PATH
        xml->writeAttribute("FldType",(i==0) ? "100": "20");
        xml->writeEndElement();

        xml->writeEndElement(); // Command
    }

    xml->writeStartElement("COMMAND");
    xml->writeAttribute("Name","Import");
    xml->writeAttribute("TblRef",TblRef[TblRef.size()-1]); // MANUFACTURING

    for(int i=0;i<7;++i) {
        xml->writeStartElement("Field");
        xml->writeAttribute("FldRef",FldRefFourth[i]);
        xml->writeAttribute("FldValue","Indeks_detalu"); // drawing number ALWAYS
        xml->writeAttribute("FldType", (i==5) ? "30" : (i==6 ? "120" : "20"));
        xml->writeEndElement();
    }

    xml->writeEndElement(); // Command
}

bool MainWindow::createXML()
{
    QFile file(ui->xmlPathLe->text());
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::information(this, tr("Informacja"), QString("Nie można otworzyć pliku XML."));
        return false;
    }

    std::unique_ptr<QXmlStreamWriter> xmlWriter(new QXmlStreamWriter(&file));
    xmlWriter->setAutoFormatting(true);

    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement("DATAEX");

    createCommandTag(xmlWriter);

    xmlWriter->writeEndElement(); // Dataex
    xmlWriter->writeEndDocument();

    file.close();
    if (file.error()) {
        QMessageBox::information(this, tr("Informacja"), QString("Nie można otworzyć pliku XML."));
        return false;
    }

    return true;
}

void MainWindow::fillMoreCb(bool isType)
{
    QStringList list;

    if(isType)
        list = getItemsFromFile("TYPES.txt");
    else
        list = getItemsFromFile("MACHINERY.txt");

    ui->moreCb->clear();
    ui->moreCb->addItems(list);
}

QStringList MainWindow::getItemsFromFile(QString fileName)
{
    QStringList list;
    QFile file(QDir::currentPath() + "\\" + fileName);
    if ( file.open(QIODevice::ReadWrite) ) {
        QString line;
        QTextStream t( &file );
        do {
            line = t.readLine();
            if(!line.isEmpty())
                list.append(line);
         } while (!line.isNull());
        file.close();
    }

    else {
        QMessageBox::warning(this, tr("Uwaga"), QString("Nie można otworzyć pliku z danymi."));
        return QStringList();
    }

    return list;
}

bool MainWindow::generatePartList()
{

}

void MainWindow::on_excelPathLe_textChanged(const QString &arg1)
{
    ui->fitBtn->setEnabled(!arg1.isEmpty());
    ui->moreCb->setEnabled(!arg1.isEmpty());
    ui->typeCb->setEnabled(!arg1.isEmpty());
}

void MainWindow::on_typeCb_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Gatunek")
        fillMoreCb(true);
    else if(arg1 == "Maszyna")
        fillMoreCb(false);
    else {
        ui->moreCb->clear();
    }
}
