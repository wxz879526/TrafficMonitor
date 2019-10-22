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

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

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

	QPoint m_dragPosition;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MAINWINDOW_H
