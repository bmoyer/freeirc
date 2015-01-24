#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConnections_triggered();
    void on_actionQuick_connect_triggered();

private:
    //Attributes
    Ui::MainWindow *ui;
    std::vector<QTextEdit> mChatPanes;
    QString mSettingsFile;

    //Operations
    void createChatPane(void);
};

#endif // MAINWINDOW_H
