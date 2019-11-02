#include "commonheader.h"
#include "updatetipsdialog.h"
#include "ui_updatetipsdialog.h"

UpdateTipsDialog::UpdateTipsDialog(QWidget *parent) :
    CFramelessDialog(QString(QObject::tr("升级提醒")), parent),
    ui(new Ui::UpdateTipsDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setProperty("manda-value", true);
    ui->lineEdit_2->setProperty("manda-value", true);
}

UpdateTipsDialog::~UpdateTipsDialog()
{
    delete ui;
}
