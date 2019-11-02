#include "mainwindow.h"
#include <QMessageBox>
#include "updatetipsdialog.h"
#include <QFile>
#include <QDebug>
#include "globalobjects.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    // qss
    {
        QFile file(":/style.qss");
        if (file.open(QFile::ReadOnly))
        {
            QString strQss = file.readAll();
            qApp->setStyleSheet(strQss);
        }
    }

    GlobalObjects::init();

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("Not support tray"));
        return 1;
    }

    UpdateTipsDialog dlg;
    dlg.exec();


   // MainWindow w;
    //w.show();

    return app.exec();
}
