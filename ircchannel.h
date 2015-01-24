#ifndef IRCCHANNEL_H
#define IRCCHANNEL_H
#include <QString>

class IrcChannel
{
public:
    IrcChannel(QString channelName);
    ~IrcChannel();

    QString GetName(void) { return mChannelName; }

private:
    QString mChannelName;
};

#endif // IRCCHANNEL_H
