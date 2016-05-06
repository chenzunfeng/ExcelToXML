#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QtXml>
#include <memory>
#include "finder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_excelPathBtn_released();
    void on_xmlPathBtn_released();
    void on_searchPathBtn_clicked();
    void on_convertButton_released();
    void on_excelPathLe_textChanged(const QString &arg1);
    void on_typeCb_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QXmlStreamWriter xml;
    QString schedulePath;
    void createCommandTag(std::unique_ptr<QXmlStreamWriter> &xml);
    bool createXML();
    QXmlStreamWriter * xmlWriter;
    void fillMoreCb(bool isType);
    QStringList getItemsFromFile(QString fileName);
    bool generatePartList();
};

#endif // MAINWINDOW_H
