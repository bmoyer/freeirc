#include "ircmanager.h"
#include "ircnetwork.h"
#include "settingsmanager.h"
#include "ircmessage.h"
#include <QDebug>

bool IrcManager::instanceFlag = false;
IrcManager* IrcManager::single = NULL;

IrcManager::IrcManager()
{

}

IrcManager::~IrcManager()
{
    qDebug() << "ircmanager killed";
    for(int i = 0; i < mCurrentNetworks.size(); i++)
    {
        delete mCurrentNetworks[i];
    }
    instanceFlag = false;
}

IrcManager* IrcManager::GetInstance(void)
{
    if(!instanceFlag)
    {
        single = new IrcManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

/* IrcManager::AddConnection
 * \brief Makes persistent localt copy of an IRC network and starts its connection.
 */
void IrcManager::AddConnection(IrcNetwork* cn)
{
    IrcNetwork* copy = new IrcNetwork();
    copy->SetName(cn->GetName());
    copy->SetNick(cn->GetNick());
    copy->SetAddress(cn->GetAddress());
    copy->SetPort(cn->GetPort());
    copy->SetJoinOnStartup(cn->GetJoinOnStartup());
    copy->SetAutojoinChannels(cn->GetAutojoinChannels());

    mCurrentNetworks.push_back(copy);
    mCurrentNetworks.back()->StartConnection();
}

/* IrcManager::OnQuickConnect
 * \brief Load saved networks from SettingsManager database, join each network.
 * For each saved network in the SettingsManager list, create a persistent copy
 * and add it to mCurrentNetworks. Start each network connection in mCurrentNetworks.
 */
void IrcManager::OnQuickConnect()
{
    SettingsManager* s = SettingsManager::GetInstance();
    s->LoadSettingsFromDB();

    for(int i = 0; i < s->GetSavedNetworks().size(); i++)
    {
        IrcNetwork* net = s->GetSavedNetworks()[i];

        IrcNetwork* copy = new IrcNetwork();
        copy->SetName(net->GetName());
        copy->SetNick(net->GetNick());
        copy->SetAddress(net->GetAddress());
        copy->SetPort(net->GetPort());
        copy->SetJoinOnStartup(net->GetJoinOnStartup());
        copy->SetAutojoinChannels(net->GetAutojoinChannels());
        mCurrentNetworks.push_back(copy);
    }

    for(int i = 0; i < mCurrentNetworks.size(); i++)
    {
        if(mCurrentNetworks[i]->GetJoinOnStartup())
        {
            mCurrentNetworks[i]->StartConnection();
        }
    }
}

void IrcManager::HandlePrivateMessage(IrcMessage message)
{
    QStringList q;
    q.append(message.GetNetworkName());
    q.append(message.GetSender());
    q.append(message.GetTarget());
    q.append(message.GetMessage());
    emit packet(q);
}
