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

    void SetAutojoinChannels(std::vector<IrcChannel> channels) { mAutojoinChannels = channels; }
    static int OnEventPrivMsg(char* params, irc_reply_data* hostd, void* conn);

private:
    QString mAddress;
    QString mPort;
    QString mNick;
    std::vector<IrcChannel> mAutojoinChannels;

public slots:
    void StartConnection(void);
    void processgps() {
        qDebug() << "processgps()" << QThread::currentThreadId();
        qDebug("hello from ircconnection");
    }

signals:
    void finished();
    void error(QString err);
    void errorString(QString err);
};

#endif //IRCCONNECTION
