#include "ircmanager.h"
#include "ircnetwork.h"
#include "settingsmanager.h"
#include <QDebug>

bool IrcManager::instanceFlag = false;
IrcManager* IrcManager::single = NULL;

IrcManager::IrcManager()
{

}

IrcManager::~IrcManager()
{
    qDebug() << "ircmanager killed";
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

void IrcManager::AddConnection(IrcNetwork* cn)
{
    mCurrentNetworks.push_back(cn);
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
        copy->SetAutojoinChannels(net->GetAutojoinChannels());
        mCurrentNetworks.push_back(copy);
    }

    for(int i = 0; i < mCurrentNetworks.size(); i++)
    {
        mCurrentNetworks[i]->StartConnection();
    }
}
