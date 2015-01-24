#include "connectionsdialog.h"
#include "settingsmanager.h"
#include "ui_connectionsdialog.h"
#include "ircnetwork.h"
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

ConnectionsDialog::ConnectionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionsDialog)
{
    ui->setupUi(this);
    LoadNetworkList();
}

ConnectionsDialog::~ConnectionsDialog()
{
    delete ui;
}

void ConnectionsDialog::on_btnSaveNetwork_clicked()
{

    QString networkName = ui->editNetworkName->text();
    QString networkAddress = ui->editNetworkAddress->text();
    QString networkPort = ui->editPort->text();
    QString networkNick = ui->editNick->text();
    QString networkAutojoin = ui->checkStartupJoin->isChecked() ? ("1") : ("0");
    QString networkChannels = ui->editChannels->toPlainText();


    //Save the network info to the database
    SettingsManager *s = SettingsManager::GetInstance();
    std::vector<IrcNetwork*> savedNetworks = s->GetSavedNetworks();
    bool existingNetwork = false;
    for(int i = 0; i < savedNetworks.size(); i++)
    {
        if(networkName == savedNetworks[i]->GetName())
        {
            existingNetwork = true;
        }
    }
    if(existingNetwork)
    {
        s->UpdateIrcNetwork(networkName, networkAddress, networkPort, networkNick, networkAutojoin, networkChannels);
    }
    else
    {
        s->SaveIrcNetwork(networkName, networkAddress, networkPort, networkNick, networkAutojoin, networkChannels);
    }

    //Refresh the listview of networks
    LoadNetworkList();

    for(int i = 0; i < ui->networkList->count(); i++)
    {
        if (ui->networkList->item(i)->text() == networkName)
        {
            ui->networkList->item(i)->setSelected(true);
        }
    }
}

void ConnectionsDialog::on_btnDeleteNetwork_clicked()
{
    QString channelName = ui->editNetworkName->text();
    SettingsManager *s = SettingsManager::GetInstance();
    s->DeleteIrcNetwork(channelName);
    ClearInputFields();
    LoadNetworkList();
}


//Load network info into fields after click
void ConnectionsDialog::LoadNetworkInfo(QString networkName)
{
    SettingsManager *s = SettingsManager::GetInstance();
    std::vector<IrcNetwork*> networks = s->GetSavedNetworks();
    for(int i = 0; i < networks.size(); i++)
    {
        if(networks[i]->GetName() == networkName)
        {
            QString address = networks[i]->GetAddress();
            QString port = networks[i]->GetPort();
            QString nick = networks[i]->GetNick();
            bool joinOnStartup = networks[i]->GetJoinOnStartup();
            std::vector<IrcChannel> channels = networks[i]->GetAutojoinChannels();

            ui->editNetworkName->setText(networkName);
            ui->editNetworkAddress->setText(address);
            ui->editPort->setText(port);
            ui->editNick->setText(nick);
            ui->checkStartupJoin->setChecked(joinOnStartup);

            QString channelList;
            for(int i = 0; i < channels.size(); i++)
            {
                channelList.append(channels[i].GetName());
                if(i != (channels.size() - 1) ) channelList.append(",");
            }
            ui->editChannels->setPlainText(channelList);
        }
    }
}

//Load networks from refreshed db into list for clicking
void ConnectionsDialog::LoadNetworkList(void)
{
    ui->networkList->clear();

    SettingsManager *s = SettingsManager::GetInstance();
    s->LoadSettingsFromDB();

    std::vector<IrcNetwork*> networks = s->GetSavedNetworks();
    for(auto it = networks.begin(); it != networks.end(); ++it)
    {
        qDebug() << networks.size();
        IrcNetwork* network = *it;
        ui->networkList->addItem(network->GetName());
    }
}

void ConnectionsDialog::on_networkList_currentTextChanged(const QString &currentText)
{
    //User clicked another

    LoadNetworkInfo(currentText);
}

void ConnectionsDialog::ClearInputFields(void)
{
    ui->editChannels->clear();
    ui->editNetworkAddress->clear();
    ui->editNetworkName->clear();
    ui->editNick->clear();
    ui->editPort->clear();
}
