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
    mChatListItemModel = new QStandardItemModel();
    AddChatPane("FreeIRC","");
    GetChatPane("FreeIRC", "")->setAlignment(Qt::AlignCenter);
    ui->stackedWidget->setCurrentWidget(GetChatPane("FreeIRC", ""));
    GetChatPane("FreeIRC", "")->setText("Welcome to FreeIRC!\nTo get started, click Chat => Quick-connect!");
    QStandardItem* nModel = new QStandardItem("FreeIRC");
    mChatListItemModel->appendRow(nModel);
    ui->chatSessionTree->setModel(mChatListItemModel);

    IrcManager* m = IrcManager::GetInstance();
    m->SetMainWindowPtr(this);
    connect(m, SIGNAL(packet(QStringList)),
            SLOT(OnIrcMessage(QStringList)));

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
    SettingsManager* m = SettingsManager::GetInstance();
    m->LoadSettingsFromDB();

    //If user has no networks saved, alert them and take them to Connections dialog
    if(m->GetSavedNetworks().size() == 0)
    {
       QMessageBox::StandardButton reply = QMessageBox::question(this,
                   "tes", "You have no IRC networks saved.\nWould you like to add some?", QMessageBox::Yes |
                                      QMessageBox::No, QMessageBox::Yes);
       if(reply == QMessageBox::Yes)
       {
        ConnectionsDialog d(this);
        d.exec();
       }
    }

    for(int i = 0; i < m->GetSavedNetworks().size(); i++)
    {
        IrcNetwork* n = m->GetSavedNetworks()[i];
        QString networkName = n->GetName();
        //AddChatPane(networkName, "");
        QStandardItem* nModel = new QStandardItem(networkName);
        mChatListItemModel->appendRow(nModel);
        QStandardItem* statusModel = new QStandardItem("Status");
        AddChatPane(networkName, "Status");
        nModel->appendRow(statusModel);

        for(int j = 0; j < n->GetAutojoinChannels().size(); j++)
        {
            /* Adding chat panes here */
            QString channelName = n->GetAutojoinChannels()[j].GetName();
            AddChatPane(networkName, channelName);
            QStandardItem* cModel = new QStandardItem( channelName);
            nModel->appendRow(cModel);
        }
    }
    ui->chatSessionTree->setModel(mChatListItemModel);
    IrcManager* ircM = IrcManager::GetInstance();
    ircM->OnQuickConnect();
}

void MainWindow::AddChatPane(QString networkName, QString channelName)
{

    ChatPane *c = new ChatPane(networkName, channelName);
    c->setReadOnly(true);
    mChatPanes.push_back(c);
    ui->stackedWidget->addWidget(c);
    for(auto i = mChatPanes.begin(); i != mChatPanes.end(); ++i)
    {
        qDebug() << "Network name: " << (*i)->GetNetworkName();
    }
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
    qDebug() << "LOOKING FOR: " << networkName << target;
    for(int i = 0; i < mChatPanes.size(); i++)
    {
        qDebug() << "PANE INFO: " << mChatPanes[i]->GetNetworkName() << mChatPanes[i]->GetChannelName();
        if((mChatPanes[i]->GetNetworkName() == networkName) && (mChatPanes[i]->GetChannelName() == target))
        {
            qDebug() << "ADDMESSAGETOCHATPANE(): FOUND PANE FOR MESSAGE";
            QString messageStr = "<" + sender + "> " + message.right(message.size() -1) + "";
            mChatPanes[i]->append(messageStr);
        }
    }
}

void MainWindow::OnIrcMessage(QStringList messagePacket)
{
    qDebug() << "OnIrcMessage()";
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
    else
    {
        ChatPane* pane = GetChatPane(channel, "");
        ui->stackedWidget->setCurrentWidget(pane);
    }
}
