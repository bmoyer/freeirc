#ifndef CONNECTIONSDIALOG_H
#define CONNECTIONSDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class ConnectionsDialog;
}

class ConnectionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionsDialog(QWidget *parent = 0);
    ~ConnectionsDialog();

private slots:
    void on_btnSaveNetwork_clicked();

    void on_btnDeleteNetwork_clicked();

    void on_networkList_currentTextChanged(const QString &currentText);

private:
    //Attributes
    Ui::ConnectionsDialog *ui;

    //Operations
    void LoadNetworkList(void);
    void LoadNetworkInfo(QString networkName);
    void ClearInputFields(void);

};

#endif // CONNECTIONSDIALOG_H
