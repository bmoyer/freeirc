#include "ircmessage.h"

IrcMessage::IrcMessage(QString networkName, QString sender, QString target, QString message):
 mNetworkName(networkName)
,mSender(sender)
,mTarget(target)
,mMessage(message)
{

}

IrcMessage::~IrcMessage()
{

}

