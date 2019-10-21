#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("UTF-8")

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void SetupTray();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;

    QAction *m_pConnDetailAction;
    QAction *m_pTopMostAction;
    QAction *m_pMouseHackAction;
    QAction *m_pLockWndPosAction;
    QAction *m_pShowTrayNotifyAction;
    QAction *m_pMoreInfoAction;
    QAction *m_pTaskWndAction;
    QAction *m_pHideMainWndAction;
    QAction *m_pSettingsAction;
    QAction *m_pAboutAction;
    QAction *m_pExitAction;
};

#endif // MAINWINDOW_H
