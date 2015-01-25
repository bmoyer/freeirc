#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <QString>

class IrcMessage
{
public:
    IrcMessage(QString networkName, QString sender, QString target, QString message);
    ~IrcMessage();

    QString GetNetworkName(void) { return mNetworkName; }
    QString GetSender(void) { return mSender; }
    QString GetMessage(void) { return mMessage; }
    QString GetTarget(void) { return mTarget; }

private:
    QString mNetworkName;
    QString mSender;
    QString mMessage;
    QString mTarget;
};

#endif // IRCMESSAGE_H
