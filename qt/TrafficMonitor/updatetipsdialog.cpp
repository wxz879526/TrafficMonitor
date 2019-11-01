#include "updatetipsdialog.h"
#include "ui_updatetipsdialog.h"

UpdateTipsDialog::UpdateTipsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTipsDialog)
{
    ui->setupUi(this);
}

UpdateTipsDialog::~UpdateTipsDialog()
{
    delete ui;
}
