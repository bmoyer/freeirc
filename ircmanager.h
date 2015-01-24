#ifndef IRCMANAGER_H
#define IRCMANAGER_H

#include <vector>
#include "ircnetwork.h"

class IrcManager
{
public:
    ~IrcManager();
    //Singleton class
    static IrcManager* GetInstance(void);

    //Operations
    void AddConnection(IrcNetwork* cn);
    void OnQuickConnect();

private:
    //Singleton class
    IrcManager();
    static bool instanceFlag;
    static IrcManager *single;

    //Attributes
    std::vector<IrcNetwork*> mCurrentNetworks;
};

#endif // IRCMANAGER_H
