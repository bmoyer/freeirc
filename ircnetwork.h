#ifndef IRCNETWORK_H
#define IRCNETWORK_H

#include "ircconnection.h"
#include "ircchannel.h"
#include <vector>
#include <QString>
#include <QObject>

class IrcNetwork : public QObject
{
    Q_OBJECT
    friend class SettingsManager;
    friend class IrcManager;
public:
    IrcNetwork();
    ~IrcNetwork();

    //Operations
    QString GetName(void) { return mName; }
    QString GetAddress(void) { return mAddress; }
    QString GetPort(void) { return mPort; }
    QString GetNick(void) { return mNick; }
    bool GetJoinOnStartup(void) { return mJoinOnStartup; }
    void SetName(QString name) { mName = name; }
    void SetAddress(QString addr) { mAddress = addr; }
    void SetPort(QString port) { mPort = port; }
    std::vector<IrcChannel> GetAutojoinChannels(void) { return mAutojoinChannels; }
    void StartConnection(void);

private:
    //Attributes
    QString mName;
    QString mAddress;
    QString mPort;
    QString mNick;
    bool mJoinOnStartup;
    std::vector<IrcChannel> mAutojoinChannels;
    IrcConnection* mpIrcConnection;

    //Operations
    void SetAutojoinChannels(std::vector<IrcChannel> channels);
    void SetJoinOnStartup(bool joinOnStartup);
    void SetNick(QString nick) { mNick = nick; }
};

#endif // IRCNETWORK_H
