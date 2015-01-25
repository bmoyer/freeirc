#ifndef CHATPANE_H
#define CHATPANE_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>

class ChatPane : public QTextEdit
{
public:
    ChatPane(QString networkName, QString channelName);
    ~ChatPane();

    QString GetNetworkName(void) { return mNetworkName; }
    QString GetChannelName(void) { return mChannelName; }

private:
    //Attributes
    QString mNetworkName;
    QString mChannelName;
};

#endif // CHATPANE_H
