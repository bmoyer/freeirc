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
#include "chatpane.h"
#include <QStandardItem>

#define HIT_MSG_BOX() QMessageBox q; q.exec();

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ChatPane *chatPane = new ChatPane("FreeIRC", "STATUS");
    chatPane->setText("mainwindow");
    chatPane->setReadOnly(true);
    ui->stackedWidget->addWidget(chatPane);
    ui->stackedWidget->setCurrentWidget(chatPane);
    mChatPanes.push_back(chatPane);

    IrcManager* m = IrcManager::GetInstance();
    m->SetMainWindowPtr(this);
    connect(m, SIGNAL(packet(QStringList)),
            SLOT(OnIrcMessage(QStringList)));

    //TODO: Check if db exists before doing this.
    SettingsManager* s = SettingsManager::GetInstance();
    s->CreateTables();
    s->LoadSettingsFromDB();

//    QStandardItemModel* model = new QStandardItemModel();

//    QStandardItem* item0 = new QStandardItem("SwiftIRC");
//    QStandardItem* item1 = new QStandardItem("Rizon");
//    QStandardItem* item3 = new QStandardItem("#pixelz");
//    QStandardItem* item4 = new QStandardItem("#drug");
//    QStandardItem* item5 = new QStandardItem("#whatever");

//    model->appendRow(item0);
//    model->appendRow(item1);
//    item0->appendRow(item3);
//    item0->appendRow(item4);
//    item1->appendRow(item5);

//    ui->chatSessionTree->setModel(model);
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
    SettingsManager* m = SettingsManager::GetInstance();
    m->LoadSettingsFromDB();
    QStandardItemModel* model = new QStandardItemModel();
    for(int i = 0; i < m->GetSavedNetworks().size(); i++)
    {
        IrcNetwork* n = m->GetSavedNetworks()[i];
        QStandardItem* nModel = new QStandardItem(n->GetName());
        model->appendRow(nModel);

        for(int j = 0; j < n->GetAutojoinChannels().size(); j++)
        {
            QString name = n->GetAutojoinChannels()[i].GetName();
            QStandardItem* cModel = new QStandardItem(name);
            nModel->appendRow(cModel);

            qDebug() << "row added!";
        }
    }
    ui->chatSessionTree->setModel(model);
    IrcManager* ircM = IrcManager::GetInstance();
    ircM->OnQuickConnect();
}

void MainWindow::AddChatPane(QString networkName, QString channelName)
{

    ChatPane *c = new ChatPane(networkName, channelName);
    mChatPanes.push_back(c);
}

ChatPane* MainWindow::GetChatPane(QString networkName, QString channelName)
{
    for(int i = 0; i < mChatPanes.size(); i++)
    {
        if((mChatPanes[i]->GetNetworkName() == networkName) && (mChatPanes[i]->GetChannelName() == channelName))
        {
            return mChatPanes[i];
        }
    }
}

void MainWindow::AddMessageToChatPane(QString networkName, QString sender, QString target, QString message)
{
    //QString networkName = "FreeIRC";
    //QString channelName = "STATUS";
    for(int i = 0; i < mChatPanes.size(); i++)
    {
        if((mChatPanes[i]->GetNetworkName() == networkName) && (mChatPanes[i]->GetChannelName() == sender))
        {
            qDebug() << "found the one";
            QString messageStr = "<" + sender + "> " + message + "\n";
            mChatPanes[i]->append(messageStr);
        }
    }
}

void MainWindow::OnIrcMessage(QStringList messagePacket)
{
    for(int i = 0; i < messagePacket.size(); i++)
    {
        qDebug() << messagePacket[i];
    }
    AddMessageToChatPane(messagePacket[0], messagePacket[1], messagePacket[2], messagePacket[3]);
}

void MainWindow::on_chatSessionTree_objectNameChanged(const QString &objectName)
{
    qDebug() << "Object name was" << objectName;
}

void MainWindow::on_chatSessionTree_clicked(const QModelIndex &index)
{
    qDebug() << index.parent().data().toString() << index.data().toString();

    QString network = index.parent().data().toString();
    QString channel = index.data().toString();
    if(network != "")
    {
        ChatPane* pane = GetChatPane(network, channel);
        ui->stackedWidget->setCurrentWidget(pane);
    }
}
