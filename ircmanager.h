#ifndef IRCMANAGER_H
#define IRCMANAGER_H

#include <vector>
#include "ircnetwork.h"
#include "ircmessage.h"
#include "mainwindow.h"
#include <QObject>

class IrcManager : public QObject
{
    Q_OBJECT
public:
    ~IrcManager();
    //Singleton class
    static IrcManager* GetInstance(void);

    //Operations
    void AddConnection(IrcNetwork* cn);
    void OnQuickConnect();
    void SetMainWindowPtr(MainWindow* pMainWindow) { mpMainWindow = pMainWindow; }
    void HandlePrivateMessage(IrcMessage);
    std::vector<IrcNetwork* > GetCurrentNetworks(void) { return mCurrentNetworks; }

signals:
    void packet(QStringList list);

private:
    //Singleton class
    IrcManager();
    static bool instanceFlag;
    static IrcManager *single;

    //Attributes
    std::vector<IrcNetwork*> mCurrentNetworks;
    MainWindow* mpMainWindow;
};


#endif // IRCMANAGER_H
