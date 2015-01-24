#include "ircnetwork.h"
#include <vector>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QThread>


IrcNetwork::IrcNetwork()
{
}

IrcNetwork::~IrcNetwork()
{
    qDebug() << "ircnetwork killed";
}

void IrcNetwork::SetAutojoinChannels(std::vector<IrcChannel> channels)
{
    mAutojoinChannels = channels;
}

void IrcNetwork::SetJoinOnStartup(bool joinOnStartup)
{
    mJoinOnStartup = joinOnStartup;
}

void IrcNetwork::StartConnection()
{
    //Set up the IrcConnection
    mpIrcConnection = new IrcConnection();
    mpIrcConnection->SetNick(mNick);
    mpIrcConnection->SetAddress(mAddress);
    mpIrcConnection->SetPort(mPort);
    mpIrcConnection->SetAutojoinChannels(mAutojoinChannels);

    //Start IRC connection thread
    QThread* thread = new QThread();
    mpIrcConnection->moveToThread(thread);
    connect(mpIrcConnection, SIGNAL(errorString(QString)), this,
            SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), mpIrcConnection, SLOT(StartConnection()));
    connect(mpIrcConnection, SIGNAL(finished()), thread, SLOT(quit()));
    connect(mpIrcConnection, SIGNAL(finished()), mpIrcConnection, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
