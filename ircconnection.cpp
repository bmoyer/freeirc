#include "ircconnection.h"
#include <QDebug>
#include <QThread>
#include <QIODevice>
#include "commonutils.h"
#include <QWidgetList>
#include <QApplication>
#include "ircmanager.h"

#include "mainwindow.h"
IrcConnection::IrcConnection()
{

    //connect(this,
}

IrcConnection::~IrcConnection()
{
    qDebug() << "ircconnection KILLED";
}

void IrcConnection::StartConnection()
{
    //IRC conn(mName);
    mConnection = new IRC(mName);
    //conn.setname(mName);
    //conn.hook_irc_command("376", this->end_of_motd); /* hook the end of MOTD message */
    char* address = strdup(CommonUtils::QStringToChars(mAddress));
    int port = mPort.toInt();
    QString n = mNick;
    char* nick = strdup(CommonUtils::QStringToChars(n));
    char* user = nick;
    char* name =  nick;
    char* password = "";

    qDebug() << "Connection data: " << nick << port << address << user << name << password;
    mConnection->hook_irc_command("PRIVMSG", this->OnEventPrivMsg); /* hook private messages */
    mConnection->hook_irc_command("JOIN", this->OnEventJoin); /* hook private messages */
    mConnection->hook_irc_command("353", this->OnEventNames); /* hook private messages */
    mConnection->start(address, port, nick, user, name, password); /* connect to a server */
    for(int i = 0; i < mAutojoinChannels.size(); i++)
    {
        QString channel = mAutojoinChannels[i].GetName();
        qDebug() << channel;
        mConnection->join(CommonUtils::QStringToChars(channel));
    }
    mConnection->message_loop(); /* start the message loop */

    //emit finished();
}


int IrcConnection::OnEventPrivMsg(char* params, irc_reply_data* hostd, void* conn) /* our callback function */
{
    IRC* irc_conn=(IRC*)conn;
    qDebug() << irc_conn->getname() << params << hostd->host << " " << hostd->ident << " " << hostd->nick << " "<< hostd->target;
    qDebug() << hostd->host;
    qDebug() << hostd->ident;
    qDebug() << hostd->nick;
    qDebug() << hostd->target;

    //QString test = QString::fromUtf8(hostd->host);
    IrcMessage msg(irc_conn->getname(), QString::fromUtf8(hostd->nick), QString::fromUtf8(hostd->target),
                   QString::fromUtf8(params));
    IrcManager* ircM = IrcManager::GetInstance();
    ircM->HandlePrivateMessage(msg);
    return 0;
}

int IrcConnection::OnEventJoin(char* params, irc_reply_data* hostd, void* conn) /* our callback function */
{
    qDebug() << "JOIN PARAMS: \n";
    IRC* irc_conn=(IRC*)conn;
    qDebug() << params << hostd->host << " " << hostd->ident << " " << hostd->nick << " "<< hostd->target;
    return 0;
}


int IrcConnection::OnEventNames(char* params, irc_reply_data* hostd, void* conn) /* our callback function */
{
    qDebug() << "NAMES PARAMS: \n";
    IRC* irc_conn=(IRC*)conn;

    //qDebug() << params;
    printf("%s", params);
    qDebug() << params << hostd->host << " " << hostd->ident << " " << hostd->nick << " "<< hostd->target;
    return 0;
}
