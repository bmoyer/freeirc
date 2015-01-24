#include "settingsmanager.h"
#include "ircnetwork.h"
#include "commonutils.h"
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include <QByteArray>
#include <QDebug>
#include <QStringList>

bool SettingsManager::instanceFlag = false;
SettingsManager* SettingsManager::single = NULL;

SettingsManager::SettingsManager() :
    mSettingsDbPath("/Users/benmoyer/Tmp/test.sqlite3")
{
}

SettingsManager::~SettingsManager()
{
    instanceFlag = false;
}

SettingsManager* SettingsManager::GetInstance(void)
{
    if(!instanceFlag)
    {
        single = new SettingsManager();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

void SettingsManager::CreateTables(void)
{
    //Create NETWORK
    QString createNetworkTable = "CREATE TABLE NETWORK("  \
                                 "NAME CHAR(50) PRIMARY KEY     NOT NULL," \
                                 "ADDRESS        CHAR(50)    NOT NULL," \
                                 "PORT       CHAR(5)    NOT NULL," \
                                 "NICK       CHAR(20) NOT NULL," \
                                 "AUTOJOIN	  INT	   NOT NULL," \
                                 "CHANNELS   CHAR(100) );";
    FireAction(createNetworkTable);
}

void SettingsManager::SaveIrcNetwork(QString name, QString address, QString port, QString nick,
                                     QString autojoin, QString channels)
{
    QString saveNetworkQuery =
                    QString("INSERT INTO NETWORK (NAME,ADDRESS,PORT,NICK,AUTOJOIN,CHANNELS) VALUES ('") +
                    name + QString("', '") + address + QString("', '") + port + "', '" +
                    nick + QString("', '") + autojoin + QString("', '") + channels + QString("');");

    FireAction(saveNetworkQuery);
}

void SettingsManager::UpdateIrcNetwork(QString name, QString address, QString port, QString nick,
                                     QString autojoin, QString channels)
{
    QString updateNetworkQuery =
                    QString("UPDATE NETWORK SET NAME='") + name + QString("',ADDRESS='") +
                    address + QString("',PORT='") + port + QString("',NICK='") + nick +
                    QString("',AUTOJOIN='") + autojoin + QString("',CHANNELS='") + channels + QString("'") +
                    QString(" WHERE NAME=='") + name + QString("'");

    FireAction(updateNetworkQuery);
}

void SettingsManager::DeleteIrcNetwork(QString name)
{
    QString deleteNetworkQuery =
            QString("DELETE FROM NETWORK WHERE NAME=='") + name + QString("';");

    FireAction(deleteNetworkQuery);
}

/* SettingsManager::LoadSettingsFromDB()
 * Loads the database contents into member variables of the singleton SettingsManager.
 * This is called after all DB operations, so that the SettingsManager is always up to date.
 */
void SettingsManager::LoadSettingsFromDB(void)
{
    mDataBuffer.clear();
    mSavedNetworks.clear();

    QString select = "SELECT * FROM NETWORK";
    FireQuery(select, "This is a data");

    //Read data about NETWORK table
    for(int i = 0; i < mDataBuffer.size(); i++)
    {
        //Initialize network
        QStringList data = mDataBuffer[i];
        qDebug() << data << " was the data";
        if(data.size() != 6) return;
        IrcNetwork* network = new IrcNetwork();
        network->SetName(data[0]);
        network->SetAddress(data[1]);
        network->SetPort(data[2]);

        QStringList autojoinChannels = CommonUtils::ParseChannelNames(data[5]);
        std::vector<IrcChannel> networkChannels;
        for(auto it = autojoinChannels.begin(); it != autojoinChannels.end(); ++it)
        {
            networkChannels.push_back(*it);
        }

        network->SetAutojoinChannels(networkChannels);
        network->SetJoinOnStartup(data[4].toInt());
        network->SetNick(data[3]);
        network->SetPort(data[2]);

        mSavedNetworks.push_back(network);
    }

    mDataBuffer.clear();
}

void SettingsManager::FireAction(QString query)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;

    /* Open database */
    QByteArray qd = mSettingsDbPath.toUtf8();
    char* path = qd.data();
    rc = sqlite3_open(path, &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }else{
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    QByteArray baQuery = query.toUtf8();
    sql = baQuery.data();
    //sql = query;//CommonUtils::QStringToChars(query);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, actionCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Action done successfully\n");
    }
    sqlite3_close(db);
}

int SettingsManager::actionCallback(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void SettingsManager::FireQuery(QString query, const char* data)
{

    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;
    //const char* data = "Callback called";

    /* Open database */
    QByteArray qd = mSettingsDbPath.toUtf8();
    char* path = qd.data();
    rc = sqlite3_open(path, &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }else{
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    QByteArray baQuery = query.toUtf8();
    sql = baQuery.data();

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, this->c_callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Query done successfully\n");
    }
    sqlite3_close(db);
}


int SettingsManager::queryCallback(void *data, int argc, char **argv, char **azColName){
    QStringList channel;
    for(int i = 0; i < argc; i++){
        channel.append(QString(argv[i]));
    }
    mDataBuffer.push_back(channel);
    return 0;
}


int SettingsManager::c_callback(void *param, int argc, char **argv, char **azColName)
{
   SettingsManager *s = SettingsManager::GetInstance();
   return s->queryCallback(param, argc, argv, azColName);
}
