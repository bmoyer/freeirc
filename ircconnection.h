#ifndef IRCCONNECTION_H
#define IRCCONNECTION_H

#include <vector>
#include "ircchannel.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QObject>
#include "irc.h"

class IrcConnection : public QObject
{
    Q_OBJECT
public:
    IrcConnection();
    ~IrcConnection();

    void SetAddress(QString address) { mAddress = address; }
    void SetPort(QString port) { mPort = port; }
    void SetNick(QString nick) { mNick = nick; }
    void SetName(QString name) { mName = name; qDebug()  << "name set to " << name;}
    void testFun(void) { qDebug() << "hello world from testfun."; }

    void SetAutojoinChannels(std::vector<IrcChannel> channels) { mAutojoinChannels = channels; }
    static int OnEventPrivMsg(char* params, irc_reply_data* hostd, void* conn);
    static int OnEventJoin(char* params, irc_reply_data* hostd, void* conn);
    static int OnEventNames(char* params, irc_reply_data* hostd, void* conn);

    void OnMessage() { emit OnIrcMessage(); }
private:
    QString mAddress;
    QString mPort;
    QString mNick;
    QString mName;
    std::vector<IrcChannel> mAutojoinChannels;
    IRC* mConnection;

public slots:
    void StartConnection(void);

signals:
    void finished();
    void error(QString err);
    void errorString(QString err);
    void OnIrcMessage();
};

#endif //IRCCONNECTION
