#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connectionsdialog.h"
#include "ircconnection.h"
#include <iostream>
#include <QTextEdit>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>
#include <QHash>
#include <QVariant>
#include <settingsmanager.h>
#include "ircmanager.h"
#include <QThread>
#include <qiodevice.h>

#define HIT_MSG_BOX() QMessageBox q; q.exec();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextEdit *qte = new QTextEdit();
    qte->setReadOnly(true);
    ui->stackedWidget->addWidget(qte);
    ui->stackedWidget->setCurrentWidget(qte);

    //TODO: Check if db exists before doing this.
    SettingsManager* s = SettingsManager::GetInstance();
    s->CreateTables();
    s->LoadSettingsFromDB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnections_triggered()
{
    SettingsManager* s = SettingsManager::GetInstance();
    s->LoadSettingsFromDB();
    ConnectionsDialog d(this);
    d.exec();
}

void MainWindow::on_actionQuick_connect_triggered()
{
    IrcManager* m = IrcManager::GetInstance();
    m->OnQuickConnect();
//    SettingsManager* s = SettingsManager::GetInstance();
//    s->LoadSettingsFromDB();

//    //std::vector<IrcNetwork*>& networks = &(s->GetSavedNetworks());
//    for(int i = 0; i < s->GetSavedNetworks().size(); i++)
//    {
//        s->GetSavedNetworks()[i]->StartConnection();
//        IrcManager* ircM = IrcManager::GetInstance();
//        IrcNetwork* net = s->GetSavedNetworks()[i];

//        IrcNetwork copy = new IrcNetwork();
//        copy.setname(net->GetName());
//        copy.set
//        //ircM->AddConnection(*net);
//    }
}
