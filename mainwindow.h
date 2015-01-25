#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <vector>
#include <QMessageBox>
#include <QDebug>
#include "chatpane.h"
#include "ircmessage.h"

#define HIT_MSG_BOX() QMessageBox q; q.exec();

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Attributes

    //Operations
    void AddChatPane(QString networkName, QString channelName);
    ChatPane* GetChatPane(QString networkName, QString channelName);
    void AddMessageToChatPane(QString networkName, QString sender, QString target, QString message);

public slots:

private slots:
    void on_actionConnections_triggered();
    void on_actionQuick_connect_triggered();
    void OnIrcMessage(QStringList messagePacket);

    void on_chatSessionTree_objectNameChanged(const QString &objectName);

    void on_chatSessionTree_clicked(const QModelIndex &index);

private:
    //Attributes
    Ui::MainWindow *ui;
    std::vector<ChatPane*> mChatPanes;
    QString mSettingsFile;

    //Operations
};

#endif // MAINWINDOW_H
