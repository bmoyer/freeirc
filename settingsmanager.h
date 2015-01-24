#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QString>
#include <QStringList>
#include "ircnetwork.h"


class SettingsManager
{
    //friend class IrcNetwork;

public:
    //Singleton class
    static SettingsManager* GetInstance(void);
    ~SettingsManager();

    //Attributes

    //Operations
    int queryCallback(void *data, int argc, char **argv, char **azColName);
    std::vector<IrcNetwork*> GetSavedNetworks(void) { return mSavedNetworks; }
    void LoadSettingsFromDB(void);
    void CreateTables(void);
    void SaveIrcNetwork(QString name, QString address, QString port, QString nick,
                                     QString autojoin, QString channels);
    void UpdateIrcNetwork(QString name, QString address, QString port, QString nick,
                                     QString autojoin, QString channels);
    void DeleteIrcNetwork(QString name);
private:
    //Singleton class
    SettingsManager();
    static bool instanceFlag;
    static SettingsManager *single;

    //Attributes
    QString mSettingsDbPath;
    std::vector<IrcNetwork*> mSavedNetworks;
    std::vector<QStringList> mDataBuffer;

    //Operations

    //Database functions
    void FireAction(QString query);
    void FireQuery(QString query, const char* data);
    static int actionCallback(void *NotUsed, int argc, char **argv, char **azColName);
    static int c_callback(void *param, int argc, char **argv, char **azColName);
};

#endif // SETTINGSMANAGER_H
