#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QtSql>
#include <QSqlQuery>

QSqlDatabase db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    DatabaseFileCheck();
    ui->setupUi(this);
}

void MainWindow::DatabaseFileCheck()
{
    // TODO: get database from config path
    QString dbpath="";
#ifdef Q_OS_WINDOWS
    dbpath = ".\\Idioms.db";
#endif
#ifdef Q_OS_MACOS
#endif
#ifdef Q_OS_LINUX
    dbpath = "./Idioms.db";
#endif

    if(QFileInfo::exists(dbpath))
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbpath);
        if(!db.open())
        {
            QMessageBox msgBox;
            msgBox.setText(db.lastError().text());
            msgBox.exec();
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("成语数据库不存在");
        msgBox.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(db.isOpen())
    {
        db.close();
    }
}

void MainWindow::on_SearchButton_clicked()
{
    QString targetstring = ui->lineEdit->text().trimmed();
    if(targetstring.isEmpty())
    {
        return;
    }

    if(!db.isOpen())
    {
        return;
    }

    QString sql = "SELECT Word FROM IdiomsData WHERE FirstChar='" + targetstring.back() + "';";
    QSqlQuery query(db);
    if(!query.exec(sql))
    {
        QMessageBox msgBox;
        msgBox.setText(query.lastError().text());
        msgBox.exec();
        return;
    }

    ui->ListWidget->clear();

    int idName = query.record().indexOf("Word");
    while (query.next())
    {
       QString name = query.value(idName).toString();
       ui->ListWidget->addItem(name);
    }
}


void MainWindow::on_ListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->lineEdit->setText(item->text());
}

