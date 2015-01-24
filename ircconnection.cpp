#include "ircconnection.h"
#include <QDebug>
#include <QThread>
#include <QIODevice>
#include "commonutils.h"

IrcConnection::IrcConnection()
{
}

IrcConnection::~IrcConnection()
{
    qDebug() << "ircconnection KILLED";
}

void IrcConnection::StartConnection()
{
    IRC conn;
    //conn.hook_irc_command("376", this->end_of_motd); /* hook the end of MOTD message */
    char* address = strdup(CommonUtils::QStringToChars(mAddress));
    int port = mPort.toInt();
    char* nick = strdup(CommonUtils::QStringToChars("testclient42"));
    char* user = nick;
    char* name =  nick;
    char* password = "";

    qDebug() << "Connection data:\n";
    qDebug() << nick;
    qDebug() << port;
    qDebug() << address;
    qDebug() << user;
    qDebug()<< name;
    qDebug() << password;
    conn.hook_irc_command("PRIVMSG", this->OnEventPrivMsg); /* hook private messages */
    conn.start(address, port, nick, user, name, password); /* connect to a server */
    conn.join("#testclient");
    for(int i = 0; i < mAutojoinChannels.size(); i++)
    {
        QString channel = mAutojoinChannels[i].GetName();
        qDebug() << channel;
        conn.join(CommonUtils::QStringToChars(channel));
    }
    conn.message_loop(); /* start the message loop */

    //emit finished();
}

int IrcConnection::OnEventPrivMsg(char* params, irc_reply_data* hostd, void* conn) /* our callback function */
{
    IRC* irc_conn=(IRC*)conn;
    qDebug() << params << hostd->host << " " << hostd->ident << " " << hostd->nick << " "<< hostd->target;
    qDebug() << hostd->host;
    qDebug() << hostd->ident;
    qDebug() << hostd->nick;
    qDebug() << hostd->target;
    //irc_conn->join("#pixelz"); /* join the channel #magpie */
    return 0;
}
